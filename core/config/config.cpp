#include "config.hpp"
#include <filesystem>
#include <fstream>
#include <array>

#include <nlohmann/json.hpp>
Core::C_ConfigManager::C_ConfigManager(std::string sConfigName, Config_t& defaultConfig)
{
	// get main drive
	std::string sWorkDirectory = std::filesystem::current_path().root_name().string() + "\\" + "MyService";


	if (!std::filesystem::exists(sWorkDirectory))
		std::filesystem::create_directory(sWorkDirectory);

	m_sConfigPath = sWorkDirectory + "\\" + sConfigName;
	m_Config = defaultConfig;
	
	ParseConfig();
}

void Core::C_ConfigManager::ParseConfig()
{
	// @note / SapDragon: Ñheck if file exists
	if (!std::filesystem::exists(m_sConfigPath))
	{
		WriteConfig();
		return;
	}

	std::ifstream jsonFile(m_sConfigPath);

	if (!jsonFile.is_open())
	{
		throw std::runtime_error("Can't open file for reading");
	}

	nlohmann::json jsonData = nlohmann::json::parse(jsonFile);

	std::array <std::string, 6> sFields = { "DataType", "Interval", "WorkMode", "LogDirectory", "XMLUrl", "JsonUrl" };

	for (auto& sField : sFields)
	{
		if (jsonData.find(sField) == jsonData.end())
		{
			WriteConfig();
			return;
		}
	}
	
	m_Config = {
		jsonData["DataType"].get<std::string>(),
		jsonData["Interval"].get<uint64_t>(),
		static_cast<EWorkMode>(jsonData["WorkMode"].get<uint8_t>()),
		jsonData["LogDirectory"].get<std::string>(),
		jsonData["XMLUrl"].get<std::string>(),
		jsonData["JsonUrl"].get<std::string>()
	};
}

Core::Config_t Core::C_ConfigManager::GetConfig() const
{
	return m_Config;
}

void Core::C_ConfigManager::WriteConfig()
{
	std::ofstream jsonFile(m_sConfigPath);

	if (!jsonFile.is_open())
	{
		throw std::runtime_error("Can't open file for writing");
	}

	nlohmann::json jsonData = {
		{"DataType", m_Config.m_sDataType},
		{"Interval", m_Config.m_uInterval},
		{"WorkMode", static_cast<uint8_t>(m_Config.m_uWorkMode)},
		{"LogDirectory", m_Config.m_sLogDirectory },
		{"XMLUrl", m_Config.m_sXMLUrl},
		{"JsonUrl", m_Config.m_sJsonUrl}
	};

	jsonFile << jsonData.dump(4);
	jsonFile.close();
}
