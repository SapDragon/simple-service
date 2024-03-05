#include "service.hpp"

#include <memory>

#include "core/logger/logger.hpp"
#include "feature/installer/installer.hpp"

int main(int argc, char* argv[])
{
	if (argc > 1)
	{
		try
		{
			Features::pInstaller = std::make_unique < Features::C_Installer >("TestService");

			if (strcmp(argv[1], "install") == 0)
			{
				Features::pInstaller->Install();
				printf("Service installed\n");
				return 0;
			}
			else if (strcmp(argv[1], "uninstall") == 0)
			{
				Features::pInstaller->Uninstall();
				printf("Service uninstalled\n");
				return 0;
			}

		}
		catch (const std::exception& e)
		{
			printf("Installer failed with exception: %s\n", e.what());
			return 1;
		}
	}
	printf("0");

	SERVICE_TABLE_ENTRYA serviceTable[] =
	{
		{ (LPSTR) "TestService", Service::Entry },
		{ nullptr, nullptr }
	};

	if (!StartServiceCtrlDispatcherA(serviceTable))
	{
		Core::pLogger = std::make_unique<Core::C_Logger>("Service");
		Core::pLogger->Error("StartServiceCtrlDispatcherA failed with error {}", GetLastError());
		return 1;
	}	

	return 0;
}