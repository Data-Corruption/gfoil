#pragma once

#include <vector>
#include <array>

#include "../small_types/vertex.h"
#include "../generic_batch_renderer.h"

#include "../../gmath.h"
#include "../cameras/generic_2d_camera.h"
#include "../cameras/generic_3d_camera.h"

namespace gfoil {

	/// Order - bl, br, tr, tl
	/// Draw order - { bl, br, tr }, { tr, tl, bl }
	struct quad {

		struct color {
			std::array<vertex::color, 4> vertices;

			bool visible_by_camera(generic_2d_camera& camera);
			bool visible_by_camera(generic_3d_camera& camera);
			bool hit_by_ray(ray& ray, float* distance);
		};

		struct texture {
			std::array<vertex::texture, 4> vertices;

			bool visible_by_camera(generic_2d_camera& camera);
			bool visible_by_camera(generic_3d_camera& camera);
			bool hit_by_ray(ray& ray, float* distance);
		};

		struct tint {
			std::array<vertex::tint, 4> vertices;

			bool visible_by_camera(generic_2d_camera& camera);
			bool visible_by_camera(generic_3d_camera& camera);
			bool hit_by_ray(ray& ray, float* distance);

		};

		class batch_renderer {
		public:
			vertex::type vertex_type;

			void generate(vertex::type target_vertex_type);

			void destroy();

			// sends buffer to gpu
			void flush();

			// draws most recent flush
			void draw();

			// add to buffer
			void buffer_data(std::vector<quad::texture>& vertices);
			void buffer_data(std::vector<quad::tint>& vertices);
			void buffer_data(std::vector<quad::color>& vertices);

		private:

			generic_batch_renderer renderer;

		};

	};

}