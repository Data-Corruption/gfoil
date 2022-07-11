#include "font.h"

#include <algorithm>
#include <mutex>

#include "../system/system.h"
#include "texture.h"
#include "generic_index_buffers.h"
#include "../gfoil.h"

#include <glm/gtc/matrix_transform.hpp>

static std::mutex font_access_mtx;

FT_Library font::loaded_font::freetype;
bool font::loaded_font::is_freetype_initialized;

std::vector<font::loaded_font> font::loaded_fonts;

void font::load(std::string path, bool is_3d) {

	path = "app/fonts/" + path;

	if (this->cached_atlas_id != 0)
		this->unload();

	// load shared data
	this->load_shared_data(path);

	// generate batch renderer
	this->renderer.generate(4000, generic_batch_renderer::primative_type::TRIANGLES, vertex::type::TINTED, generic_index_buffers::quad_1k.id, 4, 6, false);

	// load font shader
	this->is_3d = is_3d;
	if (this->is_3d) {
		this->font_shader.load("font_3d");
		this->font_shader.bind();

		// 3d shader uniform crap
		system::log::error("3d font not setup yet");

	} else {
		this->font_shader.load("font");
		this->font_shader.bind();
		this->scale_transform_uniform_id = this->font_shader.get_uniform_id("u_scale_transform");
		this->window_transform_uniform_id = this->font_shader.get_uniform_id("u_window_transform");
		this->font_shader.set_uniform(this->font_shader.get_uniform_id("u_texture"), (unsigned int)0);
	}

}
void font::unload() {
	if (this->cached_atlas_id == 0)
		return;

	this->unload_shared_data();
	this->renderer.destroy();
	this->cached_atlas_id = 0;
}

void font::buffer(std::vector<vertex::tint>& data) {
	if (data.size() >= renderer.max_batch_size)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->renderer.buffer_data(data);
}

void font::flush() { this->renderer.flush(); }

void font::draw(float scale) {
	this->bind(scale);
	this->renderer.draw(); 
}

void font::draw(std::vector<vertex::tint>& data, float scale) {
	if (data.size() >= renderer.max_batch_size)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->bind(scale);
	this->renderer.current_batch_size = 0;
	this->renderer.buffer_data(data);
	this->renderer.flush();
	this->renderer.draw();
}

void font::bind(float scale) {

	// set scale transform
	this->font_shader.bind();
	glm::mat4 scale_transform = glm::mat4(1);
	scale_transform = glm::scale(scale_transform, glm::vec3(scale, scale, scale));
	this->font_shader.set_uniform(this->scale_transform_uniform_id, false, scale_transform);

	// set window transform
	glm::vec2 window_size = window::get_size();
	glm::mat4 window_transform = gfoil::ortho(0.0f, (float)window_size.x, -(float)window_size.y, 0.0f, -1.0f, 1.0f);
	this->font_shader.set_uniform(this->window_transform_uniform_id, false, window_transform);
	
	// bind atlas
	if (texture::bound_textures[0] == this->cached_atlas_id)
		return;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->cached_atlas_id);
	texture::bound_textures[0] = this->cached_atlas_id;

}

font::character_info font::get_char(char id) {
	for (auto& font : loaded_fonts) {
		if (font.atlas_id == this->cached_atlas_id) {
			return font.characters[id - 32];
		}
	}
}

glm::vec2 font::loaded_font::generate() {

	if (!is_freetype_initialized) {
		if (FT_Init_FreeType(&freetype))
			system::log::error("Freetype: failed to init library!");
		is_freetype_initialized = true;
	}

	FT_Face face;
	if (FT_New_Face(freetype, path.c_str(), 0, &face))
		system::log::error("Failed to open font: " + path);
	FT_Set_Pixel_Sizes(face, 0, atlas_pixel_height);

	glm::uvec2 atlas_size = glm::uvec2(0, 0);

	for (short int i = 32; i < 127; i++) { // skip first 32 characters since they are control codes / space
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			system::log::error("Failed to load character: " + std::to_string(i));

		atlas_size.x += face->glyph->bitmap.width;
		atlas_size.y = max(atlas_size.y, (float)face->glyph->bitmap.rows);
	}

	// create image

	glGenTextures(1, &this->atlas_id);
	glBindTexture(GL_TEXTURE_2D, this->atlas_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_size.x, atlas_size.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

	short int x = 0;
	for (int i = 32; i < 127; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			system::log::error("Failed to load character: " + std::to_string(i));
		
		this->characters[i - 32].ax = face->glyph->advance.x >> 6;
		this->characters[i - 32].bw = (float)face->glyph->bitmap.width;
		this->characters[i - 32].bh = (float)face->glyph->bitmap.rows;
		this->characters[i - 32].bl = (float)face->glyph->bitmap_left;
		this->characters[i - 32].bt = (float)face->glyph->bitmap_top;
		this->characters[i - 32].tx = (float)x / (float)atlas_size.x;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		x += face->glyph->bitmap.width;
	}

	return atlas_size;

}

void font::loaded_font::destroy() {
	glDeleteTextures(1, &this->atlas_id);
}

void font::load_shared_data(std::string path) {

	std::lock_guard<std::mutex> lock(font_access_mtx);

	// unload the current font or return if already loaded
	if (this->cached_atlas_id != 0) {
		for (auto& font : loaded_fonts)
			if ((font.atlas_id == this->cached_atlas_id) && (font.path == path))
				return;

		this->unload();
	}
	for (auto& font : loaded_fonts) {
		if (font.path == path) {
			this->cached_atlas_id = font.atlas_id;
			font.reference_holders++;
			return;
		}
	}

	// it's not loaded so load it
	loaded_fonts.emplace_back();
	loaded_fonts.back().path = path;
	loaded_fonts.back().reference_holders = 1;

	this->atlas_size = loaded_fonts.back().generate();
	this->cached_atlas_id = loaded_fonts.back().atlas_id;
}
void font::unload_shared_data() {
	if (this->cached_atlas_id == 0)
		return;

	std::lock_guard<std::mutex> lock(font_access_mtx);

	for (int i = 0; i < loaded_fonts.size(); i++) {
		if (loaded_fonts[i].atlas_id = this->cached_atlas_id) {
			loaded_fonts[i].reference_holders--;
			if (loaded_fonts[i].reference_holders == 0) {
				loaded_fonts[i].destroy();
				loaded_fonts.erase(loaded_fonts.begin() + i);
			}
			this->cached_atlas_id = 0;
			return;
		}
	}
}