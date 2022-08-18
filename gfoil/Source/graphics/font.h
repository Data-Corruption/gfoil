#pragma once

#include <string>
#include <vector>

#include <glad/glad.h> 
#include <glm/glm.hpp>

#include "small_types/vertex.h"

#include "cameras/generic_2d_camera.h"
#include "cameras/generic_3d_camera.h"
#include "shader.h"
#include "texture.h"
#include "buffer_array_object.h"
#include "generic_batch_renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

namespace gfoil {

	/// glyph info and atlas with a batch renderer, all you need to do is bring the vertices ;p batch limit is 10k quads
	/// atlas and character info is shared. When drawing if no camera is given it draws using 2d screen coords with no offsets
	class font {
	public:

		struct character_info {
			glm::vec2 advance;
			glm::vec2 bitmap_size;
			glm::vec2 bearing;
			// x offset of glyph in texture coordinates
			float texture_offset;
		};

		struct character_set {
			std::array<character_info, 95> characters;
			glm::uint reference_holders;
			std::string path = "";
			texture atlas;
		};

		// ----==== Members ====----

		static const glm::uint atlas_pixel_height = 64;
		static std::vector<character_set> character_sets;

		glm::uint projection_uniform_id = 0;
		glm::uint view_uniform_id = 0;

		bool is_monospaced = false;
		glm::vec2 atlas_size;
		std::string path = "";

		generic_batch_renderer renderer;
		shader font_shader;

		// ----==== Methods ====----

		void generate(std::string target_path);
		void destroy();

		// binds font shader, sets uniforms, activates atlas texture.
		void bind();
		void bind(generic_2d_camera& target_camera);
		void bind(generic_3d_camera& target_camera);

		character_info& get_char(char target);

		// batch quad limit is is 10k
		void buffer(std::vector<vertex::tint>& data);
		// send data to gpu
		void flush();
		// binds then draws flushed buffer
		void draw();
		void draw(generic_2d_camera& target_camera);
		void draw(generic_3d_camera& target_camera);
		// all in one, no need to call anything else
		void draw(std::vector<vertex::tint>& data);
		void draw(std::vector<vertex::tint>& data, generic_2d_camera& target_camera);
		void draw(std::vector<vertex::tint>& data, generic_3d_camera& target_camera);

	private:

		void load_atlas_and_char_set();

		static FT_Library freetype;
		static bool is_freetype_initialized;

		glm::uint cached_atlas_id;

	};

}