#include "oglfw_shader_base.h"

#include <Framework/AFW/Application/afw_global.h>

static std::unordered_map<GLuint, int>& getShaderIds() {
	static std::unordered_map<GLuint, int>* ids = new std::unordered_map<GLuint, int>;
	return *ids;
}

static std::unordered_map<GLuint, int>& getProgramIds() {
	static std::unordered_map<GLuint, int>* ids = new std::unordered_map<GLuint, int>;
	return *ids;
}
//--------------------------------------------------------------
static void retainProgram(GLuint id) {
	if (id == 0) return;
	if (getProgramIds().find(id) != getProgramIds().end()) {
		getProgramIds()[id]++;
	}
	else {
		getProgramIds()[id] = 1;
	}
}

//--------------------------------------------------------------
static void releaseProgram(GLuint id) {
	if (getProgramIds().find(id) != getProgramIds().end()) {
		getProgramIds()[id]--;
		if (getProgramIds()[id] == 0) {
			glDeleteProgram(id);
			getProgramIds().erase(id);
		}
	}
	else {
		//ofLogWarning("oglfw_shader_class") << "releaseProgram(): something's wrong here, releasing unknown program id " << id;
		afw_globalc::get_current_logger()->log(LogLevel::ERROR, "releaseProgram(): Releasing undefined program id ");
		glDeleteProgram(id);
	}
}


//--------------------------------------------------------------
void oglfw_shader_base_class::checkProgramInfoLog() {
	GLsizei infoLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
	if (infoLength > 1) {
		//ofBuffer infoBuffer;
		//infoBuffer.allocate(infoLength);
		//glGetProgramInfoLog(program, infoLength, &infoLength, infoBuffer.getData());

		std::vector<char> 	infoBuffer;
		infoBuffer.resize(infoLength);
		glGetProgramInfoLog(program, infoLength, &infoLength, infoBuffer.data());

		// TODO: it appears that Raspberry Pi only reports shader errors whilst linking,
		// but then it becomes hard to figure out whether the fragment or the
		// vertex shader caused the error.
		// We need to find a robust way of extracing this information from
		// the log, and unfortunately can't use regex whilst gcc on RPi is assumed to
		// be < 4.9, which is the first version fully supporting this c++11 feature.
		std::string msg = "oglfw_shader_class: program reports:\n";
		//ofLogError("ofShader") << msg + infoBuffer.getText();
		//afw_globalc::get_current_logger()->log(LogLevel::INFO, msg + infoBuffer.getText());
		afw_globalc::get_current_logger()->log(LogLevel::INFO, msg + std::string(infoBuffer.begin(), infoBuffer.end()));

		// VW_CUSTOM - ADDED BEGIN ++++++++++++++++++
		compile_log += msg.c_str() + std::string(infoBuffer.begin(), infoBuffer.end());
		shader_compile_successful = false;
		// VW_CUSTOM - ADDED END ++++++++++++++++++

//#ifdef TARGET_RAPSBERRY_PI
//		for (auto it : shaders) {
//			ofLogNotice("ofShader") << it.second.expandedSource;
//		}
//#endif
	}
}


//--------------------------------------------------------------
void oglfw_shader_base_class::checkAndCreateProgram() {
	//#ifndef TARGET_OPENGLES
	if (GL_ARB_shader_objects) {
		//#else
		//	if (ofIsGLProgrammableRenderer()) {
		//#endif
		if (program == 0) {
			//ofLogVerbose("ofShader") << "checkAndCreateProgram(): creating GLSL program";
			afw_globalc::get_current_logger()->log(LogLevel::INFO, "checkAndCreateProgram(): creating GLSL program");
			program = glCreateProgram();
			retainProgram(program);
		}
	}
	else {
		//ofLogError("ofShader") << "sorry, it looks like you can't run 'ARB_shader_objects'";
		//ofLogError("ofShader") << "please check the capabilites of your graphics card: http://www.ozone3d.net/gpu_caps_viewer";
		afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Can't run 'ARB_shader_objects'\nCheck the capabilites of your graphics card:");
		// VW_CUSTOM - ADDED BEGIN ++++++++++++++++++
		compile_log += "[ERROR] ofShader : Can't run 'ARB_shader_objects' \nCheck the capabilites of your graphics card: http://www.ozone3d.net/gpu_caps_viewer\n";
		// VW_CUSTOM - ADDED END ++++++++++++++++++
	}
}
//--------------------------------------------------------------
bool oglfw_shader_base_class::linkProgram() {
	if (shaders.empty()) {
		//ofLogError("ofShader") << "linkProgram(): trying to link GLSL program, but no shaders created yet";
		afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "linkProgram(): No shaders created.");
		// VW_CUSTOM - ADDED BEGIN ++++++++++++++++++
		compile_log += "[ERROR] oglfw_shader_class : linkProgram(): No shaders created.\n";
		// VW_CUSTOM - ADDED END ++++++++++++++++++
	}
	else {
		checkAndCreateProgram();

		for (auto it : shaders) {
			auto shader = it.second;
			if (shader.id > 0) {
				//ofLogVerbose("ofShader") << "linkProgram(): attaching " << nameForType(it.first) << " shader to program " << program;
				afw_globalc::get_current_logger()->log(LogLevel::INFO, "linkProgram(): attaching " + nameForType(it.first) + " shader to program " + std::to_string(program));
				glAttachShader(program, shader.id);
			}
		}

		glLinkProgram(program);

		checkProgramLinkStatus();


		// Pre-cache all active uniforms
		GLint numUniforms = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &numUniforms);

		GLint uniformMaxLength = 0;
		glGetProgramiv(program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &uniformMaxLength);

		GLint count = -1;
		GLenum type = 0;
		GLsizei length;
		GLint location;
		std::vector<GLchar> uniformName(uniformMaxLength);
		for (GLint i = 0; i < numUniforms; i++) {
			glGetActiveUniform(program, i, uniformMaxLength, &length, &count, &type, uniformName.data());
			std::string name(uniformName.begin(), uniformName.begin() + length);
			// some drivers return uniform_name[0] for array uniforms
			// instead of the real uniform name
			location = glGetUniformLocation(program, name.c_str());
			if (location == -1) continue; // ignore uniform blocks

			uniformsCache[name] = location;
			auto arrayPos = name.find('[');
			if (arrayPos != std::string::npos) {
				name = name.substr(0, arrayPos);
				uniformsCache[name] = location;
			}
		}

		//#ifndef TARGET_OPENGLES
#ifdef GLEW_ARB_uniform_buffer_object
		if (GLEW_ARB_uniform_buffer_object) {
			// Pre-cache all active uniforms blocks
			GLint numUniformBlocks = 0;
			glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks);

			count = -1;
			type = 0;
			std::vector<GLchar> uniformBlockName(uniformMaxLength);
			for (GLint i = 0; i < numUniformBlocks; i++) {
				glGetActiveUniformBlockName(program, i, uniformMaxLength, &length, uniformBlockName.data());
				std::string name(uniformBlockName.begin(), uniformBlockName.begin() + length);
				uniformBlocksCache[name] = glGetUniformBlockIndex(program, name.c_str());
			}
		}
#endif
		//#endif
		//
		//#ifdef TARGET_ANDROID
		//		ofAddListener(ofxAndroidEvents().unloadGL, this, &ofShader::unloadGL);
		//#endif

				// bLoaded means we have loaded shaders onto the graphics card;
				// it doesn't necessarily mean that these shaders have compiled and linked successfully.
		bLoaded = true;
	}
	return bLoaded;
}

bool oglfw_shader_base_class::checkProgramLinkStatus() {
	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	GLuint err = glGetError();
	if (err != GL_NO_ERROR) {
		//ofLogError("ofShader") << "checkProgramLinkStatus(): OpenGL generated error " << err << " trying to get the program link status, does your video card support shader programs?";
		afw_globalc::get_current_logger()->log(LogLevel::ERROR, "checkProgramLinkStatus() : OpenGL generated error " + std::to_string(err) + " trying to get the program link status, does your video card support shader programs ? ");
		// VW_CUSTOM - ADDED BEGIN ++++++++++++++++++
		compile_log += "[ERROR] oglfw_shader_class :checkProgramLinkStatus(): OpenGL generated error " + std::to_string(err) + " trying to get the program link status, does your video card support shader programs?\n";
		// VW_CUSTOM - ADDED END ++++++++++++++++++
		return false;
	}
	if (status == GL_TRUE) {
		//ofLogVerbose("ofShader") << "checkProgramLinkStatus(): program " << program << " linked";
		afw_globalc::get_current_logger()->log(LogLevel::ERROR, "checkProgramLinkStatus() :program " + std::to_string(program) + " linked");

		//compile_log += "ofShader checkProgramLinkStatus() : program " + std::to_string(program) + " linked\n";
	}
	else if (status == GL_FALSE) {
		//ofLogError("ofShader") << "checkProgramLinkStatus(): program failed to link";
		afw_globalc::get_current_logger()->log(LogLevel::ERROR, "checkProgramLinkStatus(): program failed to link");
		// VW_CUSTOM - ADDED BEGIN ++++++++++++++++++
		compile_log += "[ERROR] oglfw_shader_class : checkProgramLinkStatus(): program failed to link\n";
		// VW_CUSTOM - ADDED END ++++++++++++++++++
		checkProgramInfoLog();
		return false;
	}
	return true;
}

std::string oglfw_shader_base_class::nameForType(GLenum type) {
	switch (type) {
		case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
		case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
#ifndef TARGET_OPENGLES
		case GL_GEOMETRY_SHADER_EXT: return "GL_GEOMETRY_SHADER_EXT";
#ifdef glDispatchCompute
		case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";
#endif
#endif
		default: return "UNKNOWN SHADER TYPE";
	}
}

