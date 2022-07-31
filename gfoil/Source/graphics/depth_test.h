#pragma once

#include <glad/glad.h>

namespace gfoil {

	namespace depth_test {

		const enum class functions {
			NEVER = 0x0200,
			LESS = 0x0201,
			EQUAL = 0x0202,
			LEQUAL = 0x0203,
			GREATER = 0x0204,
			NOTEQUAL = 0x0205,
			GEQUAL = 0x0206,
			ALWAYS = 0x0207,
		};

		// ----==== Members ====----

		extern bool is_enabled;
		extern functions function;

		// ----==== Methods ====----

		extern void enable();
		extern void disable();

		extern void set_function(functions f);

	}

}