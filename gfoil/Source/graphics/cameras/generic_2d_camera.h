#pragma once

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace gfoil {

	class generic_2d_camera {
	public:

		// ----==== Members ====----

		float velocity_change_per_tick = 0.016f;
		float max_velocity = 0.12f;
		float zoom_change_per_scroll = 1.0f;

		float zoom = 1.0f;

		glm::vec2 position, velocity;
		glm::mat4 projection;

		// ----==== Methods ====----

		void generate(glm::vec2 starting_position, float zoom);
		void destroy();

		void update();

	private:

		void lower_velocity(float& _velocity, float _change);

	};

}