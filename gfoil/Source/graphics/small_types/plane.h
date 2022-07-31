#pragma once

#include <glm/glm.hpp>

struct plane {
	glm::vec3 normal = { 0.f, 1.f, 0.f };
	float distance = 0.0f;

	void calculate(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
		this->normal = glm::vec3(glm::normalize(glm::cross(b - a, c - a)));
		this->distance = glm::dot(normal, a);
	}
};