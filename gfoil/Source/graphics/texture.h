#pragma once

#include <array>
#include <vector>
#include <string>

#include <glm/glm.hpp>

class texture {
public:

	static std::array<glm::uint, 16> bound_textures;

	/// Loads image and increases its referencing counter.
	/// Will unload current texture if one is loaded
	void load(const std::string& path_string);

	// Binds the texture to a slot for use. valid slots are 0-16
	void bind(const glm::uint& slot);

	/// load() only loads the file the first time it's called or when it has 
	/// been called again after all reference holders have called unload().
	/// reaload_file() will reload the file so the texture reflects any modifications to the file.
	void reaload_file();

	glm::ivec2 get_size();
	unsigned int get_id();

	// Remember to call when done using
	void unload();

private:

	struct loaded_texture {

		glm::uint reference_holders;
		glm::uint id;
		glm::uvec2 size;
		std::string path;

		void generate();
		void load_file();
		void destroy();

	};

	glm::uint cached_id = 0;

	static std::vector<loaded_texture> loaded_textures;

};