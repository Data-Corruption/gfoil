#include "generic_index_buffers.h"

buffer gfoil::generic_index_buffers::quad_10k;

void gfoil::generic_index_buffers::generate_index_buffers() {

	// generate quad 8k
	{
		uint16_t max_indices = 10000 * 6;
		std::vector<uint16_t> indices;
		indices.resize(max_indices);

		uint16_t offset = 0;
		for (int i = 0; i < max_indices; i += 6) {
			indices[i + 0] = 0 + offset; // bl
			indices[i + 1] = 1 + offset; // br
			indices[i + 2] = 2 + offset; // tr
			indices[i + 3] = 2 + offset; // tr
			indices[i + 4] = 3 + offset; // tl
			indices[i + 5] = 0 + offset; // bl
			offset += 4;
		}

		generic_index_buffers::quad_10k.generate(buffer::types::INDEX);
		generic_index_buffers::quad_10k.buffer_data(max_indices * sizeof(uint16_t), &indices[0], buffer::draw_types::STATIC);
	}

}