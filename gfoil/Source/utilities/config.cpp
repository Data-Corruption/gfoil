#include "config.h"

#include "../system/system.h"
#include "../text.h"

void gfoil::load_config() {
	if (!system::files::exists(config_path))
		system::files::write(default_config, config_path);

	text unparsed_config;
	system::files::read(unparsed_config.string, config_path);

	// parse data
	std::vector<text> lines, key_value_pair;
	unparsed_config.split(lines, '\n');
	
	for (auto& line : lines) {
		line.split(key_value_pair, ':');
		config[key_value_pair[0].string] = key_value_pair[1].string;
	}
}

void gfoil::save_config() {
	std::string result = "";

	for (auto& entry : config)
		result.append(entry.first + ':' + entry.second + '\n');
	result.pop_back();

	system::files::write(result, config_path);
}