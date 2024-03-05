#pragma once
#include <string>
#include <memory>
#include <Windows.h>
namespace Core
{
	class C_RequestManager
	{
	public:
		C_RequestManager();
		
		std::string GetRequest(const std::string_view sUrl);
	private:
		HANDLE m_hInternet = nullptr;
	};

	inline std::unique_ptr< C_RequestManager > pRequestManager = nullptr;
} // namespace Core