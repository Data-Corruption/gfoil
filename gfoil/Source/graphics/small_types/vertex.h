#pragma once

#include <glm/glm.hpp>

namespace gfoil {

	struct vertex {

		const enum class type {
			COLOR = 0,
			TEXTURE = 1,
			TINTED = 2,
		};

		// fourth is color float is alpha
		struct color {
			glm::vec3 position;
			glm::vec4 color;
		};
		// third is texture float is alpha
		struct texture {
			glm::vec3 position;
			glm::vec3 texture_coords;
		};
		// third is texture float is alpha
		struct tint {
			glm::vec3 position;
			glm::vec3 texture_coords;
			glm::vec3 color;
		};

	};

}