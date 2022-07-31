#pragma once

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
			const float& zFar);

		extern bool ray_triangle_intersect(ray& ray, glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::dvec3* result);

	}

}