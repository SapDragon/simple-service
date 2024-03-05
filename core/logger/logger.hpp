#pragma once

#include <cstdio>
#include <memory>
#include <type_traits>
#include <fstream>
#include <chrono>

#include <fmt/chrono.h>
#include <fmt/format.h>

#include <Windows.h>

namespace Core
{
	class C_Logger
	{
	public:
		C_Logger(std::string szLoggerName);
		~C_Logger();

		template < typename... tArgs >
		constexpr void BaseLog(std::string sLogType, const std::string_view szFormat, tArgs&&... argList)
		{
			std::string sFormattedString = fmt::format(fmt::runtime(szFormat), std::forward< tArgs >(argList)...);
			std::string sFormattedTime = fmt::format("{:%Y-%m-%d %H-%M-%S}", std::chrono::system_clock::now());
			sFormattedString = fmt::format("{} | {}: {}", sLogType, sFormattedTime, sFormattedString);

			OutputDebugStringA(sFormattedString.c_str());
			m_logFile << sFormattedString << std::endl;
		}

		template< typename... tArgs >
		constexpr void Trace(const std::string_view szFormat, tArgs&&... argList)
		{
			this->BaseLog("TRACE", szFormat, std::forward< tArgs >(argList)...);
		}

		template< typename... tArgs >
		constexpr void Debug(const std::string_view szFormat, tArgs&&... argList)
		{
			this->BaseLog("DEBUG", szFormat, std::forward< tArgs >(argList)...);
		}

		template< typename... tArgs >
		constexpr void Info(const std::string_view szFormat, tArgs&&... argList)
		{
			this->BaseLog("INFO", szFormat, std::forward< tArgs >(argList)...);
		}

		template< typename... tArgs >
		constexpr void Warning(const std::string_view szFormat, tArgs&&... argList)
		{
			this->BaseLog("WARNING", szFormat, std::forward< tArgs >(argList)...);
		}

		template< typename... tArgs >
		constexpr void Error(const std::string_view szFormat, tArgs&&... argList)
		{
			this->BaseLog("ERROR", szFormat, std::forward< tArgs >(argList)...);
		}

		
	private:
		std::string m_sLoggerName;
		std::ofstream m_logFile;
	};

	inline std::unique_ptr< C_Logger > pLogger = nullptr;
}; // namespace Core