#pragma once

#include <array>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "buffer_array_object.h"
#include "buffer.h"

#include "small_types/vertex.h"

namespace gfoil {
	class generic_batch_renderer {
	public:

		const enum class primative_type {
			POINTS = 0,
			LINES = 1,
			LINE_LOOP = 2,
			LINE_STRIP = 3,
			TRIANGLES = 4,
			TRIANGLE_STRIP = 5,
			TRIANGLE_FAN = 6,
		};

		// ----==== Members ====----

		primative_type primative_type;
		vertex::type vertex_type;

		unsigned int current_batch_size;
		unsigned int last_batch_size = 0;
		unsigned int max_batch_size;

		unsigned int vertex_group_size;
		unsigned int indices_per_group;

		bool allow_overflow;

		// ----==== Methods ====----

		/// Count is max number of vertices for each batch, if using an index buffer it must be a multiple of vertex group size.
		/// Index stuff is ignored if index_buffer_id is 0
		void generate(
			unsigned int count,
			enum primative_type primative,
			enum vertex::type vertex,
			unsigned int index_buffer_id,
			unsigned int vertex_group_size,
			unsigned int indices_per_group,
			bool allow_overflow
		);
		void destroy();

		// sends buffer to gpu
		void flush();

		// draws most recent flush
		void draw();

		// add to buffer
		void buffer_data(std::vector<vertex::texture>& vertices);
		void buffer_data(std::vector<vertex::tint>& vertices);
		void buffer_data(std::vector<vertex::color>& vertices);

	private:

		std::vector<vertex::texture> data_texture;
		std::vector<vertex::tint> data_tint;
		std::vector<vertex::color> data_color;

		buffer_array_object bao;
		buffer vertex_buffer;
		unsigned int index_buffer_id;

	};
}