#include "font.h"

#include "../system/system.h"

#include "generic_index_buffers.h"

void gfoil::font::load(std::string path, bool is_3d) {

	if (this->is_loaded)
		this->unload();

	// load character atlas texture
	this->atlas.load(path);
	this->atlas_size = atlas.get_size();
	this->char_size = glm::uvec2(atlas_size.x / 95, atlas_size.y);

	// generate batch renderer
	this->renderer.generate(4000, primative_type::TRIANGLES, vertex_type::TINTED, generic_index_buffers::quad_1k.id, 4, 6, false);

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
		this->uniform_offset_id = this->font_shader.get_uniform_id("u_offset");
		this->font_shader.set_uniform(this->font_shader.get_uniform_id("u_texture"), (unsigned int)0);
	}

	this->is_loaded = true;

}
void gfoil::font::unload() {
	if (!this->is_loaded)
		return;

	this->atlas.unload();
	this->renderer.destroy();
	this->is_loaded = false;
}

void gfoil::font::buffer(std::vector<tint_vertex>& data) {
	if (data.size() >= renderer.max_batch_size)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->renderer.buffer(data);
}

void gfoil::font::flush() { this->renderer.flush(); }

void gfoil::font::draw(glm::ivec2& window_size) {
	this->bind(window_size);
	this->renderer.draw(); 
}

void gfoil::font::draw(std::vector<tint_vertex>& data, glm::ivec2& window_size) {
	if (data.size() >= renderer.max_batch_size)
		system::log::error("Font: Attempting to buffer a vector larger than max batch size!");

	this->bind(window_size);
	this->renderer.current_batch_size = 0;
	this->renderer.buffer(data);
	this->renderer.flush();
	this->renderer.draw();
}

void gfoil::font::bind(glm::ivec2& window_size) {

	this->font_shader.bind();
	this->atlas.bind(0);

	if (window_size != last_window_size) {
		glm::vec2 offset = glm::vec2((window_size.x / 2.0) * (2.0 / window_size.x), (window_size.y / 2.0) * (2.0 / window_size.y));
		this->font_shader.set_uniform(this->uniform_offset_id, offset);
		last_window_size = window_size;
	}
}