#pragma once

#include <glad/glad.h>

namespace gfoil {
	// remember to destroy() on cleanup!
	class buffer_array_object {
	public:
		// ----==== Members ====----

		static unsigned int bound_buffer_array_object_ID;

		unsigned int ID = 0;

		// ----==== Methods ====----

		void generate();
		void destroy();

		void bind();
		void unbind();

	};
};