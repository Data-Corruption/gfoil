#pragma once

#include <string>
#include <vector>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "shader.h"
#include "buffer_array_object.h"
#include "generic_batch_renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

const glm::uint atlas_pixel_height = 64;

/// This is basically a tint vertex quad batch renderer that generates an atlas from a ".ttf" file.
/// Use get_char() to build vertex data and buffer, flush it like any other batch renderer, only difference 
/// is you dont bind a texture or shader it does that automatically.
class font {
public:

	glm::vec2 atlas_size;

	struct character_info {
		float ax;     // advance.x
		float bw, bh; // bitmap.width, bitmap.rows
		float bl, bt; // bitmap_left, bitmap_top
		float tx;     // x offset of glyph in texture coordinates
	};

	// ----==== Methods ====----

	void load(std::string path, bool is_3d);
	void unload();

	// binds font shader, sets uniforms, needs window size to convert screen coords to openl coords
	void bind(float scale);

	character_info get_char(char id);

	// max buffer before flush is is 1k
	void buffer(std::vector<vertex::tint>& data);
	// send data to gpu
	void flush();
	// draw flushed buffer
	void draw(float scale);
	// all in one, no need to call anything else
	void draw(std::vector<vertex::tint>& data, float scale);

private:

	struct loaded_font {

		static FT_Library freetype;
		static bool is_freetype_initialized;

		glm::uint reference_holders;
		glm::uint atlas_id;
		std::string path;
		std::array<character_info, 95> characters;

		glm::vec2 generate();
		void destroy();

	};

	static std::vector<loaded_font> loaded_fonts;

	generic_batch_renderer renderer;
	shader font_shader;

	glm::uint cached_atlas_id = 0;
	glm::uint scale_transform_uniform_id = 0;
	glm::uint window_transform_uniform_id = 0;

	bool is_3d;
	bool is_monospaced;

	void load_shared_data(std::string path);
	void unload_shared_data();

};