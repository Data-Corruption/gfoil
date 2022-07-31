#pragma once

#include <glad/glad.h>

namespace gfoil {

	namespace blending {

		const enum class factors {
			ZERO = 0,
			ONE = 1,
			SRC_COLOR = 0x0300,
			ONE_MINUS_SRC_COLOR = 0x0301,
			SRC_ALPHA = 0x0302,
			ONE_MINUS_SRC_ALPHA = 0x0303,
			DST_ALPHA = 0x0304,
			ONE_MINUS_DST_ALPHA = 0x0305,
			DST_COLOR = 0x0306,
			ONE_MINUS_DST_COLOR = 0x0307,
			SRC_ALPHA_SATURATE = 0x0308,
			CONSTANT_COLOR = 0x8001,
			ONE_MINUS_CONSTANT_COLOR = 0x8002,
			CONSTANT_ALPHA = 0x8003,
			ONE_MINUS_CONSTANT_ALPHA = 0x8004,
		};

		// ----==== Members ====----

		extern bool is_enabled;
		extern factors s_factor;
		extern factors d_factor;

		// ----==== Methods ====----

		extern void enable();
		extern void disable();

		extern void set_function(factors s, factors d);

	}

}