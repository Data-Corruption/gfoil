#include "generic_3d_camera.h"

#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "../../system/input.h"

const glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);

void gfoil::generic_3d_camera::generate(glm::vec3 starting_position) {

	last_mouse_position = glm::dvec2((double)window::size.x / 2.0f, (double)window::size.y / 2.0f);
	mouse_offset = glm::vec2(-90.0f, 0.0f);

	position = glm::vec3(0.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = world_up;

	update_rotation();

}
void gfoil::generic_3d_camera::destroy() {

}

void gfoil::generic_3d_camera::update() {

	if (input::is_cursor_enabled && !window::is_resized())
		return;

	update_position();

	if (input::mouse::window_position != last_mouse_position)
		update_rotation();

	forward_plane.calculate(glm::vec3(0) - right, right, up);
	forward_ray.direction = front;
	forward_ray.position = position;

	projection = glm::perspective(glm::radians(field_of_view), (float)window::size.x / (float)window::size.y, near_plane, far_plane);

}

void gfoil::generic_3d_camera::update_position() {
	bool x_change = false, y_change = false, z_change = false;

	if (input::keys[0x44]) { // d
		velocity.x += velocity_change_per_tick;
		x_change = true;
	}
	if (input::keys[0x41]) { // a
		velocity.x -= velocity_change_per_tick;
		if (x_change) {
			x_change = false;
		} else {
			x_change = true;
		}
	}

	if (input::keys[VK_SPACE]) { // space
		velocity.y += velocity_change_per_tick;
		y_change = true;
	}
	if (input::keys[VK_LCONTROL]) { // ctrl
		velocity.y -= velocity_change_per_tick;
		if (y_change) {
			y_change = false;
		} else {
			y_change = true;
		}
	}

	if (input::keys[0x57]) { // w
		velocity.z += velocity_change_per_tick;
		z_change = true;
	}
	if (input::keys[0x53]) { // s
		velocity.z -= velocity_change_per_tick;
		if (z_change) {
			z_change = false;
		} else {
			z_change = true;
		}
	}

	float max_velocity = 0.12f;
	if (input::keys[VK_LSHIFT])
		max_velocity *= 2.0f;

	if (x_change && y_change)
		max_velocity = max_velocity / 2;

	velocity.x = std::clamp(velocity.x, -max_velocity, max_velocity);
	velocity.y = std::clamp(velocity.y, -max_velocity, max_velocity);
	velocity.z = std::clamp(velocity.z, -max_velocity, max_velocity);

	position += velocity.x * right;                // left - right
	position += velocity.y * world_up;            // up - down
	position.x += velocity.z * front_flat.x;     // forward - back
	position.z += velocity.z * front_flat.y;    // forward - back

	if ((velocity.x != 0.0f) && !x_change) { lower_velocity(velocity.x, velocity_change_per_tick / 2); }
	if ((velocity.y != 0.0f) && !y_change) { lower_velocity(velocity.y, velocity_change_per_tick / 2); }
	if ((velocity.z != 0.0f) && !z_change) { lower_velocity(velocity.z, velocity_change_per_tick / 2); }

}
void gfoil::generic_3d_camera::update_rotation() {
	if (first_mouse) {
		last_mouse_position = input::mouse::window_position;
		first_mouse = false;
	} else if (last_mouse_position == input::mouse::window_position) {
		return;
	}

	mouse_offset = input::mouse::window_position - last_mouse_position;
	last_mouse_position = input::mouse::window_position;

	// prevent yaw from becoming a fat fucking whore
	rotation.x = glm::mod(rotation.x + (mouse_offset.x * rotate_speed), 360.0f);
	rotation.y = rotation.y - (mouse_offset.y * rotate_speed);
	rotation.y = std::clamp(rotation.y, -89.0f, 89.0f);

	// get front, front flat, right, and up
	front.x = cos(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	front.y = sin(glm::radians(rotation.y));
	front.z = sin(glm::radians(rotation.x)) * cos(glm::radians(rotation.y));
	glm::normalize(front);

	front_flat = { cos(glm::radians(rotation.x)), sin(glm::radians(rotation.x)) };
	glm::normalize(front_flat);

	right = glm::normalize(glm::cross(front, world_up));
	up = glm::normalize(glm::cross(front, right));

	view = glm::lookAt(glm::vec3(0.0f), front, world_up);
}

void gfoil::generic_3d_camera::lower_velocity(float& _velocity, float _change) {
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