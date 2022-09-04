#include "font.h"

#include <algorithm>
#include <unordered_map>

#include "../system/system.h"
#include "../gmath.h"
#include "window.h"
#include "generic_index_buffers.h"

#include <glm/gtc/matrix_transform.hpp>

std::vector<gfoil::font::character_set> gfoil::font::character_sets;

FT_Library gfoil::font::freetype;
bool gfoil::font::is_freetype_initialized = false;

void gfoil::font::generate(std::string target_path) {

	if (path != "")
		system::log::error("called generate on already generated font object");

	path = "app/fonts/" + target_path;

	// load shared data
	this->load_atlas_and_char_set();

	// generate batch renderer
	renderer.generate(vertex::type::TINTED);

	// load font shader
	font_shader.load("font");
	font_shader.bind();

	view_uniform_id = font_shader.get_uniform_id("u_view");
	transform_uniform_id = font_shader.get_uniform_id("u_transform");
	projection_uniform_id = font_shader.get_uniform_id("u_projection");

	font_shader.set_uniform(font_shader.get_uniform_id("u_texture"), (unsigned int)1);

}
void gfoil::font::destroy() {
	if (path == "")
		return;

	renderer.destroy();

	for (int i = 0; i < character_sets.size(); i++) {
		if (character_sets[i].atlas.get_id() == cached_atlas_id) {
			character_sets[i].reference_holders--;
			if (character_sets[i].reference_holders == 0) {
				character_sets[i].atlas.unload();
				character_sets.erase(character_sets.begin() + i);
			}
			return;
		}
	}
}

// binding
void gfoil::font::bind() {

	this->font_shader.bind();

	// transform
	glm::mat4 transform = glm::mat4(1.0f);
	this->font_shader.set_uniform(this->transform_uniform_id, false, transform);

	// view
	glm::mat4 view = glm::mat4(1.0f);
	this->font_shader.set_uniform(this->view_uniform_id, false, view);

	// projection
	glm::vec2 window_half = glm::vec2((float)window::size.x / 2, (float)window::size.y / 2);
	glm::mat4 projection = math::ortho(-window_half.x, window_half.x, -window_half.y, window_half.y, -1.0f, 1.0f);
	this->font_shader.set_uniform(this->projection_uniform_id, false, projection);

	// atlas
	for (auto& character_set : character_sets)
		if (character_set.atlas.get_id() == cached_atlas_id)
			character_set.atlas.bind(1);

}
void gfoil::font::bind(generic_2d_camera& target_camera) {

	this->font_shader.bind();

	// transform
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-target_camera.position.x, -target_camera.position.y, 0.0f));
	transform = glm::scale(transform, glm::vec3(target_camera.zoom, target_camera.zoom, 1.0f));
	this->font_shader.set_uniform(this->transform_uniform_id, false, transform);
	
	// view
	this->font_shader.set_uniform(this->view_uniform_id, false, target_camera.view);
	// projection
	this->font_shader.set_uniform(this->projection_uniform_id, false, target_camera.projection);
	
	// atlas
	for (auto& character_set : character_sets)
		if (character_set.atlas.get_id() == cached_atlas_id)
			character_set.atlas.bind(1);

}
void gfoil::font::bind(generic_3d_camera& target_camera) {

	this->font_shader.bind();

	// transform
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-target_camera.position.x, -target_camera.position.y, -target_camera.position.z));
	this->font_shader.set_uniform(this->transform_uniform_id, false, transform);

	// view
	this->font_shader.set_uniform(this->view_uniform_id, false, target_camera.view);
	// projection
	this->font_shader.set_uniform(this->projection_uniform_id, false, target_camera.projection);

	// atlas
	for (auto& character_set : character_sets)
		if (character_set.atlas.get_id() == cached_atlas_id)
			character_set.atlas.bind(1);

}

gfoil::font::character_info& gfoil::font::get_char(char target) {
	if (target < 32)
		system::log::error("Attempting to get character info for a char less than 32, aka a non visual char");
	for (auto& character_set : character_sets)
		if (character_set.atlas.get_id() == cached_atlas_id)
			return character_set.characters[target - 32];

	system::log::error("issue getting character info");
}

void gfoil::font::buffer(std::vector<quad::tint>& data) {
	if (data.size() >= renderer.max_quads_per_batch)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->renderer.buffer_data(data);
}

void gfoil::font::flush() { 
	this->renderer.flush(); 
}

void gfoil::font::draw() {
	this->bind();
	this->renderer.draw();
}
void gfoil::font::draw(generic_2d_camera& target_camera) {
	this->bind(target_camera);
	this->renderer.draw();
}
void gfoil::font::draw(generic_3d_camera& target_camera) {
	this->bind(target_camera);
	this->renderer.draw();
}

void gfoil::font::draw(std::vector<quad::tint>& data) {
	if (data.size() >= renderer.max_quads_per_batch)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->bind();
	this->renderer.renderer.current_batch_size = 0;
	this->renderer.buffer_data(data);
	this->renderer.flush();
	this->renderer.draw();
}
void gfoil::font::draw(std::vector<quad::tint>& data, generic_2d_camera& target_camera) {
	if (data.size() >= renderer.max_quads_per_batch)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->bind(target_camera);
	this->renderer.renderer.current_batch_size = 0;
	this->renderer.buffer_data(data);
	this->renderer.flush();
	this->renderer.draw();
}
void gfoil::font::draw(std::vector<quad::tint>& data, generic_3d_camera& target_camera) {
	if (data.size() >= renderer.max_quads_per_batch)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->bind(target_camera);
	this->renderer.renderer.current_batch_size = 0;
	this->renderer.buffer_data(data);
	this->renderer.flush();
	this->renderer.draw();
}

void gfoil::font::load_atlas_and_char_set() {

	// check if already loaded
	for (auto& character_set : character_sets) {
		if (character_set.path == path) {
			cached_atlas_id = character_set.atlas.get_id();
			character_set.reference_holders++;
			return;
		}
	}

	character_sets.emplace_back();
	character_sets.back().path = path;
	character_sets.back().reference_holders = 1;
	character_sets.back().atlas.generate(path);
	cached_atlas_id = character_sets.back().atlas.get_id();

	// init freetype
	if (!is_freetype_initialized) {
		if (FT_Init_FreeType(&freetype))
			system::log::error("Freetype: failed to init library!");
		is_freetype_initialized = true;
	}

	// load font face
	FT_Face face;
	if (FT_New_Face(freetype, path.c_str(), 0, &face))
		system::log::error("Failed to open font: " + path);
	FT_Set_Pixel_Sizes(face, 0, atlas_pixel_height);

	// get atlas size
	for (short int i = 32; i < 127; i++) { // skip first 32 characters since they are control codes / space
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			system::log::error("Failed to load character: " + std::to_string(i));

		character_sets.back().atlas.size.x += face->glyph->bitmap.width;
		character_sets.back().atlas.size.y = std::max((int)character_sets.back().atlas.size.y, (int)face->glyph->bitmap.rows);
	}

	atlas_size = character_sets.back().atlas.size;

	// bind and set image hints
	glBindTexture(GL_TEXTURE_2D, character_sets.back().atlas.get_id());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// generate empty image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, character_sets.back().atlas.size.x, character_sets.back().atlas.size.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

	// load them characters
	short int x = 0;
	for (int i = 32; i < 127; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			system::log::error("Failed to load character: " + std::to_string(i));

		character_sets.back().characters[i - 32].advance.x = (float)((signed long)face->glyph->advance.x >> 6);
		character_sets.back().characters[i - 32].bitmap_size.x = (float)face->glyph->bitmap.width;
		character_sets.back().characters[i - 32].bitmap_size.y = (float)face->glyph->bitmap.rows;
		character_sets.back().characters[i - 32].bearing.x = (float)face->glyph->bitmap_left;
		character_sets.back().characters[i - 32].bearing.y = (float)face->glyph->bitmap_top;
		character_sets.back().characters[i - 32].texture_offset = (float)x / (float)character_sets.back().atlas.size.x;

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, face->glyph->bitmap.width, face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		x += face->glyph->bitmap.width;
	}

}