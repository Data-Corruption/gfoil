#pragma once

#include <string>
#include <unordered_map>

namespace gfoil {
	class config {
	public:

		static std::string config_path;
		static std::string default_config;

		static std::unordered_map<std::string, std::string> data;

		static void load();
		static void save();

	};
}