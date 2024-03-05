#include "parser.hpp"
#include <thread>
#include <sstream>
#include <filesystem>
#include <fstream>

#include "nlohmann/json.hpp"
#include <fmt/chrono.h>
#include <fmt/core.h>

#include "../../core/config/config.hpp"
#include "../../core/logger/logger.hpp"
#include "../../core/request/request.hpp"

Features::C_CurrencyParser::C_CurrencyParser()
{
	m_bIsRunning = true;
	std::thread currencyThread(&C_CurrencyParser::ThreadFunction, this);
	currencyThread.detach();
}

Features::C_CurrencyParser::~C_CurrencyParser()
{
	m_bIsRunning = false;
}

std::string Features::C_CurrencyParser::ConvertFromJsonToCsv(const std::string_view sJson)
{
    nlohmann::json jsonData = nlohmann::json::parse(sJson);

    if (jsonData.empty())
        return "";

    std::stringstream ssResult;

    // @note / SapDragon: Write headers
    const auto& firstObject = jsonData[0];
    for (auto it = firstObject.begin(); it != firstObject.end(); ++it) {
        ssResult << it.key();
        if (std::next(it) != firstObject.end())
            ssResult << ",";
    }
    ssResult << "\n";

    // @note / SapDragon: Write data
    for (const auto& item : jsonData) {
        for (auto it = item.begin(); it != item.end(); ++it) {
            ssResult << it.value();
            if (std::next(it) != item.end())
                ssResult << ",";
        }
        ssResult << "\n";
    }

    return ssResult.str();
}

void Features::C_CurrencyParser::ThreadFunction()
{
    while (m_bIsRunning)
    {
        Core::Config_t currentConfig = Core::pConfig->GetConfig();
        try
        {
            std::string sFinalData;
            // @note / SapDragon: Too bad C++ still doesn't have a switchcase for strings ( Although it can be implemented via fnv1a =) )
            if (currentConfig.m_sDataType == "json")
                sFinalData = Core::pRequestManager->GetRequest(currentConfig.m_sJsonUrl);
            else if (currentConfig.m_sDataType == "xml")
                sFinalData = Core::pRequestManager->GetRequest(currentConfig.m_sXMLUrl);
            else if (currentConfig.m_sDataType == "csv")
            {
                std::string sJsonData = Core::pRequestManager->GetRequest(currentConfig.m_sJsonUrl);

                if (sJsonData.empty())
                {
                    throw std::runtime_error("Failed to get JSON data for CSV");
                }

                sFinalData = ConvertFromJsonToCsv(sJsonData);
            }
            else
            {
                throw std::runtime_error("Unknown data type. Check config!");
                continue;
            }

            if (sFinalData.empty())
            {
                throw std::runtime_error(fmt::format("Failed to get data from {}", currentConfig.m_sDataType));
                continue;
            }

            std::string sFileName;
            if (currentConfig.m_uWorkMode == Core::WORK_MODE_CREATE_NEW)
                sFileName = fmt::format(fmt::runtime("{}." + currentConfig.m_sDataType), fmt::format("{:%Y-%m-%d %H-%M-%S}", std::chrono::system_clock::now()));
            else
                sFileName = fmt::format("dump.{}", currentConfig.m_sDataType);
            std::ofstream outputFile;

            // @note / SapDragon: Is the full path used or relative to this folder?
            if (std::filesystem::path(currentConfig.m_sLogDirectory).is_relative())
            {
                std::string sWorkDirectory = std::filesystem::current_path().root_name().string() + "\\" + "MyService" + "\\" + currentConfig.m_sLogDirectory;
                if (!std::filesystem::exists(sWorkDirectory))
                    std::filesystem::create_directories(sWorkDirectory);

                outputFile.open(sWorkDirectory + "\\" + sFileName);
            }
            else
            {
                outputFile.open(currentConfig.m_sLogDirectory + "\\" + sFileName);
            }

            if (!outputFile.is_open())
            {
                throw std::runtime_error(fmt::format("Failed to open file {}", sFileName));
                continue;
            }
            outputFile.clear();
            outputFile << sFinalData;
            outputFile.close();

            Core::pLogger->Info("Data has been written to file {}", sFileName);
		}
		catch (const std::exception& e)
		{
			Core::pLogger->Error(e.what());
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(currentConfig.m_uInterval));
    }
}
