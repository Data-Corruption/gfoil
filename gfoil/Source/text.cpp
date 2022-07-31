#include "text.h"

#include "convert.h"
#include "graphics/window.h"

const std::string valid_hex_characters = "0123456789abcdef";

// ----==== Constructors / Deconstructor ====----

gfoil::text::text() {
	
}

gfoil::text::text(const std::string& input) {
	this->string = input;
}
gfoil::text::text(const char* input) {
	this->string = std::string(input);
}
gfoil::text::text(std::vector<text>& input) {
	this->string = "";
	for (int i = 0; i < input.size(); i++)
		this->string.append(input[i].string);
}

gfoil::text::~text() {

}

// ----==== Operator Overloaders ====----

void gfoil::text::operator = (const std::string& input) {
	this->string = input;
}
void gfoil::text::operator = (const char* input) {
	this->string = std::string(input);
}
void gfoil::text::operator = (std::vector<text>& input) {
	this->string = "";
	for (int i = 0; i < input.size(); i++)
		this->string.append(input[i].string);
}

void gfoil::text::operator += (const text& input) {
	this->string.append(input.string);
}
bool gfoil::text::operator == (const text& input) {
	if (this->string == input.string)
		return true;
	return false;
}
bool gfoil::text::operator != (const text& input) {
	if (this->string != input.string)
		return true;
	return false;
}

std::string gfoil::text::operator + (const text& input) {
	return (this->string + input.string);
}

gfoil::text::operator std::string() {
	return this->string;
}

char& gfoil::text::operator [](int index) {
	if (index >= size())
		system::log::error("Text index out of range");
	return string[index];
}

// ----==== Methods ====----

int gfoil::text::size() {
	return (int)this->string.size();
}

void gfoil::text::clear() {
	this->string.clear();
	this->string.shrink_to_fit();
}

void gfoil::text::split(std::vector<text>& output, char delimiter) {
	output.clear();
	output.shrink_to_fit();

	std::stringstream ss(this->string);
	std::string segment;

	while (std::getline(ss, segment, delimiter))
		output.emplace_back(segment);
}

std::string gfoil::text::stem() {
	std::vector<text> parts;
	this->split(parts, '.');

	if (parts.size()) {
		return parts.back().string;
	} else {
		return "";
	}
}

int gfoil::text::char_count(char target) {
	int count = 0;
	for (int i = 0; i < this->size(); i++)
		if (this->string[i] == target)
			count++;

	return count;
}

void gfoil::text::contains(std::vector<int>& output, std::string& target) {
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

bool gfoil::text::contains(const std::string& target) {
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

void gfoil::text::build_buffer() {
	if (font_renderer == nullptr)
		system::log::error("Text: Trying to build buffer with no font set");

	this->buffer_data.clear();
	this->buffer_data.shrink_to_fit();

	glm::vec2 cursor = this->position;
	glm::vec3 color = this->default_color;

	glm::uint line = 1;

	for (int i = 0; i < this->string.size(); i++) {

		// get color
		if (this->color_codes.count(i) != 0)
			color = color_codes[i];

		// new line code
		if (this->string[i] == '\n') {
			float vert_change = line * (scale * (font::atlas_pixel_height + this->spacing.y));
			cursor = glm::vec2(this->position.x, this->position.y - vert_change);
			line++;
			continue;
		}

		// if outside mask skip

		font::character_info c = this->font_renderer->get_char(this->string[i]);
		glm::vec2 cp = glm::vec2(cursor.x + (c.bearing.x * scale), -cursor.y - (c.bearing.y * scale));
		glm::vec2 cs = glm::vec2(c.bitmap_size.x * scale, c.bitmap_size.y * scale);

		cursor.x += c.advance.x * scale;
		if (this->string[i] == '	')
			cursor.x += (c.advance.x * scale) * 3.0f;

		if (!cs.x || !cs.y)
			continue;

		float txcw = c.bitmap_size.x / this->font_renderer->atlas_size.x;
		float txch = c.bitmap_size.y / this->font_renderer->atlas_size.y;

		//                                     /px           /py                           /tx           /ty   /ta 
		this->buffer_data.push_back({ glm::vec3(cp.x,        -cp.y - cs.y, 0.0f), glm::vec3(c.texture_offset,        txch, 1.0f), color }); // bl
		this->buffer_data.push_back({ glm::vec3(cp.x + cs.x, -cp.y - cs.y, 0.0f), glm::vec3(c.texture_offset + txcw, txch, 1.0f), color }); // br
		this->buffer_data.push_back({ glm::vec3(cp.x + cs.x, -cp.y,        0.0f), glm::vec3(c.texture_offset + txcw, 0.0f, 1.0f), color }); // tr
		this->buffer_data.push_back({ glm::vec3(cp.x,        -cp.y,        0.0f), glm::vec3(c.texture_offset,        0.0f, 1.0f), color }); // tl

	}
}

void gfoil::text::unembed_color_codes() {

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
void gfoil::text::embed_color_codes() {

	// create 2 vectors, one with indices, one with string hex codes

	// for each color code
		// insert color code
		// for each index
			// if index > current code index increment it by 7

}

void gfoil::text::draw() {
	if (this->font_renderer == nullptr)
		system::log::error("Attempting to draw text without having a font set");
	this->build_buffer();
	this->font_renderer->draw(this->buffer_data);
}

void gfoil::text::draw(generic_2d_camera& camera) {
	if (this->font_renderer == nullptr)
		system::log::error("Attempting to draw text without having a font set");
	this->build_buffer();
	this->font_renderer->draw(this->buffer_data, camera);
}
