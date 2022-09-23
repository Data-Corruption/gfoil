#pragma once

#include <array>

#include <windows.h>

#include <glm/glm.hpp>

namespace gfoil {

	namespace input {

		struct mouse {

			static int left;
			static int right;
			static int middle;

			static glm::dvec2 position;
			static glm::dvec2 window_position;
			static glm::dvec2 wheel_offset;
			static bool wheel_event;

		};

		/// key = code https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		/// value = ticks held down
		extern std::array<int, 255> keys;

		// returns 0 if not found
		extern char key_code_to_visual_char(int target_character, bool is_capitalized);

		extern bool is_cursor_enabled;

		extern void update();

		extern void toggle_cursor();

	}

}