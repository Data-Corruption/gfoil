#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace gfoil {
	class system {
	public:

		// ----==== Members ====----

		const enum class dialog_types { OPEN_FOLDER = 0, OPEN_FILE = 1, SAVE_AS = 2 };

		// ----==== Methods ====----

		static void throw_error(const std::string& e);
		static void throw_error(const std::exception& e);
		static void throw_error(const std::ifstream::failure& e);

		// blocking
		static std::string open_dialog(dialog_types type);

		class files {
		public:

			// overwrites file!
			static void write(const std::string& data, const std::string& path);

			static void append(const std::string& data, const std::string& path);

			static std::string read(const std::string& path);

			static void read(std::string& data, const std::string& path);

			static void list_contents(std::vector<std::string>& files, std::vector<std::string>& folders, std::string path);

			static bool exists(const std::string& path);

		};

		class log {
		public:

			// ----==== Members ====----

			const enum class levels { INFO = 0, WARN = 1, I_NEED_AN_ADULT = 2, NONE = 3 };

			static std::string log_path;

			static levels current_logging_level;

			// ----==== Methods ====----

			static void set_level(levels level);

			static void info(const std::string& msg);

			static void warn(const std::string& msg);

			static void error(const std::string& msg);

		};

	private:

		static std::string get_time();

	};
};