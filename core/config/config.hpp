#pragma once
#include <string>
#include <memory>

namespace Core
{
	enum EWorkMode : uint8_t
	{
		WORK_MODE_OVERWRITE = 0,
		WORK_MODE_CREATE_NEW = 1
	};

	struct Config_t
	{
		// @note / SapDragon: Type of serialization (JSON, XML, CSV)
		std::string m_sDataType;
		// @note / SapDragon: Interval between requests in milliseconds
		uint64_t m_uInterval;
		// @note / SapDragon: Work mode ( 0 - overwriting, 1 - appending, 2 - creating new file)
		EWorkMode m_uWorkMode;
		// @note / SapDragon: Only for "creating new file" mode
		std::string m_sLogDirectory;

		std::string m_sXMLUrl;
		std::string m_sJsonUrl;
		
	};

	class C_ConfigManager
	{
	public:
		C_ConfigManager(std::string sConfigName, Config_t& defaultConfig);
		// @note / SapDragon: Right now there is one instance of this class under one config, 
		// perhaps we shouldn't reload it, but just create a new instance by deleting the old one?
		void ParseConfig();

		Config_t GetConfig() const;
	private:
		void WriteConfig();

		Config_t m_Config;
		std::string m_sConfigPath;
	};

	inline std::unique_ptr <C_ConfigManager> pConfig = nullptr;
} // namespace Core