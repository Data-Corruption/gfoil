#include "generic_2d_camera.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "../../gmath.h"
#include "../window.h"
#include "../../system/input.h"

void gfoil::generic_2d_camera::generate(glm::vec2 starting_position, float starting_zoom) {
	position = starting_position;
	zoom = starting_zoom;
}
void gfoil::generic_2d_camera::destroy() {

}

void gfoil::generic_2d_camera::update() {

	// position
	bool w = input::keys[0x57], a = input::keys[0x41], s = input::keys[0x53], d = input::keys[0x44], x_change = false, y_change = false;

	if (a != d) { x_change = true; }
	if (w != s) { y_change = true; }

	if (w) { velocity.y += velocity_change_per_tick; }
	if (a) { velocity.x -= velocity_change_per_tick; }
	if (s) { velocity.y -= velocity_change_per_tick; }
	if (d) { velocity.x += velocity_change_per_tick; }

	float max_v = max_velocity;
	if (input::keys[VK_LSHIFT])
		max_v *= 2.0f;

	velocity.x = std::clamp(velocity.x, -max_v, max_v);
	velocity.y = std::clamp(velocity.y, -max_v, max_v);

	position.x += velocity.x;
	position.y += velocity.y;

	if ((velocity.x != 0.0f) && !x_change) { lower_velocity(velocity.x, velocity_change_per_tick); }
	if ((velocity.y != 0.0f) && !y_change) { lower_velocity(velocity.y, velocity_change_per_tick); }

	glm::vec2 window_half = glm::vec2((float)window::size.x / 2, (float)window::size.y / 2);

	// mask
	mask[0] = glm::vec2(position.x - window_half.x, position.y - window_half.y);    // bl
	mask[1] = glm::vec2(position.x + window_half.x, position.y - window_half.y);   // br
	mask[2] = glm::vec2(position.x + window_half.x, position.y + window_half.y);  // tr
	mask[3] = glm::vec2(position.x - window_half.x, position.y + window_half.y); // tl

	// view
	view = glm::mat4(1.0f);
	// projection
	projection = math::ortho(-window_half.x, window_half.x, -window_half.y, window_half.y, -1.0f, 1.0f);

}

void gfoil::generic_2d_camera::lower_velocity(float& _velocity, float _change) {
	bool is_negative;

	if (_velocity < 0.0f) {
		is_negative = true;
		_velocity += _change;
	} else {
		is_negative = false;
		_velocity -= _change;
	}

	if (is_negative != (_velocity < 0.0f))
		_velocity = 0.0f;
}