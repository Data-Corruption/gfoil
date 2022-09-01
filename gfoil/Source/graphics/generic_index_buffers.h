#pragma once

#include "buffer.h"

namespace gfoil {

	struct generic_index_buffers {

		/// bl, br, tr, tr, tl, bl
		/// low_memory_mode: true - 10k, false - 40k
		static buffer quad;
		/// bl, br, tr, tr, tl, bl
		/// low_memory_mode: true - 1.8k, false - 10k
		static buffer cube;

		static void generate_index_buffers();

	};

}