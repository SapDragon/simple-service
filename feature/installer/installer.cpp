#include "installer.hpp"

#include <Windows.h>
#include <strsafe.h>

#include "../../core/logger/logger.hpp"

void Features::C_Installer::Install()
{
    SC_HANDLE hServiceManager;
    SC_HANDLE hService;
    TCHAR arrUnquotedPath[MAX_PATH];

    if (!GetModuleFileName(NULL, arrUnquotedPath, MAX_PATH))
    {
		throw std::runtime_error(fmt::format("Cannot install service ({})", GetLastError()));
        return;
    }
    
    TCHAR szPath[MAX_PATH];
    StringCbPrintfA(szPath, MAX_PATH, TEXT("\"%s\""), arrUnquotedPath);

    hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);            

    if (!hServiceManager)
    {
		throw std::runtime_error(fmt::format("OpenSCManager failed ({})", GetLastError()));
        return;
    }

    // @note / SapDragon: Craete service 
    hService = CreateService(
        hServiceManager,              // SCM database 
        m_sServiceName.c_str(),                   // name of service 
        m_sServiceName.c_str(),                   // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 

    if (!hService)
    {
		throw std::runtime_error(fmt::format("CreateService failed ({})", GetLastError()));
        CloseServiceHandle(hServiceManager);
        return;
    }
    
	Core::pLogger->Info("Service installed successfully");

    CloseServiceHandle(hService);
    CloseServiceHandle(hServiceManager);
}

void Features::C_Installer::Uninstall()
{
	SC_HANDLE hServiceManager;
	SC_HANDLE hService;

	hServiceManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (!hServiceManager)
	{
		throw std::runtime_error(fmt::format("OpenSCManager failed ({})", GetLastError()));
		return;
	}

	hService = OpenService(hServiceManager, m_sServiceName.c_str(), SERVICE_ALL_ACCESS);

	if (!hService)
	{
		throw std::runtime_error(fmt::format("OpenService failed ({})", GetLastError()));
		CloseServiceHandle(hServiceManager);
		return;
	}

	if (!DeleteService(hService))
	{
		throw std::runtime_error(fmt::format("DeleteService failed ({})", GetLastError()));
		CloseServiceHandle(hService);
		CloseServiceHandle(hServiceManager);
		return;
	}

	Core::pLogger->Info("Service uninstalled successfully");

	CloseServiceHandle(hService);
	CloseServiceHandle(hServiceManager);
}
