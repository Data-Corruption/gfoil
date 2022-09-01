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
			unsigned int max_cubes_per_batch;

			generic_batch_renderer renderer;

			/// low_memory_mode: true - 1.8k cubes per batch, false - 10k cubes per batch
			void generate(vertex::type target_vertex_type);
			void generate(unsigned int max_cubes_per_batch, vertex::type target_vertex_type, unsigned int index_buffer_id);
			void destroy() { this->renderer.destroy(); };

			// sends buffer to gpu
			void flush() { this->renderer.flush(); };
			// draws most recent flush
			void draw() { this->renderer.draw(); };

			// add to buffer
			void buffer_data(std::vector<cube::color>& cubes);
			void buffer_data(std::vector<cube::texture>& cubes);
			void buffer_data(std::vector<cube::tint>& cubes);
		};

	};

}