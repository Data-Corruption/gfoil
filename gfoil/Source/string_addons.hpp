#pragma once

#include <string>
#include <vector>

namespace gfoil {
	namespace string_addons {

		extern void split(std::vector<std::string>& output, const std::vector<char> delimiters, const std::string& input);
		// returns true if the target sub string is at input_index in input.
		extern bool matches(const std::string& target, const unsigned int input_index, const std::string& input);
		// returns indices to the beginnings of all occurrences of the target sub string.
		extern void contains(std::vector<int>& output, const std::string& target, const std::string& input);
		/// returns string segment following the final '.', useful for getting file exstensions. returns an empty string if there is no '.'
		std::string stem(const std::string& input);

	}
}