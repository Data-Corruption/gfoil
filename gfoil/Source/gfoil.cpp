#include "gfoil.h"

void gfoil::init(
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

	// input
	input::keys.fill(0);
	input::is_cursor_enabled = true;
	glfwSetScrollCallback(window::handle, [](GLFWwindow* window, double xoffset, double yoffset) {
		input::mouse::wheel_offset = glm::dvec2(xoffset, yoffset);
	});

	// generate index buffers
	generic_index_buffers::generate_index_buffers();

	// opengl
	glEnable(GL_MULTISAMPLE);

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