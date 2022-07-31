#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <memory>

#include <climits>

#include <glm/glm.hpp>

#include "system/system.h"
#include "graphics/font.h"

/*
Text Rendering Example:

	// load the font you want to use
	font serif;
	serif.load(serif.ttf);

	// create the text
	text = "blah blah blah"
	text.font = &serif;
	// set position, spacing, scale, line/line char start and limits

	// create vertex info, call each time you edit the text and want it reflected visually
	text.build_buffer();

	// automatically buffers data, flushes, and draws
	text.draw(window_size);


	// for multiple text in 1 batch

	// bind font, this binds a shader, sets uniforms, binds atlas. It needs window size to convert screen coords to openl coords
	serif.bind(window.size);
	// buffer all the text data you want rendered
	serif.buffer(text.buffer_data);
	serif.buffer(other_text.buffer_data);
	// flush
	serif.flush();
	serif.draw();

	What This Does NOT Do...
		You need to set what depth function and blending mode you want

*/

namespace gfoil {

	class text {
	public:

		// ----==== Members ====----

		std::string string = "";

		// render info
		glm::vec2 position = glm::vec2(0.0f, 0.0f);   // coords in pixels
		glm::vec2 spacing = glm::vec2(0.0f, 0.0f);   // spacing in pixels
		glm::vec2 mask = glm::vec2(0, 0);           // relative limit for drawing in pixels

		float scale = 1.0f;
		glm::vec3 default_color = glm::vec3(0.9f, 0.9f, 0.9f);

		// index, color
		std::unordered_map<int, glm::vec3> color_codes;
		bool color_codes_embedded = true;

		unsigned int line_start = 0;
		unsigned int line_limit = 200;

		unsigned int line_char_start = 0;
		unsigned int line_char_limit = 200;

		font* font_renderer = nullptr;

		std::vector<vertex::tint> buffer_data;

		// ----==== Constructors / Deconstructor ====----

		text();

		text(const std::string& input);
		text(const char* input);
		text(std::vector<text>& input);

		~text();

		// ----==== Operator Overloaders ====----

		void operator = (const std::string& input);
		void operator = (const char* input);
		void operator = (std::vector<text>& input);

		void operator += (const text& input);
		bool operator == (const text& input);
		bool operator != (const text& input);

		std::string operator + (const text& input);

		operator std::string();

		char& operator [](int index);

		// ----==== Methods ====----

		int size();
		void clear();

		// returns segments of this text, split by a target character
		void split(std::vector<text>& output, char delimiter);

		/// returns a string of characters following the final '.' to the end of the text, useful for getting file exstensions.
		/// returns an empty string if there is no '.'
		std::string stem();

		// returns the number of times the target character occurs in the text
		int char_count(char target);

		// returns indices to the beginnings of all occurrences of the target sub string.
		void contains(std::vector<int>& output, std::string& target);
		bool contains(const std::string& target);

		// ----==== Rendering ====----

		/// Builds the render data that is used to draw the text. If the text, font, position, or spacing 
		/// have been changed this needs to be called before to the next draw to reflect that.
		void build_buffer();

		void unembed_color_codes();
		void embed_color_codes();

		void draw();
		void draw(generic_2d_camera& camera);

	};

}