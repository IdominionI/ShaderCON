#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <filesystem>

#define GLEW_STATIC
#include <GL/glew.h>



class oglfw_shader_base_class
{
public:
	oglfw_shader_base_class() {}
	~oglfw_shader_base_class() { delete_program(); }

	struct Source {
		// FIXME: change source to of::filesystem::path
		Source(GLuint type, const std::string& source, const std::filesystem::path& directoryPath)
			:type(type)
			, source(source)
			, directoryPath(directoryPath) {
		}

		Source() {}

		GLuint type;
		std::string source;
		std::string expandedSource;
		std::filesystem::path directoryPath;
		std::unordered_map<std::string, int>   intDefines;
		std::unordered_map<std::string, float> floatDefines;
	};

	struct Shader {
		GLuint id;
		Source source;
	};

	std::unordered_map<GLenum, Shader> shaders;
	std::unordered_map<std::string, GLint> uniformsCache;
	mutable std::unordered_map<std::string, GLint> attributesBindingsCache;
	std::unordered_map<std::string, GLint> uniformBlocksCache;

	bool bLoaded = false;
	GLuint program = 0;

	std::string compile_log               = "";
	bool        shader_compile_successful = true;

	void checkProgramInfoLog();
	void checkAndCreateProgram();
	bool linkProgram();
	bool checkProgramLinkStatus();

	GLint getAttributeLocation(const std::string& name)  const {
		return glGetAttribLocation(program, name.c_str());
	}

	//--------------------------------------------------------------
	//GLint getUniformLocation(const std::string& name)  const {
	//	if (!bLoaded) return -1;
	//	auto it = uniformsCache.find(name);
	//	if (it == uniformsCache.end()) {
	//		return -1;
	//	}
	//	else {
	//		return it->second;
	//	}
	//}

	GLuint get_program() {
		return program;
	}

	// Use the program
	void bind() {
		glUseProgram(program);
	}

	void release() {
		glUseProgram(0);
	}

	void use() {
		glUseProgram(program);
	}

	void use(GLuint shader_program_id) {
		glUseProgram(shader_program_id);
	}

	void delete_program() const {
		glDeleteProgram(program);
	}

	void delete_program(GLuint mprogram) const {
		glDeleteProgram(mprogram);
	}

	std::string nameForType(GLenum type);

protected:



private:


};

