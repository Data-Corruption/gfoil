#include "string_addons.hpp"

#include <sstream>

void gfoil::string_addons::split(std::vector<std::string>& output, const std::vector<char> delimiters, const std::string& input) {
	output.clear();
	std::string segment = "";
	for (auto& c : input) {
		if (std::find(delimiters.begin(), delimiters.end(), c) != delimiters.end()) { // if c is a delimiter
			if (segment.size() != 0) {
				output.push_back(segment);
				segment.clear();
			}
		} else {
			segment += c;
		}
	}
	if (segment.size() != 0)
		output.push_back(segment);
}

bool gfoil::string_addons::matches(const std::string& target, const unsigned int input_index, const std::string& input) {
	if (input_index + target.size() > input.size())
		return false;
	for (unsigned int x = 0; x < target.size(); x++)
		if (input[x + input_index] != target[x])
			return false;
	return true;
}

void gfoil::string_addons::contains(std::vector<int>& output, const std::string& target, const std::string& input) {
	output.clear();
	for (unsigned int i = 0; i < input.size(); i++) {
		if ((target.size() + i) > input.size())
			return;
		if (matches(input, i, target))
			output.emplace_back(i);
	}
}

std::string gfoil::string_addons::stem(const std::string& input) {
	std::vector<std::string> string_parts;
	split(string_parts, { '.' }, input);
	if (string_parts.size() == 0)
		return "";
	return string_parts.back();
}