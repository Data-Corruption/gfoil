#pragma once

#include <glad/glad.h>

namespace gfoil {

	namespace culling {

		const enum class targets {
			BACK = 0x0405,
			FRONT = 0x0404,
			BOTH = 0x0408,
		};

		// ----==== Members ====----

		extern bool is_enabled;
		extern targets target;

		// ----==== Methods ====----

		extern void enable();
		extern void disable();

		extern void set_target(targets t);

	}

}