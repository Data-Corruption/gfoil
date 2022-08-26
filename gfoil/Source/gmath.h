#pragma once

#include <array>

#include <glm/glm.hpp>

#include "graphics/small_types/ray.h"
#include "graphics/small_types/plane.h"

namespace gfoil {

	namespace math {

		extern glm::mat4 ortho(
			const float& left,
			const float& right,
			const float& bottom,
			const float& top,
			const float& zNear,
			const float& zFar
		);

		extern bool vec3_in_front_of_plane(plane& plane, glm::vec3& vec3);

		extern bool vec2_in_rect(glm::vec2 point, glm::vec2& bl, glm::vec2& tr);

		extern bool ray_triangle_intersect(ray& ray, glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::dvec3* result);
		
		extern bool ray_rect_intersect(ray& ray, glm::vec3& bl, glm::vec3& br, glm::vec3& tr, glm::vec3& tl, float* distance);

	}

}