#pragma once

#include "sfw_shader_definitions.h"

// ====================== Begin glsl variable data types =====================
struct glsl_variable_base_struct {
	glsl_variable_base_struct(int dt, std::string name, bool edit = false, bool con = false) {
		initialise(dt, name, edit, con);
	}

	virtual void initialise(int dt, std::string uname, bool edit, bool con = false) {
		editable = edit;
		uniform_name = uname;
		data_type = dt;
		constant = con;
	}

	bool editable = false;
	bool constant = false;
	int  data_type = GLSL_DT_UNDEFINED;
	std::string uniform_name = "";

	//VDT value;

	virtual void *get_variable_value(size_t i) = 0;
};


struct variable_float : public glsl_variable_base_struct {
	using glsl_variable_base_struct::glsl_variable_base_struct;// Bring to derived class the glsl_variable_base_struct contructor

	std::vector<float> values;

	void *get_variable_value(size_t i) override {
		return &values[i];
	}
};

struct variable_vec4 : public glsl_variable_base_struct {
	using glsl_variable_base_struct::glsl_variable_base_struct;// Bring to derived class the glsl_variable_base_struct contructor

	std::vector <glm::vec4> values;

	void* get_variable_value(size_t i) override {
		return &values[i];
	}
};

// ====================== End glsl variable data types =====================