#pragma once

#include "../small_types/ray.h"
#include "../small_types/plane.h"

#include "../window.h"

namespace gfoil {

	class generic_3d_camera {
	public:

		// ----==== Members ====----

		plane forward_plane;
		ray   forward_ray;

		bool first_mouse = true;

		float rotate_speed = 0.04f;
		float velocity_change_per_tick = 0.016f;

		float far_plane = 400.0f;
		float near_plane = 0.1f;
		float field_of_view = 60;

		glm::vec3 position, rotation, velocity, front, up, right;
		glm::vec2 mouse_offset, front_flat;
		glm::dvec2 last_mouse_position;

		glm::mat4 view, projection;

		// ----==== Methods ====----

		void generate(glm::vec3 starting_position);
		void destroy();

		void update();

	private:

		void update_position();
		void update_rotation();

		void lower_velocity(float& _velocity, float _change);

	};

}