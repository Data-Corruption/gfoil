#pragma once

#include <string>
#include <vector>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "buffer_array_object.h"
#include "generic_batch_renderer.h"

/// This is basically a tint vertex quad batch renderer that generates an atlas from a monospaced ".ttf" file.
/// Use the char_size to build vertex data and buffer, flush it like any other batch renderer, only difference 
/// is you dont bind a texture, it does that automatically.
class font {
public:

	// ----==== Members ====----

	std::string path;
	glm::uvec2 atlas_size;
	glm::uvec2 char_size;

	generic_batch_renderer renderer;

	// ----==== Methods ====----

	void load(std::string path, bool is_3d);
	void unload();

	// max buffer before flush is is 1k
	void buffer(std::vector<vertex::tint>& data);
	void flush();
	// draw flushed buffer
	void draw(glm::ivec2& window_size);
	void draw(std::vector<vertex::tint>& data, glm::ivec2& window_size);

private:

	shader font_shader;
	texture atlas;
	unsigned int uniform_offset_id;
	bool is_3d;
	bool is_loaded = false;

	glm::ivec2 last_window_size = glm::ivec2(0, 0);

	// binds font shader, sets uniforms, needs window size to convert screen coords to openl coords
	void bind(glm::ivec2& window_size);

};