#pragma once

#include <string>
#include <unordered_map>

const std::string config_path = "app/config.txt";
const std::string default_config =
"anti_aliasing:2\n"
"fps_limit:120\n"
"window_max:0\n"
"window_pos_x:null\n"
"window_pos_y:null\n"
"window_size_x:1280\n"
"window_size_y:720\n";

class config {
public:

	static std::unordered_map<std::string, std::string> data;

	static void load_config();
	static void save_config();

};