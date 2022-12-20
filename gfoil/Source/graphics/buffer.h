#pragma once

#include <vector>

#include <glad/glad.h> 

namespace gfoil {
	class buffer {
	public:

		const enum class types {
			VERTEX = 0x8892,
			INDEX = 0x8893
		};

		const enum class draw_types {
			STATIC = 0x88E4,
			DYNAMIC = 0x88E8,
			STREAM = 0x88E0
		};

		const enum class data_types {
			BYTE = 0x1400,
			UBYTE = 0x1401,
			SHORT = 0x1402,
			USHORT = 0x1403,
			INT = 0x1404,
			UINT = 0x1405,
			HALF_FLOAT = 0x140B,
			FLOAT = 0x1406,
			DOUBLE = 0x140A
		};

		// ----==== Members ====----

		static unsigned int bound_buffer_ID;

		types type;

		unsigned int id = 0;
		unsigned int last_tail_in_bytes = 0;

		// ----==== Methods ====----

		// also binds the buffer
		void generate(enum types type);
		void destroy();

		// needs to be bound before doing anything except generating or destroying
		void bind();

		/// if (start_of_data == nullptr) this will just allocate a memory and set the draw_type
		/// Draw Types:
		///	 Static = write one time, draw many times
		///	 Dynamic = write and draw many times
		///	 Stream = write and draw once
		/// Max size of data for indexed data is 65k due to unsigned short range
		void buffer_data(unsigned int size_of_data_in_bytes, void* start_of_data, draw_types draw_type);
		// use buffer_data() with nullptr to allocate enough memory before calling this
		void buffer_sub_data(unsigned int offset_into_buffer, unsigned int size_of_data_in_bytes, void* start_of_data);

		void enable_vertex_attributes(std::vector<unsigned int> positions);
		void disable_vertex_attributes(std::vector<unsigned int> positions);

		/// Example, how to set attributes for a simple vertex struct with 2 members
		/// set_vertex_attribute(0, 3, FLOAT, false, sizeof(vertex), (const void*)offsetof(vertex, position)); for a vec3
		/// set_vertex_attribute(1, 4, FLOAT, false, sizeof(vertex), (const void*)offsetof(vertex, color));    for a vec4
		/// If type is an integer and normalie is true, values normalize to [-1,1](signed) or [0,1](unsigned)
		/// If type is an integer and normalie is false, values will be directly converted to floats
		void set_vertex_attribute(unsigned int position, unsigned int size, data_types type, bool normalize, int stride, const void* component_offset);
		/// Same as set_vertex_attribute() but only accepts integers and will not convert them. 
		void set_int_vertex_attribute(unsigned int position, unsigned int size, data_types type, int stride, const void* component_offset);

	};
}