#pragma once

#include <array>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gfoil {

	class texture {
	public:

		glm::ivec2 size;

		/// Creates a texture from a file or caches its id and increases its referencing counter.
		/// Will unload current texture if one is loaded
		/// return true if first ref holder
		bool load(const std::string& path);
		/// Creates a texture or caches its id and increases its referencing counter.
		/// Will unload current texture if one is loaded
		/// return true if first ref holder
		bool generate(const std::string& tag);

		// sets target active texture slot to this texture. valid slots are 0-16
		void bind(const glm::uint& slot);

		/// Reloads the file so the texture reflects any modifications to the file.
		void reaload_file();

		// gets size from opengl, very $$$ maybe ?
		glm::ivec2 update_size();

		unsigned int get_id();

		// Remember to call when done using
		void unload();

	private:

		struct generated_texture {

			glm::uint reference_holders;
			glm::uint id;

			// if it has a path it was just generated from a file
			std::string path = "";
			std::string tag = "";

			void generate();
			void load_file(glm::ivec2& size);
			void destroy();

		};

		glm::uint cached_id = 0;

		static std::vector<generated_texture> generated_textures;
		static std::array<glm::uint, 16> active_textures;
		static glm::uint current_bound_texture;

	};

}