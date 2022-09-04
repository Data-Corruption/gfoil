#pragma once

#include "buffer.h"

namespace gfoil {

	struct generic_index_buffers {

		/// bl, br, tr, tr, tl, bl 10k quads
		static buffer quad;
		/// bl, br, tr, tr, tl, bl 1.8k cubes
		static buffer cube;

		static void generate_index_buffers();

	};

}