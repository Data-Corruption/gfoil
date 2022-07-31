#pragma once

#include <vector>
#include <array>

#include "../../gmath.h"
#include "../cameras/generic_3d_camera.h"


namespace gfoil {

	struct cube {
		// increment order - zyx
		std::array<glm::vec3, 8> vertices;

		void generate(glm::ivec3 _position, glm::ivec3 _size);

		// ---- plane stuff ----
		bool in_front_of_camera(generic_3d_camera& camera);

		bool inline vertex_in_front_of_plane(plane& plane, glm::vec3& vertex);

		// ---- ray stuff ----
		bool hit_by_ray(ray& ray, int* side_hit, float* distance);

	};

}