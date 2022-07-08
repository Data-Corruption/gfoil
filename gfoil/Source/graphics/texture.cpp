#include "texture.h"

#include <algorithm>
#include <mutex>

#include "../system/system.h"

#include "../text.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glad/glad.h>

static std::mutex texture_access_mtx;

std::array<glm::uint, 16> texture::bound_textures;
std::vector<texture::loaded_texture> texture::loaded_textures;

FT_Library texture::freetype;
bool texture::is_freetype_initialized = false;
int texture::font_atlas_height;

void texture::load(const std::string& path_string) {

	text path = path_string;

	std::lock_guard<std::mutex> lock(texture_access_mtx);

	// unload the current texture or return if already loaded
	if (this->cached_id != 0) {
		for (auto& texture : loaded_textures)
			if ((texture.id == this->cached_id) && (texture.path == path.string))
				return;

		this->unload();
	}
	for (auto& texture : loaded_textures) {
		if (texture.path == path.string) {

			this->cached_id = texture.id;
			texture.reference_holders++;

			return;
		}
	}

	// it's not loaded so load it
	loaded_textures.emplace_back();
	loaded_textures.back().path = path;
	loaded_textures.back().reference_holders = 1;

	if (path.contains(".ttf")) {
		loaded_textures.back().generate_font();
	} else {
		loaded_textures.back().generate();
	}

	this->cached_id = loaded_textures.back().id;
}

void texture::bind(const glm::uint& texture_slot) {
	if (texture_slot > 16)
		system::log::error("Attempting to bind a texture slot over 16: " + std::to_string(texture_slot));

	if (bound_textures[texture_slot] == this->cached_id)
		return;

	glActiveTexture(GL_TEXTURE0 + texture_slot);
	glBindTexture(GL_TEXTURE_2D, this->cached_id);
	bound_textures[texture_slot] = this->cached_id;
}

void texture::reaload_file() {
	if (this->cached_id != 0) {
		for (auto& texture : loaded_textures) {
			if (texture.id == this->cached_id) {
				text path = texture.path;
				if (path.contains(".ttf")) {
					texture.load_font_file();
				} else {
					texture.load_file();
				}
			}
		}
	}
}

glm::ivec2 texture::get_size() {
	if (this->cached_id != 0) {
		for (auto& texture : loaded_textures) {
			if (texture.id == this->cached_id) {
				return texture.size;
			}
		}
	}
	return glm::ivec2(0, 0);
}

unsigned int texture::get_id() { return this->cached_id; }

void texture::unload() {
	if (this->cached_id == 0)
		return;
		
	std::lock_guard<std::mutex> lock(texture_access_mtx);

	for (int i = 0; i < loaded_textures.size(); i++) {
		if (loaded_textures[i].id = this->cached_id) {
			loaded_textures[i].reference_holders--;
			if (loaded_textures[i].reference_holders == 0) {
				loaded_textures[i].destroy();
				loaded_textures.erase(loaded_textures.begin() + i);
			}
			this->cached_id = 0;
			return;
		}
	}
}

void texture::loaded_texture::generate() {
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	this->load_file();
	system::log::info("generated texture:" + this->path + " id: " + std::to_string(this->id));
}
void texture::loaded_texture::load_file() {
	int width, height, format;

	unsigned char* data;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &width, &height, &format, 0);

	if (data) {
		switch (format) {
		case 1: format = GL_RED;  break;
		case 3: format = GL_RGB;  break;
		case 4: format = GL_RGBA; break;
		default: system::log::error("image format error, format = " + std::to_string(format)); break;
		}

		glBindTexture(GL_TEXTURE_2D, this->id);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}
	else {
		stbi_image_free(data);
		system::log::error("Failed to load texture: " + this->path);
	}
}

void texture::loaded_texture::generate_font() {
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	this->load_font_file();
	system::log::info("generated font atlas:" + this->path + " id: " + std::to_string(this->id));
}
void texture::loaded_texture::load_font_file() {

	if (!is_freetype_initialized) {
		if (FT_Init_FreeType(&freetype))
			system::log::error("Freetype: failed to init library!");
		is_freetype_initialized = true;
	}

	FT_Face face;
	if (FT_New_Face(freetype, path.c_str(), 0, &face))
		system::log::error("Failed to open font: " + path);
	FT_Set_Pixel_Sizes(face, 0, texture::font_atlas_height);

	// get height, width

	unsigned int char_width = 0;

	for (short int i = 33; i <= 127; i++) { // skip first 32 characters since they are control codes / space
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			system::log::error("Failed to load character: " + std::to_string(i));

		char_width = std::max(char_width, face->glyph->bitmap.width);
		this->size.y = std::max(this->size.y, face->glyph->bitmap.rows);
	}

	this->size.x = char_width * 95;

	// create image

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, this->size.x, this->size.y, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

	short int x = 0;
	for (int i = 33; i <= 127; i++) {
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
			continue;
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, char_width, this->size.y, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		x += char_width;
	}

}

void texture::loaded_texture::destroy() {
	glDeleteTextures(1, &this->id);
	system::log::warn("deleting texture:" + this->path);
}