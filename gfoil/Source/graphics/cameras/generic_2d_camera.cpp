#include "generic_2d_camera.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "../window.h"
#include "../../system/input.h"

void gfoil::generic_2d_camera::generate(glm::vec2 starting_position, float zoom) {
	position = starting_position;
}
void gfoil::generic_2d_camera::destroy() {

}

void gfoil::generic_2d_camera::update() {

	bool w = input::keys[0x57], a = input::keys[0x41], s = input::keys[0x53], d = input::keys[0x44], x_change = false, y_change = false;

	if (a != d)
		x_change = true;
	if (w != s)
		y_change = true;

	if (w)
		velocity.y += velocity_change_per_tick; // w
	if (a)
		velocity.x -= velocity_change_per_tick; // a
	if (s)
		velocity.y -= velocity_change_per_tick; // s
	if (d)
		velocity.x += velocity_change_per_tick; // d

	float max_v = max_velocity;
	if (input::keys[VK_LSHIFT])
		max_v *= 2.0f;

	velocity.x = std::clamp(velocity.x, -max_v, max_v);
	velocity.y = std::clamp(velocity.y, -max_v, max_v);

	position.x += velocity.x;
	position.y += velocity.y;

	if ((velocity.x != 0.0f) && !x_change) { lower_velocity(velocity.x, velocity_change_per_tick); }
	if ((velocity.y != 0.0f) && !y_change) { lower_velocity(velocity.y, velocity_change_per_tick); }

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