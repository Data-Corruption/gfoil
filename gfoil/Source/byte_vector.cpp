#include "byte_vector.h"

std::string byte_vector_to_string(byte_vector input) {
	std::string result = "";
	for (int i = 0; i < input.size(); i++)
		result = result + (char)input[i];
	return result;
}