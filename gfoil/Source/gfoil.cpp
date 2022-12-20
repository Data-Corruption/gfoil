#include "gfoil.hpp"

#include <mutex>

glm::dvec2 gfoil::cursor::position;
glm::dvec2 gfoil::cursor::window_position;
bool gfoil::cursor::enabled = true;

void gfoil::init(
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
	bool transparent_buffer) 
{
	// config
	config::load();

	// create window
	window::generate(
		title,
		icon_path,
		size,
		position,
		clear_color,
		samples,
		center_window,
		resizable,
		decorated,
		minimize_on_focus_loss,
		floating,
		maximized,
		center_cursor,
		transparent_buffer
	);
	
	// generate index buffers
	generic_index_buffers::generate_index_buffers();

	// opengl
	if (samples) {
		glEnable(GL_MULTISAMPLE);
	}else {
		glDisable(GL_MULTISAMPLE);
	}

	// culling
	glEnable(GL_CULL_FACE);
	culling::is_enabled = true;
	culling::target = culling::targets::BACK;

	// depth testing
	glEnable(GL_DEPTH_TEST);
	depth_test::is_enabled = true;
	depth_test::function = depth_test::functions::LESS;

	// blending
	glDisable(GL_BLEND);
	blending::is_enabled = false;
	blending::s_factor = blending::factors::ONE;
	blending::d_factor = blending::factors::ZERO;

}

void gfoil::shutdown() {
	glfwTerminate();
}

double gfoil::get_time() { return glfwGetTime(); }

void gfoil::cursor::enable_cursor() { glfwSetInputMode(window::handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
void gfoil::cursor::disable_cursor() { glfwSetInputMode(window::handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }