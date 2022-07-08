#pragma once

#include <glm/glm.hpp>

struct vertex {

	const enum class type {
		TEXTURE = 0,
		TINTED = 1,
		COLOR = 2,
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
	// fourth is color float is alpha
	struct color {
		glm::vec3 position;
		glm::vec4 color;
	};

};