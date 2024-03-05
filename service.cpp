#include "service.hpp"

#include "core/config/config.hpp"
#include "core/logger/logger.hpp"
#include "core/request/request.hpp"

#include "feature/parser/parser.hpp"

#define SERVICE_CONTROL_RELOAD_CONFIG 0xDEAD

void Service::Internal::SetNewServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint)
{
    static DWORD dwCheckPoint = 1;

    serviceStatus.dwCurrentState = dwCurrentState;
    serviceStatus.dwWin32ExitCode = dwWin32ExitCode;
    serviceStatus.dwWaitHint = dwWaitHint;

    if (dwCurrentState == SERVICE_START_PENDING)
        serviceStatus.dwControlsAccepted = 0;
    else serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

    if ((dwCurrentState == SERVICE_RUNNING) ||
        (dwCurrentState == SERVICE_STOPPED))
        serviceStatus.dwCheckPoint = 0;
    else
        serviceStatus.dwCheckPoint = dwCheckPoint++;

    SetServiceStatus(hServiceStatus, &serviceStatus);
}

void __stdcall Service::ControlHandler(DWORD dwControl)
{
    Core::pLogger->Info("Service control handler called with control code {}", dwControl);

    switch (dwControl)
    {
    case SERVICE_CONTROL_STOP:
        Core::pLogger->Info("Deinitializing service");

        Internal::SetNewServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);


		SetEvent(Internal::hServiceStopEvent);

        break;

    case SERVICE_CONTROL_RELOAD_CONFIG:
        Core::pLogger->Info("Reloading configuration");

        Core::pConfig->ParseConfig();
        
        // @note / SapDragon: This is a great option to reset the wait until the next cycle. 
        // I haven't used it because it doesn't make sense right now, but it can be done that way!
        // Implement the FORCE FETCH code in the same way
        //Features::pCurrencyParser.reset();
		//Features::pCurrencyParser = std::make_unique<Features::C_CurrencyParser>();
        Service::Internal::SetNewServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);
        break;

    case SERVICE_CONTROL_INTERROGATE:
        break;


    }
}

void WINAPI Service::Entry(DWORD dwArgc, LPTSTR* lpszArgv)
{
    Core::pLogger = std::make_unique<Core::C_Logger>("TestService");

    Core::pLogger->Info("Service entry point called");
    
    Internal::serviceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    Internal::serviceStatus.dwServiceSpecificExitCode = 0;
    Internal::hServiceStatus = RegisterServiceCtrlHandler("TestService", ControlHandler);
    if (!Internal::hServiceStatus)
    {
        Core::pLogger->Error("Failed to register service control handler");
        return;
    }

    Core::pLogger->Info("Service control handler registered");

    Internal::SetNewServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

    Core::Config_t defaultConfig;
    defaultConfig.m_sDataType = "json";
    defaultConfig.m_sLogDirectory = "log";
    defaultConfig.m_uInterval = 10000;
    defaultConfig.m_uWorkMode = Core::EWorkMode::WORK_MODE_OVERWRITE;
    defaultConfig.m_sXMLUrl = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange";
    defaultConfig.m_sJsonUrl = "https://bank.gov.ua/NBUStatService/v1/statdirectory/exchange?json";

    Core::pConfig = std::make_unique<Core::C_ConfigManager>("config.json", defaultConfig);

    Core::pLogger->Info("Configuration manager created");

    Core::pRequestManager = std::make_unique<Core::C_RequestManager>();

	Internal::hServiceStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
	if (!Internal::hServiceStopEvent)
	{
		Core::pLogger->Error("Failed to create service stop event");
		return;
	}

	Internal::SetNewServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);


    Features::pCurrencyParser = std::make_unique<Features::C_CurrencyParser>();

	WaitForSingleObject(Internal::hServiceStopEvent, INFINITE);

	Core::pLogger->Info("Service stop event received");

    Features::pCurrencyParser.reset();

    Core::pConfig.reset();
    Core::pLogger.reset();
    Core::pRequestManager.reset();

    Service::Internal::SetNewServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);

}
