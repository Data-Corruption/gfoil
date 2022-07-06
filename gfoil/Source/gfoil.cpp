#include "gfoil.h"

bool gfoil::init() {

	// config
	load_config();
	
	// logging
	system::log::log_path = "app/log.txt";
	system::log::current_logging_level = system::log::levels::I_NEED_AN_ADULT;

	// other
	texture::bound_textures.fill(0);
	texture::font_atlas_height = 48;

	// quad index buffer
	uint16_t max_indices = 1000 * 6;

	std::vector<uint16_t> indices;
	indices.resize(max_indices);

	uint16_t offset = 0;
	for (int i = 0; i < max_indices; i += 6) {
		indices[i + 0] = 0 + offset; // bl
		indices[i + 1] = 1 + offset; // br
		indices[i + 2] = 2 + offset; // tr
		indices[i + 3] = 2 + offset; // tr
		indices[i + 4] = 3 + offset; // tl
		indices[i + 5] = 0 + offset; // bl
		offset += 4;
	}

	generic_index_buffers::quad_1k.generate(buffer::types::INDEX);
	generic_index_buffers::quad_1k.buffer_data(max_indices * sizeof(uint16_t), &indices[0], buffer::draw_types::STATIC);

	// opengl
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);
	depth_testing = true;
	depth_function = depth_test_functions::LESS;

	glDisable(GL_BLEND);
	blending = false;
	blend_s_factor = blend_factors::ONE;
	blend_d_factor = blend_factors::ZERO;

}

void gfoil::shutdown() {
	if (window::flag_glfw_initialized())
		glfwTerminate();
}

void gfoil::enable_blending() { 
	if (blending)
		return;
	glEnable(GL_BLEND);
	blending = true;
}
void gfoil::disable_blending() { 
	if (!blending)
		return;
	glDisable(GL_BLEND); 
	blending = false;
}
void gfoil::set_blend_function(blend_factors s, blend_factors d) { 
	if ((blend_s_factor == s) && (blend_d_factor == d))
		return;
	glBlendFunc((GLenum)s, (GLenum)d); 
	blend_s_factor = s;
	blend_d_factor = d;
}

void gfoil::enable_depth_test() { 
	if (depth_testing)
		return;
	glEnable(GL_DEPTH_TEST); 
	depth_testing = true;
}
void gfoil::disable_depth_test() { 
	if (!depth_testing)
		return;
	glDisable(GL_DEPTH_TEST);
	depth_testing = false;
}
void gfoil::set_depth_test_function(depth_test_functions function) { 
	if (depth_function == function)
		return;
	glDepthFunc((GLenum)function); 
	depth_function = function;
}