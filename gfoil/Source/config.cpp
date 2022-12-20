#include "config.hpp"

#include "system.hpp"
#include "string_addons.hpp"

std::string gfoil::config::config_path = "config.txt";
std::string gfoil::config::default_config = "";

std::unordered_map<std::string, std::string> gfoil::config::data;

void gfoil::config::load() {
	if (!system::files::exists(config_path)) {
		system::log::warn("Config file not found, generating default one.");
		system::files::write(default_config, config_path);
	}

	std::string unparsed_config;
	system::files::read(unparsed_config, config_path);

	if (unparsed_config == "") {
		system::log::warn("Config file is empty, generating default one.");
		system::files::write(default_config, config_path);
		unparsed_config = default_config;
	}

	// parse data
	std::vector<std::string> lines, key_value_pair;
	string_addons::split(lines, { '\n' }, unparsed_config);

	for (auto& line : lines) {
		string_addons::split(key_value_pair, { '~' }, line);
		data[key_value_pair[0]] = key_value_pair[1];
	}
}
void gfoil::config::save() {
	std::string result = "";

	for (auto& entry : data) {
		result.append(entry.first + '~' + entry.second + '\n');
	}
	result.pop_back();

	system::files::write(result, config_path);
}