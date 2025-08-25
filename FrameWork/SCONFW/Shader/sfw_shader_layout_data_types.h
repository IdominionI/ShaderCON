#pragma once

#include "sfw_shader_definitions.h"

// =================== Vertex layout Data Types ===============================

struct vlayout_base_struct {
	//vlayout_base_struct(int loc, shader_stream_type_enum st, glsl_data_type_enum dt , std::string name) {
	vlayout_base_struct(int loc, shader_stream_type_enum st, int dt, std::string name) {
		initialise(loc, st, dt, name);
	}

	//vlayout_base_struct(shader_stream_type_enum st, glsl_data_type_enum dt, std::string name) {
	vlayout_base_struct(shader_stream_type_enum st, int dt, std::string name) {
		initialise(-1, st, dt, name);
	}

	vlayout_base_struct() {}

	//virtual void initialise(int loc, shader_stream_type_enum st, glsl_data_type_enum dt, std::string name) {
	virtual void initialise(int loc, shader_stream_type_enum st, int dt, std::string name) {
		location = loc;
		stream_type = st;
		layout_name = name;
		data_type = dt;
		//set_data_type(); // Cannot have a virtual function called within a constructor or function called by a constructor
	}

	int location = -1;
	shader_stream_type_enum stream_type = shader_stream_type_enum::undefined;
	//glsl_data_type_enum     data_type   = glsl_data_type_enum::undefined;
	int						data_type = GLSL_DT_UNDEFINED;
	std::string layout_name = "";

	virtual size_t get_data_size() = 0;
	virtual void* get_data_element(size_t i) { return nullptr; }
	virtual void* get_data_vector() { return nullptr; }

	//virtual std::string display_data(unsigned int i) { return "\n"; };

};

struct vlayout_vec3 : public vlayout_base_struct {
	using vlayout_base_struct::vlayout_base_struct;// Bring to derived class the vlayout contructor

	glm::vec3 data_element; // Must be present to add data element to layout for output

	std::vector<glm::vec3> data = {};

	size_t get_data_size() override { return data.size(); }
	void* get_data_element(size_t i) override { return &data[i]; }
	void* get_data_vector() override { return &data; }

	void add_data_element() { // Must be present to add data element to layout for output
		data.push_back(data_element);
	}

	//std::string display_data(unsigned int i) override {
	//	return std::to_string(data[i].x) + " : " + std::to_string(data[i].y) + " : " + std::to_string(data[i].z);
	//}
};

struct vlayout_vec4 : public vlayout_base_struct {
	using vlayout_base_struct::vlayout_base_struct;// Bring to derived class the vlayout contructor

	glm::vec4 data_element; // Must be present to add data element to layout for output

	std::vector<glm::vec4> data = {};

	size_t get_data_size() override { return data.size(); }
	void* get_data_element(size_t i) override { return &data[i]; }
	void* get_data_vector() override { return &data; }

	void add_data_element() { // Must be present to add data element to layout for output
		data.push_back(data_element);
	}

	//std::string display_data(unsigned int i) override {
	//	return std::to_string(data[i].x) + " : " + std::to_string(data[i].y) + " : " + std::to_string(data[i].z);
	//}
};


// !!!!!!!!!!!!!!!!!! ADD MORE AS REQUIRED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!