#include "gfoil.h"

#include "system/system.h"
#include "config.h"

#include "graphics/window.h"
#include "graphics/texture.h"
#include "graphics/generic_index_buffers.h"

#include "text.h"

gfoil::gfoil_data gfoil::data;

void gfoil::pre_window_init() {
	// config
	config::load_config();

	// logging
	system::log::log_path = "app/log.txt";
	system::log::current_logging_level = system::log::levels::I_NEED_AN_ADULT;
}
void gfoil::post_window_init() {
	// other
	texture::bound_textures.fill(0);

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
	data.depth_testing = true;
	data.depth_function = depth_test_functions::LESS;

	glDisable(GL_BLEND);
	data.blending = false;
	data.blend_s_factor = blend_factors::ONE;
	data.blend_d_factor = blend_factors::ZERO;
}

void gfoil::shutdown() {
	if (window::flag_glfw_initialized())
		glfwTerminate();
}

void gfoil::enable_blending() {
	if (data.blending)
		return;
	glEnable(GL_BLEND);
	data.blending = true;
}
void gfoil::disable_blending() {
	if (!data.blending)
		return;
	glDisable(GL_BLEND); 
	data.blending = false;
}
void gfoil::set_blend_function(blend_factors s, blend_factors d) {
	if ((data.blend_s_factor == s) && (data.blend_d_factor == d))
		return;
	glBlendFunc((GLenum)s, (GLenum)d); 
	data.blend_s_factor = s;
	data.blend_d_factor = d;
}

void gfoil::enable_depth_test() {
	if (data.depth_testing)
		return;
	glEnable(GL_DEPTH_TEST); 
	data.depth_testing = true;
}
void gfoil::disable_depth_test() {
	if (!data.depth_testing)
		return;
	glDisable(GL_DEPTH_TEST);
	data.depth_testing = false;
}
void gfoil::set_depth_test_function(depth_test_functions function) {
	if (data.depth_function == function)
		return;
	glDepthFunc((GLenum)function); 
	data.depth_function = function;
}

double gfoil::get_time() { return glfwGetTime(); }

glm::mat4 gfoil::ortho
(
	const float& left,
	const float& right,
	const float& bottom,
	const float& top,
	const float& zNear,
	const float& zFar
)
{
	glm::mat4 result = glm::mat4(1.0f);
	result[0][0] = float(2) / (right - left);
	result[1][1] = float(2) / (top - bottom);
	result[2][2] = -float(2) / (zFar - zNear);
	result[3][0] = -(right + left) / (right - left);
	result[3][1] = -(top + bottom) / (top - bottom);
	result[3][2] = -(zFar + zNear) / (zFar - zNear);
	return result;
}