#pragma once

#include <string>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace gfoil {

	class window {
	public:

		// ----==== Members ====----

		static GLFWwindow* handle;

		static std::string title;
		static std::string icon_path;

		static glm::ivec2 position;
		static glm::ivec2 size;

		static glm::vec4 clear_color;

		// ----==== Methods ====----

		// ignores position if center_window is true
		static void generate(
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
		static void destroy();

		static void poll_events();
		static void swap_buffers();

		static void set_icon(std::string path);

		// minification overides maximization
		static void toggle_maximization();
		// minification overides maximization
		static void toggle_minimization();

		static bool is_open();
		static bool is_focused();
		static bool is_resized();
		static bool is_minimized();
		static bool is_maximized();
		static bool is_cursor_enabled();
		static bool is_asking_to_close();
		// true = close, false = cancel close
		static void close_response(bool value);

	private:

		static std::string last_title;
		static glm::ivec2 last_position;
		static glm::ivec2 last_size;

		static bool focused;
		static bool resized;
		static bool maximized;
		static bool minimized;
		static bool cursor;

		static bool asking_to_close;
		static bool close_confirm;
		static bool close_cancel;

		static bool glfw_initialized;
		static bool glad_initialized;

		static void init_glfw();
		static void init_glad();

		static void register_callbacks();

	};

}