#pragma once

#include <string>
#include <vector>

#include <stringapiset.h>

class convert {
public:

	static std::string utf16_to_utf8(const std::wstring& wstring);
	static std::wstring utf8_to_utf16(const std::string& string);

	static std::wstring to_wstring(const std::string& string);

};