#include "convert.hpp"

std::string convert::utf16_to_utf8(const std::wstring& wstring) {
	if (wstring.empty())
		return std::string();

	int result_size = WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), NULL, 0, NULL, NULL);
	std::string result(result_size, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstring[0], (int)wstring.size(), &result[0], result_size, NULL, NULL);

	return result;
}

std::wstring convert::utf8_to_utf16(const std::string& string) {
	if (string.empty())
		return std::wstring();

	int result_size = MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.size(), NULL, 0);
	std::wstring result(result_size, 0);
	MultiByteToWideChar(CP_UTF8, 0, &string[0], (int)string.size(), &result[0], result_size);

	return result;
}

std::wstring convert::to_wstring(const std::string& string) {
	return std::wstring(string.begin(), string.end());
}