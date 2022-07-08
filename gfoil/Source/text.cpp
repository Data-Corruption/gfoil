#include "text.h"

#include "convert.h"

const std::string valid_hex_characters = "0123456789abcdef";

// ----==== Constructors / Deconstructor ====----

text::text() {
	
}

text::text(const std::string& input) {
	this->string = input;
}
text::text(const char* input) {
	this->string = std::string(input);
}
text::text(const byte_vector& input) {
	this->string = byte_vector_to_string(input);
}
text::text(std::vector<text>& input) {
	this->string = "";
	for (int i = 0; i < input.size(); i++)
		this->string.append(input[i].string);
}

text::~text() {

}

// ----==== Operator Overloaders ====----

void text::operator = (const std::string& input) {
	this->string = input;
}
void text::operator = (const char* input) {
	this->string = std::string(input);
}
void text::operator = (const byte_vector& input) {
	this->string = byte_vector_to_string(input);
}
void text::operator = (std::vector<text>& input) {
	this->string = "";
	for (int i = 0; i < input.size(); i++)
		this->string.append(input[i].string);
}

void text::operator += (const text& input) {
	this->string.append(input.string);
}
bool text::operator == (const text& input) {
	if (this->string == input.string)
		return true;
	return false;
}
bool text::operator != (const text& input) {
	if (this->string != input.string)
		return true;
	return false;
}

std::string text::operator + (const text& input) {
	return (this->string + input.string);
}

text::operator std::string() {
	return this->string;
}

char& text::operator [](int index) {
	if (index >= size())
		system::log::error("Text index out of range");
	return string[index];
}

// ----==== Methods ====----

int text::size() {
	return (int)this->string.size();
}

void text::clear() {
	this->string.clear();
	this->string.shrink_to_fit();
}

byte_vector text::to_byte_vector() {
	byte_vector result;
	for (int i = 0; i < this->size(); i++)
		result.emplace_back(this->string[i]);
	return result;
}

void text::split(std::vector<text>& output, char delimiter) {
	output.clear();
	output.shrink_to_fit();

	std::stringstream ss(this->string);
	std::string segment;

	while (std::getline(ss, segment, delimiter))
		output.emplace_back(segment);
}

std::string text::stem() {
	std::vector<text> parts;
	this->split(parts, '.');

	if (parts.size()) {
		return parts.back().string;
	} else {
		return "";
	}
}

int text::char_count(char target) {
	int count = 0;
	for (int i = 0; i < this->size(); i++)
		if (this->string[i] == target)
			count++;

	return count;
}

void text::contains(std::vector<int>& output, std::string& target) {
	output.clear();
	output.shrink_to_fit();

	for (int i = 0; i < this->size(); i++) {
		if ((target.size() + i) > this->size())
			return;

		bool found = true;
		for (int x = 0; x < target.size(); x++) {
			if (this->string[x + i] != target[x]) {
				found = false;
				break;
			}
		}

		if (found)
			output.emplace_back(i);
	}
}

bool text::contains(const std::string& target) {
	for (int i = 0; i < this->size(); i++) {
		if ((target.size() + i) > this->size())
			return false;

		bool found = true;
		for (int x = 0; x < target.size(); x++) {
			if (this->string[x + i] != target[x]) {
				found = false;
				break;
			}
		}

		if (found)
			return true;
	}
	return false;
}

void text::build_buffer() {
	if (font == nullptr)
		system::log::error("Text: Trying to build buffer with no font set");

	this->buffer_data.clear();
	this->buffer_data.shrink_to_fit();

	glm::vec3 color = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);

	float cw = this->font->char_size.x * scale;
	float ch = this->font->char_size.y * scale;

	unsigned int row = 0;
	unsigned int collum = 0;

	for (int i = 0; i < this->string.size(); i++) {

		// new line code
		if (this->string[i] == '\n') {
			row++;
			collum = 0;
			continue;
		}

		// skip unwanted rows(aka lines)
		if (row > line_limit)
			return;
		if (row < line_start)
			continue;

		// skip unwanted collums 
		if ((collum < line_char_start) || (collum > line_char_limit)) {
			collum++;
			continue;
		}

		// get color
		if (this->color_codes.count(i) != 0)
			color = color_codes[i];

		// not visable
		if (this->string[i] <= ' ') {
			collum++;
			continue;
		}

		// build quad
		float x = (((cw * collum) + (this->spacing.x * collum)) * this->scale) + this->position.x;
		float y = (((ch *    row) + (this->spacing.y *    row)) * this->scale) + this->position.y;

		// atlas texture offset
		float tx = (this->string[i] - 32) * cw;

		//                                     /px      /py                  /tx       /ty /ta 
		this->buffer_data.push_back({ glm::vec3(x,      y - ch, 0), glm::vec3(tx,      0, 1.0f), color }); // bl
		this->buffer_data.push_back({ glm::vec3(x + cw, y - ch, 0), glm::vec3(tx + cw, 0, 1.0f), color }); // br
		this->buffer_data.push_back({ glm::vec3(x + cw, y,      0), glm::vec3(tx + cw, 1, 1.0f), color }); // tr
		this->buffer_data.push_back({ glm::vec3(x,      y,      0), glm::vec3(tx,      1, 1.0f), color }); // tl

		collum++;

	}
}

void text::unembed_color_codes() {

	int i = 0;

	while (true) {

		if (i > (this->string.size() - 7))
			break;

		// check if start of color code
		if (this->string[i] != '#') {
			if (i > 0) {
				if (this->string[i - 1] == '\\') {
					i++;
					continue;
				}
			}

			// parse the code
			std::string hex = "";
			for (int x = (i + 1); x < (i + 7); x++) {
				// check if character is valid hex
				bool found = false;
				for (const auto& c : valid_hex_characters) {
					if (this->string[x] == c) {
						found = true;
						break;
					}
				}
				if (!found)
					break;
				hex += this->string[x];
			}

			if (hex.size() == 6) {

				this->color_codes[i] = convert::hex_color_to_vec3(hex);
				
				// erase the code from the string
				this->string.erase(i, 7);

			}
		} else {
			i++;
		}
	}
}
void text::embed_color_codes() {

	// create 2 vectors, one with indices, one with string hex codes

	// for each color code
		// insert color code
		// for each index
			// if index > current code index increment it by 7

}

void text::draw(glm::ivec2& window_size) {
	if (this->font == nullptr)
		system::log::error("Attempting to draw text without having a font set");
	this->build_buffer();
	this->font->draw(this->buffer_data, window_size);
}

