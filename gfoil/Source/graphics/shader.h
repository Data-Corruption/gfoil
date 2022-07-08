#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

class shader {
public:

	/// Loads shader and increases its referencing counter
	/// name is a folder in app/shaders with vertex.glsl , fragment.glsl , and optionally geometry.glsl
	void load(const std::string& name);

	void bind();

	// Remember to call when done using
	void unload();

	// ----==== Uniform stuff ====----

	glm::uint get_uniform_id(const std::string& name);

	void set_uniform(const glm::uint& id, const bool& value);

	// --- unsigned ints
	void set_uniform(const glm::uint& id, const glm::uint& value);
	void set_uniform(const glm::uint& id, const glm::uvec2& value);
	void set_uniform(const glm::uint& id, const glm::uvec3& value);
	void set_uniform(const glm::uint& id, const glm::uvec4& value);

	void set_uniform(const glm::uint& id, const std::vector<glm::uint>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::uvec2>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::uvec3>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::uvec4>& value);

	// --- ints
	void set_uniform(const glm::uint& id, const int& value);
	void set_uniform(const glm::uint& id, const glm::ivec2& value);
	void set_uniform(const glm::uint& id, const glm::ivec3& value);
	void set_uniform(const glm::uint& id, const glm::ivec4& value);

	void set_uniform(const glm::uint& id, const std::vector<int>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::ivec2>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::ivec3>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::ivec4>& value);

	// --- floats
	void set_uniform(const glm::uint& id, const float& value);
	void set_uniform(const glm::uint& id, const glm::vec2& value);
	void set_uniform(const glm::uint& id, const glm::vec3& value);
	void set_uniform(const glm::uint& id, const glm::vec4& value);

	void set_uniform(const glm::uint& id, const std::vector<float>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::vec2>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::vec3>& value);
	void set_uniform(const glm::uint& id, const std::vector<glm::vec4>& value);

	// --- matrices 
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat2& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat3& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat4& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat2x3& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat3x2& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat2x4& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat4x2& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat3x4& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const glm::mat4x3& value);

	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat2>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat3>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat4>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat2x3>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat3x2>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat2x4>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat4x2>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat3x4>& value);
	void set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat4x3>& value);

private:

	struct loaded_shader {

		glm::uint reference_holders;
		glm::uint id;

		std::string path;

		void generate();
		void destroy();

		glm::uint compile_shader(std::string& path);

	};

	glm::uint cached_id = 0;

	static glm::uint currently_bound_shader;

	static std::vector<loaded_shader> loaded_shaders;

};