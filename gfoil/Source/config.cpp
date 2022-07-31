#include "config.h"

#include "system/system.h"

#include "text.h"

std::unordered_map<std::string, std::string> gfoil::config::data;

void gfoil::config::load() {
	if (!system::files::exists(config_path)) {
		system::log::warn("Config file not found, generating default one.");
		system::files::write(default_config, config_path);
	}

	text unparsed_config;
	system::files::read(unparsed_config.string, config_path);

	// parse data
	std::vector<text> lines, key_value_pair;
	unparsed_config.split(lines, '\n');

	for (auto& line : lines) {
		line.split(key_value_pair, ':');
		data[key_value_pair[0].string] = key_value_pair[1].string;
	}
}
void gfoil::config::save() {
	std::string result = "";

	for (auto& entry : data)
		result.append(entry.first + ':' + entry.second + '\n');
	result.pop_back();

	system::files::write(result, config_path);
}