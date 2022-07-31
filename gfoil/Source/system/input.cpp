#include "input.h"

#include "system.h"
#include "../graphics/window.h"

// ----==== Members ====----

int gfoil::input::mouse::left = 0;
int gfoil::input::mouse::right = 0;
int gfoil::input::mouse::middle = 0;

glm::dvec2 gfoil::input::mouse::position;
glm::dvec2 gfoil::input::mouse::window_position;
glm::dvec2 gfoil::input::mouse::wheel_offset;

std::array<int, 255> gfoil::input::keys;

bool gfoil::input::is_cursor_enabled;

// ----==== Methods ====----

void gfoil::input::update() {

	for (int code = 1; code < 255; code++) {
		if (GetAsyncKeyState(code) & 0x8000) {
			keys[code]++;
		} else {
			keys[code] = 0;
		}
	}

	mouse::left = keys[VK_LBUTTON];
	mouse::right = keys[VK_RBUTTON];
	mouse::middle = keys[VK_MBUTTON];

	// get mouse screen coords
	POINT p;
	if (GetCursorPos(&p)) {
		mouse::position = glm::dvec2((double)p.x, (double)p.y);
	} else {
		system::log::warn("Failed to get mouse screen coords");
	}

	// get mouse window coords
	if (window::handle)
		glfwGetCursorPos(window::handle, &mouse::window_position.x, &mouse::window_position.y);

}

void gfoil::input::toggle_cursor() {
	if (window::handle) {
		is_cursor_enabled = !is_cursor_enabled;
		if (is_cursor_enabled) {
			glfwSetInputMode(window::handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		} else {
			glfwSetInputMode(window::handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}