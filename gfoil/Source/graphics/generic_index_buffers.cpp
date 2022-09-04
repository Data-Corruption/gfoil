#include "generic_index_buffers.h"

#include "../config.h"

buffer gfoil::generic_index_buffers::quad;
buffer gfoil::generic_index_buffers::cube;

void gfoil::generic_index_buffers::generate_index_buffers() {

	// quad
	{
		uint16_t max_indices = 10000 * 6;
		std::vector<uint16_t> indices;
		indices.resize(max_indices);

		uint16_t offset = 0;
		for (uint16_t i = 0; i < max_indices; i += 6) {
			indices[i + 0] = 0 + offset; // bl
			indices[i + 1] = 1 + offset; // br
			indices[i + 2] = 2 + offset; // tr

			indices[i + 3] = 2 + offset; // tr
			indices[i + 4] = 3 + offset; // tl
			indices[i + 5] = 0 + offset; // bl
			offset += 4;
		}

		generic_index_buffers::quad.generate(buffer::types::INDEX);
		generic_index_buffers::quad.buffer_data((unsigned int)max_indices * sizeof(uint16_t), &indices[0], buffer::draw_types::STATIC);
	}

	// cube
	{
		uint16_t max_indices = 1800 * 36;
		std::vector<uint16_t> indices;
		indices.resize(max_indices);

		uint16_t offset = 0;
		for (uint16_t i = 0; i < max_indices; i += 36) {

			// up
			indices[i + 0]  = 2 + offset;
			indices[i + 1]  = 3 + offset;
			indices[i + 2]  = 7 + offset;

			indices[i + 3]  = 7 + offset;
			indices[i + 4]  = 6 + offset;
			indices[i + 5]  = 2 + offset;

			// front
			indices[i + 6]  = 0 + offset;
			indices[i + 7]  = 2 + offset;
			indices[i + 8]  = 6 + offset;

			indices[i + 9]  = 6 + offset;
			indices[i + 10] = 4 + offset;
			indices[i + 11] = 0 + offset;

			// left
			indices[i + 12] = 5 + offset;
			indices[i + 13] = 4 + offset;
			indices[i + 14] = 6 + offset;

			indices[i + 15] = 6 + offset;
			indices[i + 16] = 7 + offset;
			indices[i + 17] = 5 + offset;

			// back 
			indices[i + 18] = 1 + offset;
			indices[i + 19] = 5 + offset;
			indices[i + 20] = 7 + offset;

			indices[i + 21] = 7 + offset;
			indices[i + 22] = 3 + offset;
			indices[i + 23] = 1 + offset;

			// right
			indices[i + 24] = 0 + offset;
			indices[i + 25] = 1 + offset;
			indices[i + 26] = 3 + offset;

			indices[i + 27] = 3 + offset;
			indices[i + 28] = 2 + offset;
			indices[i + 29] = 0 + offset;

			// down
			indices[i + 30] = 0 + offset;
			indices[i + 31] = 4 + offset;
			indices[i + 32] = 5 + offset;

			indices[i + 33] = 5 + offset;
			indices[i + 34] = 1 + offset;
			indices[i + 35] = 0 + offset;

			offset += 8;
		}

		generic_index_buffers::cube.generate(buffer::types::INDEX);
		generic_index_buffers::cube.buffer_data((unsigned int)max_indices * sizeof(uint16_t), &indices[0], buffer::draw_types::STATIC);
	}

}