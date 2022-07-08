#include "device_input.h"

#include <windows.h>

std::array<int, 255> device_input::keys;

void device_input::update() {

	for (int code = 1; code < 255; code++) {
		if (GetAsyncKeyState(code) & 0x8000) {
			keys[code]++;
		} else {
			keys[code] = 0;
		}
	}

	// get mouse coords

}