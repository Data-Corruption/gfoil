#pragma once

#include <string>

#include <glm/glm.hpp>

#include <stringapiset.h>

class convert {
public:

	static std::string utf16_to_utf8(const std::wstring& wstring);

	static std::wstring utf8_to_utf16(const std::string& string);

	static glm::vec3 hex_color_to_vec3(const std::string& hex);

	static std::string vec3_color_to_hex_string(glm::vec3& color);

};