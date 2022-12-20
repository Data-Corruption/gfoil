#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace gfoil {
	namespace system {

		extern std::string get_time();

		extern void throw_error(const std::string& error);
		extern void throw_error(const std::exception& error);
		extern void throw_error(const std::ifstream::failure& error);

		namespace files {
			extern bool exists(const std::string& path);
			extern void list_folder_contents(std::vector<std::string>& files, std::vector<std::string>& folders, const std::string path);
			extern void read(std::string& data, const std::string& path);
			extern void write(const std::string& data, const std::string& path);
			extern void append(const std::string& data, const std::string& path);
			extern std::string last_write_time(std::string path);
			extern void open_external(const std::string& path);
		};

		namespace log {
			const enum class levels { INFO = 0, WARN = 1, ERR = 2, NONE = 3 };
			// ----==== Members ====----
			extern std::string log_path;
			extern levels current_logging_level;
			// ----==== Methods ====----
			extern void set_level(levels level);
			extern void info(const std::string& msg);
			extern void warn(const std::string& msg);
			extern void error(const std::string& msg);
		};

		namespace dialog {
			const enum class types { OPEN_FOLDER = 0, OPEN_FILE = 1, SAVE_AS = 2 };
			// blocking, returns false is user clicks cancel or fails
			extern bool open(std::string& path, types type);
		};

	};
}