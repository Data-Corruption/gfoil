#pragma once

#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "string_addons.hpp"
#include "convert.hpp"
#include "config.hpp"
#include "system.hpp"

#include "graphics/small_types/ray.h"
#include "graphics/small_types/plane.h"
#include "graphics/small_types/vertex.h"

#include "gmath.h"

#include "graphics/culling.h"
#include "graphics/depth_test.h"
#include "graphics/blending.h"

#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/buffer.h"
#include "graphics/buffer_array_object.h"
#include "graphics/generic_index_buffers.h"
#include "graphics/generic_batch_renderer.h"
#include "graphics/shapes/quad.h"
#include "graphics/shapes/cube.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

namespace gfoil {

	extern void init(
		std::string title,
		std::string icon_path,
		glm::ivec2 size,
		glm::ivec2 position,
		glm::vec4 clear_color,
		int samples,
		bool center_window,
		bool resizable,
		bool decorated,
		bool minimize_on_focus_loss,
		bool floating,
		bool maximized,
		bool center_cursor,
		bool transparent_buffer
	);
	extern void shutdown();
	
	// return glfwGetTime()
	extern double get_time();

	class cursor {
	public:
		static glm::dvec2 position;
		static glm::dvec2 window_position;
		
		static bool enabled;
		
		static void enable_cursor();
		static void disable_cursor();
	};

}