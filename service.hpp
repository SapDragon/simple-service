#pragma once
#include <Windows.h>

namespace Service
{
	namespace Internal
	{
		inline SERVICE_STATUS          serviceStatus;
		inline SERVICE_STATUS_HANDLE   hServiceStatus;
		inline HANDLE 				hServiceStopEvent;

		void SetNewServiceStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
	}

	void WINAPI Entry(DWORD dwArgc, LPTSTR* lpszArgv);
	void WINAPI ControlHandler(DWORD dwControl);
}