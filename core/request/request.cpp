#include "request.hpp"

#include <string_view>
#include <stdexcept>

#include <wininet.h>
#include <fmt/format.h>

Core::C_RequestManager::C_RequestManager()
{
	m_hInternet = InternetOpenA("SomeTest", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);

	if (m_hInternet == NULL)
		throw std::runtime_error(fmt::format(fmt::runtime("InternetOpen failed: {}"), GetLastError()));

}
std::string Core::C_RequestManager::GetRequest(const std::string_view szUrl)
{
	HINTERNET hUrl = InternetOpenUrlA(m_hInternet, szUrl.data(), NULL, 0, INTERNET_FLAG_RELOAD, 0);

	if (!hUrl)
		throw std::runtime_error(fmt::format(fmt::runtime("InternetOpenUrl failed: {}"), GetLastError()));

	char szBuffer[1024];
	DWORD dwRead;
	std::string sResult;
	while (InternetReadFile(hUrl, szBuffer, sizeof(szBuffer) - 1, &dwRead) && dwRead)
	{
		szBuffer[dwRead] = 0;
		sResult += szBuffer;
	}

	InternetCloseHandle(hUrl);
	return sResult;
}
