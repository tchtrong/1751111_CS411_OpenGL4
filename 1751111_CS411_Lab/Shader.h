#pragma once
#include <GL/glew.h>
#include <sstream>
#include <fstream>
#include <string>
#include <string_view>


class Shader {
public:
	Shader(std::string_view file_path, GLenum type) : shader_id(glCreateShader(type)) {
		std::ifstream in(file_path.data());
		std::ostringstream os;
		os << in.rdbuf();
		std::string source = os.str();
		const GLchar* source_ = source.c_str();
		glShaderSource(shader_id, 1, &source_, NULL);
		glCompileShader(shader_id);
	}

	GLuint get_id () const {
		return shader_id;
	}

private:
	GLuint shader_id;
};