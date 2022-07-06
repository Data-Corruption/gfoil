#pragma once

#include <string>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace gfoil {
	class window {
	private:

		struct window_data {
			GLFWwindow* handle;
			std::string title;
			glm::ivec2 position;
			glm::ivec2 size;
			
			bool resized = false;
			bool maximized = false;
			bool minimized = false;
			bool asking_to_close = false;
			bool close_confirm = false;
			bool close_cancel = false;

			glm::vec4 clear_color = { 0.2f, 0.3f, 0.3f, 1.0f };
			glm::vec2 mouse_position;

			bool glfw_initialized = false;
			bool glad_initialized = false;
		};

		static window_data data;

		static void init_glfw();
		static void init_glad();

		static void register_callbacks();

	public:

		// ignores position if center_window is true
		static void generate(
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

		static void destroy();

		static void poll_events();

		static void swap_buffers();

		// --- setters ---
		static void set_title(std::string title);
		static void set_position(glm::ivec2 position);
		static void set_icon(std::string path);

		// --- getters ---
		static GLFWwindow* get_handle();
		static std::string get_title();
		static glm::ivec2 get_position();
		static glm::ivec2 get_size();

		// --- flags ---
		static bool flag_resized();
		static bool flag_maximized();
		static bool flag_minimized();
		static bool flag_glfw_initialized();
		static bool flag_asking_to_close();

		// true = close, false = cancel close
		static void close_response(bool value);

	};
}