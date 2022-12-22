#include "window.h"

#include "../system.hpp"
#include "../config.hpp"

#include "stb_image.h"

#include "../gfoil.hpp"

// public members

GLFWwindow* gfoil::window::handle;

std::string gfoil::window::title;
std::string gfoil::window::icon_path;

glm::ivec2 gfoil::window::position;
glm::ivec2 gfoil::window::size;

glm::vec4 gfoil::window::clear_color;

// private members

std::string gfoil::window::last_title;
glm::ivec2 gfoil::window::last_position;
glm::ivec2 gfoil::window::last_size;

bool gfoil::window::focused = true;
bool gfoil::window::resized;
bool gfoil::window::maximized;
bool gfoil::window::minimized;

bool gfoil::window::asking_to_close = false;
bool gfoil::window::close_confirm = false;
bool gfoil::window::close_cancel = false;

bool gfoil::window::glfw_initialized = false;
bool gfoil::window::glad_initialized = false;

// methods

void gfoil::window::generate(
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
) {

	window::clear_color = clear_color;
	window::position = position;
	window::size = size;
	window::title = title;

	window::last_position = position;
	window::last_size = size;
	window::last_title = title;

	window::maximized = maximized;
	window::minimized = false;

	if (!glfw_initialized)
		init_glfw();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // no old opengl
	glfwWindowHint(GLFW_VISIBLE, false);

	// --- hints ---

	glfwWindowHint(GLFW_SAMPLES, samples);
	glfwWindowHint(GLFW_RESIZABLE, resizable);
	glfwWindowHint(GLFW_DECORATED, decorated);
	glfwWindowHint(GLFW_AUTO_ICONIFY, minimize_on_focus_loss);
	glfwWindowHint(GLFW_FLOATING, floating);
	glfwWindowHint(GLFW_MAXIMIZED, maximized);
	glfwWindowHint(GLFW_CENTER_CURSOR, center_cursor);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, transparent_buffer);

	// --- create the window ---

	handle = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
	if (!handle) { 
		glfwTerminate(); 
		system::log::error("GLFW: failed to create window!"); 
	}
	glfwMakeContextCurrent(handle);

	if (!glad_initialized)
		init_glad();

	set_icon(icon_path);
	register_callbacks();
	glfwSwapInterval(-0);

	glViewport(0, 0, size.x, size.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

	// set position
	if (center_window) {
		int monitorX, monitorY;
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);
		glfwSetWindowPos(handle, monitorX + (mode->width - size.x) / 2, monitorY + (mode->height - size.y) / 2);
	} else {
		glfwSetWindowPos(handle, position.x, position.y);
	}

	glfwShowWindow(handle);

	system::log::info("GLFW: successfully created window!");

}

void gfoil::window::destroy() {
	glfwDestroyWindow(handle);
}

void gfoil::window::poll_events() {

	if (asking_to_close) {
		if (close_confirm)
			glfwSetWindowShouldClose(handle, GLFW_TRUE);
		if (close_cancel) {
			asking_to_close = false;
			close_cancel = false;
			glfwSetWindowShouldClose(handle, GLFW_FALSE);
		}
	}

	// react to window variable modifications
	if (last_position != position)
		glfwSetWindowPos(handle, position.x, position.y);
	if (last_title != title)
		glfwSetWindowTitle(handle, title.c_str());
	resized = false;
	if (last_size != size) {
		glfwSetWindowSize(handle, size.x, size.y);
		glViewport(0, 0, size.x, size.y);
		resized = true;
	}

	// get cursor screen coords
	POINT p;
	if (GetCursorPos(&p)) {
		cursor::position = glm::dvec2((double)p.x, (double)p.y);
	} else {
		system::log::warn("Failed to get mouse screen coords");
	}
	// get cursor window coords
	if (window::handle)
		glfwGetCursorPos(window::handle, &cursor::window_position.x, &cursor::window_position.y);
	

	glfwPollEvents();

	last_position = position;
	last_size = size;
	last_title = title;

}
void gfoil::window::swap_buffers() {
	glfwSwapBuffers(handle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gfoil::window::set_icon(std::string path) {
	GLFWimage image;
	image.pixels = stbi_load(path.c_str(), &image.width, &image.height, 0, 4);
	glfwSetWindowIcon(handle, 1, &image);
	stbi_image_free(image.pixels);
}

void gfoil::window::toggle_maximization() {
	maximized = !maximized;
	if (!minimized && maximized)
		glfwMaximizeWindow(handle);
	if (!minimized && !maximized)
		glfwRestoreWindow(handle);
}
void gfoil::window::toggle_minimization() {
	minimized = !minimized;
	if (minimized)
		glfwIconifyWindow(handle);
	if (!minimized && maximized)
		glfwMaximizeWindow(handle);
	if (!minimized && !maximized)
		glfwRestoreWindow(handle);
}

bool gfoil::window::is_open() { return !glfwWindowShouldClose(handle); }
bool gfoil::window::is_focused() { return focused; }
bool gfoil::window::is_resized() { return resized; }
bool gfoil::window::is_minimized() { return minimized; }
bool gfoil::window::is_maximized() { return maximized; }
bool gfoil::window::is_asking_to_close()  { return asking_to_close; }

void gfoil::window::close_response(bool value) {
	if (value) {
		close_confirm = true;
	} else {
		close_cancel = true;
	}
}

void gfoil::window::init_glfw() {
	glfwSetErrorCallback([](int error, const char* description) {
		system::log::error("GLFW: " + static_cast<std::string>(description));
	});
	if (!glfwInit())
		system::log::error("GLFW: failed to initialize!");
	glfw_initialized = true;
}
void gfoil::window::init_glad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		system::log::error("GLAD: failed to initialize!");
	}
	glad_initialized = true;
}

void gfoil::window::register_callbacks() {
	glfwSetWindowPosCallback(handle, [](GLFWwindow* window, int xpos, int ypos) {
		position = glm::ivec2(xpos, ypos);
	});
	glfwSetFramebufferSizeCallback(handle, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
		size = glm::ivec2(width, height);
		resized = true;
	});
	glfwSetWindowMaximizeCallback(handle, [](GLFWwindow* window, int maximized) {
		window::maximized = false;
		if (maximized)
			window::maximized = true;
		resized = true;
	});
	glfwSetWindowIconifyCallback(handle, [](GLFWwindow* window, int minimized) {
		window::minimized = false;
		if (minimized)
			window::minimized = true;
	});
	glfwSetWindowCloseCallback(handle, [](GLFWwindow* window) {
		asking_to_close = true;
		glfwSetWindowShouldClose(window, GLFW_FALSE);
	});
	glfwSetWindowFocusCallback(handle, [](GLFWwindow* window, int focused) {
		window::focused = false;
		if (focused)
			window::focused = true;
	});
}