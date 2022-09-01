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

		// ----==== Members ====----

		primative_type primative_type;
		vertex::type vertex_type;

		unsigned int current_batch_size;
		unsigned int last_batch_size = 0;
		unsigned int max_batch_size;

		unsigned int vertex_group_size;
		unsigned int indices_per_group;

		// ----==== Methods ====----

		/// Count is max number of vertices for each batch, if using an index buffer it must be a multiple of vertex group size.
		/// Index stuff is ignored if index_buffer_id is 0
		void generate(
			unsigned int count,
			enum primative_type primative,
			enum vertex::type vertex,
			unsigned int index_buffer_id,
			unsigned int vertex_group_size,
			unsigned int indices_per_group
		);
		void destroy();

		// sends buffer to gpu
		void flush();

		// draws most recent flush
		void draw();

		// add to buffer
		void buffer_data(vertex::color* start_of_vertices, unsigned int count); // count = number of vertices not bytes
		void buffer_data(vertex::texture* start_of_vertices, unsigned int count); // count = number of vertices not bytes
		void buffer_data(vertex::tint* start_of_vertices, unsigned int count); // count = number of vertices not bytes

	private:

		std::vector<vertex::color> data_color;
		std::vector<vertex::texture> data_texture;
		std::vector<vertex::tint> data_tint;

		buffer_array_object bao;
		buffer vertex_buffer;
		unsigned int index_buffer_id;

	};
}