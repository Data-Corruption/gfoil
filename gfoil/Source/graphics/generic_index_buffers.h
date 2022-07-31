#pragma once

#include "buffer.h"

namespace gfoil {

	struct generic_index_buffers {

		// bl, br, tr, tr, tl, bl
		static buffer quad_10k;

		static void generate_index_buffers();

	};

}