#pragma once
#include <string>
#include <memory>
#include <atomic>
namespace Features
{
	class C_CurrencyParser
	{
	public:
		C_CurrencyParser();
		~C_CurrencyParser();
	private:
		std::atomic < bool > m_bIsRunning = false;
		std::string ConvertFromJsonToCsv(const std::string_view sJson);

		void ThreadFunction();
	};

	inline std::unique_ptr< C_CurrencyParser > pCurrencyParser = nullptr;
}