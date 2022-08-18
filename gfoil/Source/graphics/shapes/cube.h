#pragma once

#include <vector>
#include <array>

#include "../small_types/vertex.h"

#include "../../gmath.h"
#include "../cameras/generic_3d_camera.h"

namespace gfoil {

	/// Vertex increment order - zyx
	/// Face hit order - up, front, left, back, right, down
	struct cube {

		struct color {
			std::array<vertex::color, 8> vertices;

			bool visible_by_camera(generic_3d_camera& camera);
			bool hit_by_ray(ray& ray, int* face_hit, float* distance);
		};

		struct texture {
			std::array<vertex::texture, 4> vertices;

			bool visible_by_camera(generic_3d_camera& camera);
			bool hit_by_ray(ray& ray, int* face_hit, float* distance);
		};

		struct tint {
			std::array<vertex::tint, 4> vertices;

			bool visible_by_camera(generic_3d_camera& camera);
			bool hit_by_ray(ray& ray, int* face_hit, float* distance);
		};

		// batch renderer

	};

}