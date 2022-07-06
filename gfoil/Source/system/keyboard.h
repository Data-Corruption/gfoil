#pragma once

#include <array>

namespace gfoil {
	class keyboard {
	public:

		/// First = code https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
		/// Second = ticks held down
		static std::array<int, 255> keys;

		static void update();

	};
}