#include "keyboard.h"

#include <Windows.h>

std::array<int, 255> gfoil::keyboard::keys;

void gfoil::keyboard::update() {

	for (int code = 1; code < 255; code++) {
		if (GetAsyncKeyState(code) & 0x8000) {
			keys[code]++;
		} else {
			keys[code] = 0;
		}
	}

}