#pragma once

#include <unordered_map>

#include <windows.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "convert.h"
#include "config.h"

#include "system/system.h"
#include "system/device_input.h"

#include "graphics/window.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/buffer_array_object.h"
#include "graphics/buffer.h"
#include "graphics/generic_index_buffers.h"
#include "graphics/generic_batch_renderer.h"
#include "graphics/font.h"

#include "text.h"

// Exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN

class gfoil {
public:

	const enum class blend_factors {
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

	const enum class depth_test_functions {
		NEVER = 0x0200,
		LESS = 0x0201,
		EQUAL = 0x0202,
		LEQUAL = 0x0203,
		GREATER = 0x0204,
		NOTEQUAL = 0x0205,
		GEQUAL = 0x0206,
		ALWAYS = 0x0207,
	};

private:

	struct gfoil_data {
		blend_factors blend_s_factor;
		blend_factors blend_d_factor;
		depth_test_functions depth_function;

		bool culling;
		bool blending;
		bool depth_testing;

		bool initalized = false;
	};

public:

	static gfoil_data data;

	static void pre_window_init();
	static void post_window_init();
	static void shutdown();

	static void enable_blending();
	static void disable_blending();
	static void set_blend_function(blend_factors s, blend_factors d);

	static void enable_depth_test();
	static void disable_depth_test();
	static void set_depth_test_function(depth_test_functions function);

	static double get_time();

	static glm::mat4 ortho
	(
		const float& left,
		const float& right,
		const float& bottom,
		const float& top,
		const float& zNear,
		const float& zFar
	);

};