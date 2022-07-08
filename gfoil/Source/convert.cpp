#include "convert.h"

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

glm::vec3 convert::hex_color_to_vec3(const std::string& hex) {
	return glm::vec3(0.0, 0.0, 0.0);
}

std::string convert::vec3_color_to_hex_string(glm::vec3& color) {
	return "";
}