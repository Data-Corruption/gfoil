#include "texture.h"

#include "../system/system.h"
#include "../text.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

std::array<glm::uint, 16> gfoil::texture::active_textures = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };
glm::uint gfoil::texture::current_bound_texture = 0;
std::vector<gfoil::texture::generated_texture> gfoil::texture::generated_textures;

bool gfoil::texture::load(const std::string& path) {

	// unload the current texture or return if already loaded
	if (this->cached_id != 0) {
		for (auto& texture : generated_textures)
			if ((texture.id == this->cached_id) && (texture.path == path))
				return false;

		this->unload();
	}
	for (auto& texture : generated_textures) {
		if (texture.path == path) {

			this->cached_id = texture.id;
			texture.reference_holders++;

			return false;
		}
	}

	// it's not loaded so load it
	generated_textures.emplace_back();
	generated_textures.back().path = path;
	generated_textures.back().reference_holders = 1;
	generated_textures.back().generate();
	generated_textures.back().load_file(this->size);

	this->cached_id = generated_textures.back().id;

	return true;
}
bool gfoil::texture::generate(const std::string& tag) {

	// unload the current texture or return if already loaded
	if (this->cached_id != 0) {
		for (auto& texture : generated_textures)
			if ((texture.id == this->cached_id) && (texture.tag == tag))
				return false;

		this->unload();
	}
	for (auto& texture : generated_textures) {
		if (texture.tag == tag) {

			this->cached_id = texture.id;
			texture.reference_holders++;

			return false;
		}
	}

	// it's not loaded so load it
	generated_textures.emplace_back();
	generated_textures.back().tag = tag;
	generated_textures.back().reference_holders = 1;
	generated_textures.back().generate();

	this->cached_id = generated_textures.back().id;

	return true;
}

void gfoil::texture::bind(const glm::uint& texture_slot) {
	if (texture_slot > 16)
		system::log::error("Attempting to bind a texture slot over 16: " + std::to_string(texture_slot));

	if ((current_bound_texture == this->cached_id) && (active_textures[texture_slot] == this->cached_id))
		return;

	glActiveTexture(GL_TEXTURE0 + texture_slot);
	glBindTexture(GL_TEXTURE_2D, this->cached_id);

	current_bound_texture = this->cached_id;
	active_textures[texture_slot] = this->cached_id;
}

void gfoil::texture::reaload_file() {
	if (this->cached_id == 0)
		return;
	for (auto& texture : generated_textures)
		if (texture.id == this->cached_id)
			texture.load_file(this->size);
}

glm::ivec2 gfoil::texture::update_size() {
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &this->size.x);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &this->size.y);
}

unsigned int gfoil::texture::get_id() {
	return this->cached_id; 
}

void gfoil::texture::unload() {
	if (this->cached_id == 0)
		return;

	for (int i = 0; i < generated_textures.size(); i++) {
		if (generated_textures[i].id = this->cached_id) {
			generated_textures[i].reference_holders--;
			if (generated_textures[i].reference_holders == 0) {
				generated_textures[i].destroy();
				generated_textures.erase(generated_textures.begin() + i);
			}
			this->cached_id = 0;
			return;
		}
	}
}

void gfoil::texture::generated_texture::generate() {
	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	system::log::info("generated texture:" + std::to_string(this->id));
}
void gfoil::texture::generated_texture::load_file(glm::ivec2& size) {

	if (path == "")
		return;

	int width, height, format;

	unsigned char* data;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &width, &height, &format, 0);
	size = glm::ivec2(width, height);

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
		system::log::error("failed to load texture from file: " + this->path);
	}
	system::log::info("loaded texture data from file: " + this->path);
}
void gfoil::texture::generated_texture::destroy() {
	glDeleteTextures(1, &this->id);
	system::log::info("deleting texture: " + std::to_string(this->id));
}