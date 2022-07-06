#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

#include "../system/system.h"

#include <glad/glad.h>

const std::string shader_folder = "app/shaders";

std::vector<gfoil::shader::loaded_shader> gfoil::shader::loaded_shaders;

// ----==== shader functions ====----

void gfoil::shader::load(const std::string& name) {

	std::string path = shader_folder + "/" + name;

	// unload the current shader or return if already loaded
	if (this->cached_id != 0) {
		for (auto& shader : loaded_shaders)
			if ((shader.id == this->cached_id) && (shader.path == path))
				return;

		this->unload();
	}

	for (auto& shader : loaded_shaders) {
		if (shader.path == path) {

			this->cached_id = shader.id;
			shader.reference_holders++;

			return;
		}
	}

	loaded_shaders.emplace_back();
	loaded_shaders.back().path = path;
	loaded_shaders.back().generate();
	loaded_shaders.back().reference_holders = 1;
	this->cached_id = loaded_shaders.back().id;

}

void gfoil::shader::bind() {
	if (currently_bound_shader != cached_id) {
		glUseProgram(cached_id);
		currently_bound_shader = cached_id;
	}
}

void gfoil::shader::unload() {
	if (this->cached_id == 0)
		return;

	for (int i = 0; i < loaded_shaders.size(); i++) {
		if (this->cached_id == loaded_shaders[i].id) {

			this->cached_id = 0;
			loaded_shaders[i].reference_holders--;

			if (loaded_shaders[i].reference_holders == 0)
				loaded_shaders.erase(loaded_shaders.begin() + i);

			return;
		}
	}
}

// ----==== Uniform stuff ====----

glm::uint gfoil::shader::get_uniform_id(const std::string& name) { return glGetUniformLocation(this->cached_id, name.c_str()); }

// --- bool
void gfoil::shader::set_uniform(const glm::uint& id, const bool& value) { glUniform1i(id, (int)value); }

// --- unsigned ints
void gfoil::shader::set_uniform(const glm::uint& id, const glm::uint& value)  { glUniform1ui(id, value); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::uvec2& value) { glUniform2ui(id, value.x, value.y); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::uvec3& value) { glUniform3ui(id, value.x, value.y, value.z); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::uvec4& value) { glUniform4ui(id, value.x, value.y, value.z, value.w); }

void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::uint>& value)  { glUniform1uiv(id, (GLsizei)value.size(), glm::value_ptr(value)); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::uvec2>& value) { glUniform2uiv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::uvec3>& value) { glUniform3uiv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::uvec4>& value) { glUniform4uiv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }

// --- ints
void gfoil::shader::set_uniform(const glm::uint& id, const int& value)        { glUniform1i(id, value); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::ivec2& value) { glUniform2i(id, value.x, value.y); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::ivec3& value) { glUniform3i(id, value.x, value.y, value.z); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::ivec4& value) { glUniform4i(id, value.x, value.y, value.z, value.w); }

void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<int>& value)        { glUniform1iv(id, (GLsizei)value.size(), glm::value_ptr(value)); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::ivec2>& value) { glUniform2iv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::ivec3>& value) { glUniform3iv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::ivec4>& value) { glUniform4iv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }

// --- floats
void gfoil::shader::set_uniform(const glm::uint& id, const GLfloat& value)   { glUniform1f(id, value); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::vec2& value) { glUniform2f(id, value.x, value.y); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::vec3& value) { glUniform3f(id, value.x, value.y, value.z); }
void gfoil::shader::set_uniform(const glm::uint& id, const glm::vec4& value) { glUniform4f(id, value.x, value.y, value.z, value.w); }

void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<GLfloat>& value)   { glUniform1fv(id, (GLsizei)value.size(), glm::value_ptr(value)); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::vec2>& value) { glUniform2fv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::vec3>& value) { glUniform3fv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const std::vector<glm::vec4>& value) { glUniform4fv(id, (GLsizei)value.size(), glm::value_ptr(value[0])); }

// --- matrices 
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat2>& value)   { glUniformMatrix2fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat3>& value)   { glUniformMatrix3fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat4>& value)   { glUniformMatrix4fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat2x3>& value) { glUniformMatrix2x3fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat3x2>& value) { glUniformMatrix3x2fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat2x4>& value) { glUniformMatrix2x4fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat4x2>& value) { glUniformMatrix4x2fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat3x4>& value) { glUniformMatrix3x4fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }
void gfoil::shader::set_uniform(const glm::uint& id, const bool& transpose, const std::vector<glm::mat4x3>& value) { glUniformMatrix4x3fv(id, (GLsizei)value.size(), transpose, glm::value_ptr(value[0])); }

// ----==== loaded_shader functions ====----

void gfoil::shader::loaded_shader::generate() {

	if (!system::files::exists(shader_folder))
		system::log::error("Shader: shader folder does not exist!");

	if (!system::files::exists(path))
		system::log::error("Shader: shader does not exist! - " + path);

	// get stage paths
	std::vector<std::string> stage_paths;
	stage_paths.emplace_back(this->path + "/vertex.glsl");
	stage_paths.emplace_back(this->path + "/fragment.glsl");
	if (!system::files::exists(this->path + "/geometry.glsl"))
		stage_paths.emplace_back(this->path + "/geometry.glsl");

	// compile stages
	std::vector<glm::uint> stage_ids;
	for (auto& stage_path : stage_paths)
		stage_ids.emplace_back(compile_shader(stage_path));

	// link stages
	this->id = glCreateProgram();
	for (auto& stage_id : stage_ids)
		glAttachShader(this->id, stage_id);
	glLinkProgram(this->id);

	int  success;
	char infoLog[512];
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(this->id, 512, NULL, infoLog);
		system::log::warn("Shader: linking failed when loading: " + this->path);
		system::log::error(" linking error: " + (std::string)infoLog);
	}

	system::log::info("Shader: sucsessfully built shader: " + this->path + " id: " + std::to_string(id));

	glUseProgram(this->id);
	for (auto& stage_id : stage_ids)
		glDeleteShader(stage_id);
	glUseProgram(0);
}
void gfoil::shader::loaded_shader::destroy() {
	system::log::warn("Shader: deleting shader: " + this->path + " id: " + std::to_string(id));
	glDeleteProgram(id);
}

glm::uint gfoil::shader::loaded_shader::compile_shader(std::string& path) {

	// get type
	GLenum shader_type = 0;
	if (path.find("vertex") != std::string::npos)        { shader_type = GL_VERTEX_SHADER; }
	else if (path.find("geometry") != std::string::npos) { shader_type = GL_GEOMETRY_SHADER; }
	else if (path.find("fragment") != std::string::npos) { shader_type = GL_FRAGMENT_SHADER; }
	else { system::log::error("Shader: incorrected naming convention for shader: " + path); }

	std::string raw_file;
	system::files::read(raw_file, path);
	const char* source_code = raw_file.c_str();
	glm::uint stage_id = glCreateShader(shader_type);
	glShaderSource(stage_id, 1, &source_code, NULL);
	glCompileShader(stage_id);

	int  success;
	char info_log[512];
	glGetShaderiv(stage_id, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(stage_id, 512, NULL, info_log);
		system::log::error("Shader: " + path + " compile failed! error: " + (std::string)info_log);
	}

	return stage_id;
}