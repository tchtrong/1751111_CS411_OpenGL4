#pragma once

#include "Shader.h"
#include <vector>

class Program {
public:
	Program() : program_id(glCreateProgram()){}

	void add_shader(std::string_view file_path, GLenum type) {
		shaders.emplace_back(file_path, type);
		glAttachShader(program_id, shaders.back().get_id());
	}

	void link_program() {
		glLinkProgram(program_id);
		delete_shader();
	}

	void delete_shader() {
		for (auto& shader : shaders) {
			glDeleteShader(shader.get_id());
		}
	}

	void delete_program() {
		glDeleteProgram(program_id);
	}

	GLuint get_id() const {
		return program_id;
	}

	void use_program() const {
		glUseProgram(program_id);
	}

private:
	GLuint program_id;
	std::vector<Shader> shaders;
};