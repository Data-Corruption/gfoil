#pragma once

#include <array>

#include <glm/glm.hpp>

class device_input {
public:

	/// First = code https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	/// Second = ticks held down
	static std::array<int, 255> keys;
	static glm::ivec2 mouse_pos;

	static void update();

};