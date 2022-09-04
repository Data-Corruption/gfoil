#pragma once

#include <unordered_map>

#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "convert.h"
#include "config.h"

#include "system/system.h"
#include "system/input.h"

#include "graphics/small_types/ray.h"
#include "graphics/small_types/plane.h"
#include "graphics/small_types/vertex.h"

#include "math.h"

#include "graphics/culling.h"
#include "graphics/depth_test.h"
#include "graphics/blending.h"

#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/buffer_array_object.h"
#include "graphics/buffer.h"
#include "graphics/generic_index_buffers.h"
#include "graphics/generic_batch_renderer.h"
#include "graphics/shapes/quad.h"
#include "graphics/shapes/cube.h"
#include "graphics/font.h"

#include "text.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

namespace gfoil {

	extern void init(
		std::string title,
		std::string icon_path,
		glm::ivec2 size,
		glm::ivec2 position,
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

	extern double get_time();

}