#include "logger.hpp"

#include <array>
#include <memory>
#include <filesystem>

#include "fmt/format.h"

namespace Core
{
	C_Logger::C_Logger(std::string szLoggerName)
	{	
		std::string sLogPath = fmt::format("{}\\{}\\info.log", std::filesystem::current_path().root_name().string(), "MyService");

		m_logFile = std::ofstream(sLogPath, std::ios::app);

		if (!m_logFile.is_open())
			throw std::runtime_error("Can't open log file");

		m_sLoggerName = szLoggerName;
	}
	C_Logger::~C_Logger()
	{
		m_logFile.close();
	}
} // namespace Core