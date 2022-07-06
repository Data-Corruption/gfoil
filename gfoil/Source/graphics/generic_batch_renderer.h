#pragma once

#include <array>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "buffer_array_object.h"
#include "buffer.h"

namespace gfoil {

	const enum class primative_type {
		POINTS = 0,
		LINES = 1,
		LINE_LOOP = 2,
		LINE_STRIP = 3,
		TRIANGLES = 4,
		TRIANGLE_STRIP = 5,
		TRIANGLE_FAN = 6,
	};

	const enum class vertex_type {
		TEXTURE = 0,
		TINTED = 1,
		COLOR = 2,
	};

	// third is texture float is alpha
	struct texture_vertex {
		glm::vec3 position;
		glm::vec3 texture_coords;
	};
	// third is texture float is alpha
	struct tint_vertex {
		glm::vec3 position;
		glm::vec3 texture_coords;
		glm::vec3 color;
	};
	// fourth is color float is alpha
	struct color_vertex {
		glm::vec3 position;
		glm::vec4 color;
	};

	class generic_batch_renderer {
	public:

		// ----==== Members ====----

		primative_type primative_type;
		vertex_type vertex_type;

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
			gfoil::primative_type primative_type, 
			gfoil::vertex_type vertex_type, 
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
		void buffer(std::vector<texture_vertex>& vertices);
		void buffer(std::vector<tint_vertex>& vertices);
		void buffer(std::vector<color_vertex>& vertices);

	private:

		std::vector<texture_vertex> data_texture;
		std::vector<tint_vertex> data_tint;
		std::vector<color_vertex> data_color;

		buffer_array_object bao;
		gfoil::buffer vertex_buffer;
		unsigned int index_buffer_id;

	};
};