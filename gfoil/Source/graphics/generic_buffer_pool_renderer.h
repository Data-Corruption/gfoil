#pragma once

/*

#include <array>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "buffer_array_object.h"
#include "buffer.h"

#include "small_types/vertex.h"

namespace gfoil {
	class generic_buffer_pool_renderer {
	public:

		// ----==== Members ====----

		primative_type primative_type;
		vertex::type vertex_type;

		std::vector<buffer> buffer_pool;

		std::vector<int> commit_index;
		std::vector<int> commit_queue;

		// max verticies per draw
		unsigned int max_result_size;
		unsigned int vertex_group_size;
		unsigned int indices_per_group;

		// ----==== Methods ====----

		/// If using an index buffer it must be a multiple of vertex group size.
		/// Index stuff is ignored if index_buffer_id is 0.
		/// If you buffer data to a buffer out of range it will generate new buffers to meet that pool size.
		/// If you commit a buffer out of range it will throw an error.
		void generate(
			unsigned int max_verticies_per_draw,
			unsigned int max_verticies_per_buffer, 
			unsigned int starting_buffer_count, 
			enum primative_type primative,
			enum vertex::type vertex,
			unsigned int index_buffer_id,
			unsigned int vertex_group_size,
			unsigned int indices_per_group
		);
		void destroy();
		
		void buffer_data(int buffer_index, vertex::color* start_of_vertices, unsigned int count); // count = number of vertices not bytes
		void buffer_data(int buffer_index, vertex::texture* start_of_vertices, unsigned int count); // count = number of vertices not bytes
		void buffer_data(int buffer_index, vertex::tint* start_of_vertices, unsigned int count); // count = number of vertices not bytes

		void commit(int buffer_index);
		void draw();

	private:

		buffer_array_object bao;
		buffer commit_buffer;
		unsigned int index_buffer_id;

	};
}

*/