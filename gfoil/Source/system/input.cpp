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
bool gfoil::input::mouse::wheel_event = false;

std::array<int, 255> gfoil::input::keys;

bool gfoil::input::is_cursor_enabled;

// ----==== Methods ====----

char gfoil::input::key_code_to_visual_char(int target_character, bool is_capitalized) {
    if (is_capitalized) {
        switch (target_character) {
        case 0x20: return ' '; break;
        case 0x30: return ')'; break;
        case 0x31: return '!'; break;
        case 0x32: return '@'; break;
        case 0x33: return '#'; break;
        case 0x34: return '$'; break;
        case 0x35: return '%'; break;
        case 0x36: return '^'; break;
        case 0x37: return '&'; break;
        case 0x38: return '*'; break;
        case 0x39: return '('; break;
        case 0x41: return 'A'; break;
        case 0x42: return 'B'; break;
        case 0x43: return 'C'; break;
        case 0x44: return 'D'; break;
        case 0x45: return 'E'; break;
        case 0x46: return 'F'; break;
        case 0x47: return 'G'; break;
        case 0x48: return 'H'; break;
        case 0x49: return 'I'; break;
        case 0x4A: return 'J'; break;
        case 0x4B: return 'K'; break;
        case 0x4C: return 'L'; break;
        case 0x4D: return 'M'; break;
        case 0x4E: return 'N'; break;
        case 0x4F: return 'O'; break;
        case 0x50: return 'P'; break;
        case 0x51: return 'Q'; break;
        case 0x52: return 'R'; break;
        case 0x53: return 'S'; break;
        case 0x54: return 'T'; break;
        case 0x55: return 'U'; break;
        case 0x56: return 'V'; break;
        case 0x57: return 'W'; break;
        case 0x58: return 'X'; break;
        case 0x59: return 'Y'; break;
        case 0x5A: return 'Z'; break;
        case 0xBD: return '_'; break;
        case 0xBB: return '+'; break;
        case 0xDB: return '{'; break;
        case 0xDD: return '}'; break;
        case 0xDC: return '|'; break;
        case 0xBA: return ':'; break;
        case 0xDE: return '"'; break;
        case 0xBC: return '<'; break;
        case 0xBE: return '>'; break;
        case 0xBF: return '?'; break;
        case 0xC0: return '~'; break;
        default: return 0; break;
        }
    }
    else {
        switch (target_character) {
        case 0x20: return ' '; break;
        case 0x30: return '0'; break;
        case 0x31: return '1'; break;
        case 0x32: return '2'; break;
        case 0x33: return '3'; break;
        case 0x34: return '4'; break;
        case 0x35: return '5'; break;
        case 0x36: return '6'; break;
        case 0x37: return '7'; break;
        case 0x38: return '8'; break;
        case 0x39: return '9'; break;
        case 0x41: return 'a'; break;
        case 0x42: return 'b'; break;
        case 0x43: return 'c'; break;
        case 0x44: return 'd'; break;
        case 0x45: return 'e'; break;
        case 0x46: return 'f'; break;
        case 0x47: return 'g'; break;
        case 0x48: return 'h'; break;
        case 0x49: return 'i'; break;
        case 0x4A: return 'j'; break;
        case 0x4B: return 'k'; break;
        case 0x4C: return 'l'; break;
        case 0x4D: return 'm'; break;
        case 0x4E: return 'n'; break;
        case 0x4F: return 'o'; break;
        case 0x50: return 'p'; break;
        case 0x51: return 'q'; break;
        case 0x52: return 'r'; break;
        case 0x53: return 's'; break;
        case 0x54: return 't'; break;
        case 0x55: return 'u'; break;
        case 0x56: return 'v'; break;
        case 0x57: return 'w'; break;
        case 0x58: return 'x'; break;
        case 0x59: return 'y'; break;
        case 0x5A: return 'z'; break;
        case 0xBD: return '-'; break;
        case 0xBB: return '='; break;
        case 0xDB: return '['; break;
        case 0xDD: return ']'; break;
        case 0xDC: return '\\'; break;
        case 0xBA: return ';'; break;
        case 0xDE: return '\''; break;
        case 0xBC: return ','; break;
        case 0xBE: return '.'; break;
        case 0xBF: return '/'; break;
        case 0xC0: return '`'; break;
        default: return 0; break;
        }
    }
}

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

	mouse::wheel_event = false;

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