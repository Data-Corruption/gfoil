#include "window.h"

#include "../system/system.h"
#include "../utilities/config.h"

#include "stb_image.h"

gfoil::window::window_data gfoil::window::data;

void gfoil::window::init_glfw() {
	glfwSetErrorCallback([](int error, const char* description) {
		system::log::error("GLFW: " + static_cast<std::string>(description));
	});

	if (!glfwInit())
		system::log::error("GLFW: failed to initialize!");

	data.glfw_initialized = true;
}
void gfoil::window::init_glad() {
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		glfwTerminate();
		system::log::error("GLAD: failed to initialize!");
	}
	data.glad_initialized = true;
}

void gfoil::window::register_callbacks() {

	glfwSetCursorPosCallback(data.handle, [](GLFWwindow* window, double xpos, double ypos) {
		data.mouse_position = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
	});
	glfwSetWindowPosCallback(data.handle, [](GLFWwindow* window, int xpos, int ypos) {
		data.position = glm::ivec2(xpos, ypos);
	});
	glfwSetFramebufferSizeCallback(data.handle, [](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height); // maybe not needed
		data.size = glm::ivec2(width, height);
		data.resized = true;
	});
	glfwSetWindowMaximizeCallback(data.handle, [](GLFWwindow* window, int maximized) {
		if (maximized) {
			data.maximized = true;
		} else {
			data.maximized = false;
		}
		data.resized = true;
	});
	glfwSetWindowCloseCallback(data.handle, [](GLFWwindow* window) {
		data.asking_to_close = true;

		config["window_max"] = std::to_string(data.maximized);
		config["window_pos_x"] = std::to_string(data.position.x);
		config["window_pos_y"] = std::to_string(data.position.y);
		config["window_size_x"] = std::to_string(data.size.x);
		config["window_size_y"] = std::to_string(data.size.y);
		save_config();

		glfwSetWindowShouldClose(window, GLFW_FALSE);
	});

}

void gfoil::window::generate(
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
) {

	data.title = title;
	data.size = size;
	data.position = position;

	if (!data.glfw_initialized)
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

	data.handle = glfwCreateWindow(size.x, size.y, title.c_str(), NULL, NULL);
	if (!data.handle) { 
		glfwTerminate(); 
		system::log::error("GLFW: failed to create window!"); 
	}
	glfwMakeContextCurrent(data.handle);

	if (!data.glad_initialized)
		init_glad();

	set_icon(icon_path);
	register_callbacks();
	glfwSwapInterval(-0);

	glViewport(0, 0, size.x, size.y);
	glClearColor(data.clear_color.x, data.clear_color.y, data.clear_color.z, data.clear_color.w);

	// set position
	if (center_window) {
		int monitorX, monitorY;
		const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		glfwGetMonitorPos(glfwGetPrimaryMonitor(), &monitorX, &monitorY);
		set_position(glm::ivec2(monitorX + (mode->width - size.x) / 2, monitorY + (mode->height - size.y) / 2));
	} else {
		set_position(data.position);
	}

	glfwShowWindow(data.handle);

	system::log::info("GLFW: successfully created window!");

}

void gfoil::window::destroy() {
	glfwDestroyWindow(data.handle);
}

void gfoil::window::poll_events() {

	if (data.asking_to_close) {
		if (data.close_confirm) {
			glfwSetWindowShouldClose(data.handle, GLFW_TRUE);
		}
		else if (data.close_cancel) {
			data.asking_to_close = false;
			data.close_cancel = false;
			glfwSetWindowShouldClose(data.handle, GLFW_FALSE);
		}
	}

	data.resized = false;
	glfwPollEvents();
}

void gfoil::window::swap_buffers() {
	glfwSwapBuffers(data.handle);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// --- setters ---
void gfoil::window::set_title(std::string title) {
	data.title = title;
	glfwSetWindowTitle(data.handle, title.c_str());
}
void gfoil::window::set_position(glm::ivec2 position) {
	data.position = position;
	glfwSetWindowPos(data.handle, position.x, position.y);
}
void gfoil::window::set_icon(std::string path) {
	GLFWimage image;
	image.pixels = stbi_load(path.c_str(), &image.width, &image.height, 0, 4);
	glfwSetWindowIcon(data.handle, 1, &image);
	stbi_image_free(image.pixels);
}

// --- getters ---
GLFWwindow* gfoil::window::get_handle()  { return data.handle; }
std::string gfoil::window::get_title()   { return data.title; }
glm::ivec2 gfoil::window::get_position() { return data.position; }
glm::ivec2 gfoil::window::get_size()     { return data.size; }

// --- flags ---
bool gfoil::window::flag_resized()          { return data.resized; }
bool gfoil::window::flag_maximized()        { return data.maximized; }
bool gfoil::window::flag_minimized()        { return data.minimized; }
bool gfoil::window::flag_glfw_initialized() { return data.glfw_initialized; }
bool gfoil::window::flag_asking_to_close()  { return data.asking_to_close; }

// true = close, false = cancel close
void gfoil::window::close_response(bool value) {
	if (value) {
		data.close_confirm = true;
	} else {
		data.close_cancel = true;
	}
}