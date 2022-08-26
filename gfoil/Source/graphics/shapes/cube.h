#pragma once

#include <vector>
#include <array>

#include "../small_types/vertex.h"
#include "../generic_batch_renderer.h"

#include "../../gmath.h"
#include "../cameras/generic_3d_camera.h"

namespace gfoil {

	/// vertex increment order - zyx
	/// face hit order - up, front, left, back, right, down
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

		class batch_renderer {
		public:
			vertex::type vertex_type;

			// count is max number of cubes per batch
			void generate(unsigned int count, vertex::type target_vertex_type, unsigned int index_buffer_id);
			void destroy();

			// sends buffer to gpu
			void flush();
			// draws most recent flush
			void draw();

			// add to buffer
			void buffer_data(std::vector<cube::color>& cubes);
			void buffer_data(std::vector<cube::texture>& cubes);
			void buffer_data(std::vector<cube::tint>& cubes);

		private:
			generic_batch_renderer renderer;
		};

	};

}