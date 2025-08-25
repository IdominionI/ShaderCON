#pragma once

#include "sfw_shader_definitions.h"

// ====================== Begin glsl uniform data types =====================

//template <typename DT>
struct glsl_base_uniform_struct {
	glsl_base_uniform_struct(int loc, int dt, std::string name, bool edit = false) {
		initialise(loc, dt, name, edit);
	}

	glsl_base_uniform_struct(int dt, std::string uname, bool edit = false) {
		initialise(-1, dt, uname, edit);
	}

	virtual void initialise(int loc, int dt, std::string uname, bool edit) {
		editable = edit;
		location = loc;
		uniform_name = uname;
		data_type = dt;
	}

	bool editable = false;
	int  location = -1;
	int  data_type = GLSL_DT_UNDEFINED;
	std::string uniform_name = "";

	//DT value;

	virtual void* get_uniform_data() = 0;
	//virtual void  display_uniform(float& xpos, float& ypos) = 0;
};

struct uniform_int : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	int value;

	void* get_uniform_data() override {
		return &value;
	}
};

struct uniform_float : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	float value;

	void* get_uniform_data() override {
		return &value;
	}
};

struct uniform_vec2 : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	glm::vec2 value;

	void* get_uniform_data() override {
		return &value;
	}
};

struct uniform_vec3 : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	glm::vec3 value;

	void* get_uniform_data() override {
		return &value;
	}
};

struct uniform_vec4 : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	glm::vec4 value;

	void* get_uniform_data() override {
		return &value;
	}
};

struct uniform_mat4 : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	glm::mat4 value;

	void* get_uniform_data() override {
		return &value;
	}
};


// ====================== End glsl uniform data types =====================