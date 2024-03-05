#pragma once
#include <string>
#include <memory>
namespace Features
{
	class C_Installer
	{
	public:
		C_Installer(std::string sServiceName) : m_sServiceName(sServiceName) {}

		void Install();
		void Uninstall();
	private:
		std::string m_sServiceName;
	};
	
	inline std::unique_ptr< C_Installer > pInstaller = nullptr;
}