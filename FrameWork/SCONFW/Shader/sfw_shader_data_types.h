#pragma once

#include "sfw_shader_layout_data_types.h"

#include "sfw_shader_uniform_data_types.h"

#include "sfw_shader_variable_data_types.h"

/*
#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <FrameWork/OGLFW/Shader/oglfw_shader_definitions.h>

enum class shader_stream_type_enum {undefined, in, out };

#define GLSL_DT_UNDEFINED 0
#define GLSL_DT_BOOL	  1
#define GLSL_DT_INT		  2
#define GLSL_DT_UINT	  3
#define GLSL_DT_FLOAT	  4
#define GLSL_DT_DOUBLE	  5
#define GLSL_DT_BVEC2	  6
#define GLSL_DT_BVEC3	  7
#define GLSL_DT_BVEC4	  8
#define GLSL_DT_IVEC2	  9
#define GLSL_DT_IVEC3	  10
#define GLSL_DT_IVEC4	  11
#define GLSL_DT_UVEC2	  12 
#define GLSL_DT_UVEC3	  13
#define GLSL_DT_UVEC4	  14
#define GLSL_DT_VEC2	  15
#define GLSL_DT_VEC3	  16
#define GLSL_DT_VEC4	  17
#define GLSL_DT_DVEC2	  18
#define GLSL_DT_DVEC3	  19
#define GLSL_DT_DVEC4	  20
#define GLSL_DT_MAT2	  21
#define GLSL_DT_MAT3	  22
#define GLSL_DT_MAT4	  23



enum class glsl_data_type_enum{undefined,
							   Bool,Int,UInt,Float,Double,
							   bvec2, bvec3, bvec4,
							   ivec2, ivec3, ivec4,
							   uvec2, uvec3, uvec4,
							   vec2, vec3, vec4,
							   dvec2, dvec3, dvec4,
							   mat2,mat3,mat4,
							   custom
};// Add others as required

struct  gl_PerVertex{
	glm::vec4 gl_Position;
	float gl_PointSize;// to use this must enable  GL_PROGRAM_POINT_SIZE via glEnable(GL_PROGRAM_POINT_SIZE);
	//float gl_ClipDistance[]; //uncomment if ever going to use
};
*/

/*
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
		editable     = edit;
		location     = loc;
		uniform_name = uname;
		data_type    = dt;
	}

	bool editable            = false;
	int  location			 = -1;
	int  data_type			 = GLSL_DT_UNDEFINED; 
	std::string uniform_name = "";

	//DT value;

	virtual void  *get_uniform_data() =0;
	//virtual void  display_uniform(float& xpos, float& ypos) = 0;
};

struct uniform_mat4 : public glsl_base_uniform_struct {
	using glsl_base_uniform_struct::glsl_base_uniform_struct;// Bring to derived class the glsl_base_uniform_struct contructor

	glm::mat4 value;

	void *get_uniform_data() override {
		return &value;
	}
};


// ====================== End glsl uniform data types =====================
*/
/*
// ====================== Begin glsl variable data types =====================
//template <typename VDT>
struct glsl_variable_base_struct {
	glsl_variable_base_struct(int dt, std::string name, bool edit = false, bool con = false) {
		initialise(dt, name, edit, con);
	}

	virtual void initialise(int dt, std::string uname, bool edit, bool con = false) {
		editable	 = edit;
		uniform_name = uname;
		data_type	 = dt;
		constant	 = con;
	}

	bool editable            = false;
	bool constant            = false;
	int  data_type			 = GLSL_DT_UNDEFINED; 
	std::string uniform_name = "";

	//VDT value;

	virtual void *get_variable_value () = 0;
};


struct variable_float : public glsl_variable_base_struct {
	using glsl_variable_base_struct::glsl_variable_base_struct;// Bring to derived class the glsl_variable_base_struct contructor

	float value;

	void *get_variable_value() override {
		return &value;
	}
};

// ====================== End glsl variable data types =====================
*/

//struct buffer_info {
//	unsigned int buffer_target_type;
//
//};

/*
// =================== Vertex layout Data Types ===============================


//typedef std::array<glm::vec3, 2> line_type;
//typedef std::array<glm::vec3, 3> triangle_type;

struct vlayout_base_struct {
	//vlayout_base_struct(int loc, shader_stream_type_enum st, glsl_data_type_enum dt , std::string name) {
	vlayout_base_struct(int loc, shader_stream_type_enum st, int dt , std::string name) {
		initialise(loc, st, dt, name);
	}

	//vlayout_base_struct(shader_stream_type_enum st, glsl_data_type_enum dt, std::string name) {
	vlayout_base_struct(shader_stream_type_enum st, int dt, std::string name) {
		initialise(-1, st,dt, name);
	}

	vlayout_base_struct() {}

	//virtual void initialise(int loc, shader_stream_type_enum st, glsl_data_type_enum dt, std::string name) {
	virtual void initialise(int loc, shader_stream_type_enum st, int dt, std::string name) {
		location    = loc;
		stream_type = st;
		layout_name = name;
		data_type   = dt;
		//set_data_type(); // Cannot have a virtual function called within a constructor or function called by a constructor
	}

	int location = -1;
	shader_stream_type_enum stream_type = shader_stream_type_enum::undefined;
	//glsl_data_type_enum     data_type   = glsl_data_type_enum::undefined;
	int						data_type   = GLSL_DT_UNDEFINED;
	std::string layout_name = "";

	virtual size_t get_data_size() = 0;
	virtual void  *get_data_element(size_t i) { return nullptr;}
	virtual void  *get_data_vector() { return nullptr; }

	//virtual std::string display_data(unsigned int i) { return "\n"; };

};

struct vlayout_vec3 : public vlayout_base_struct {
	using vlayout_base_struct::vlayout_base_struct;// Bring to derived class the vlayout contructor

	std::vector<glm::vec3> data = {};

	size_t get_data_size() override   { return data.size(); }
	void  *get_data_element(size_t i) override { return &data[i]; }
	void  *get_data_vector() override { return &data; }

	//std::string display_data(unsigned int i) override {
	//	return std::to_string(data[i].x) + " : " + std::to_string(data[i].y) + " : " + std::to_string(data[i].z);
	//}
};




// !!!!!!!!!!!!!!!!!! ADD MORE AS REQUIRED !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

// =================== Vertex Data Types ===============================
// In glsl must specify as an array otherwise will not work as one for each input vertex eg
// in VertexData{
//	  vec4 variable;
// } gs_in[];
// 
// For C++ emulation specify this as a vector eg std::vector<in_VertexData<glm::vec4>> gs_in;
// gs_in is a data identifer that can be of any name and any valid glsl data type or structure 
// within the vector datatype

//vlayout_????  in_varname = vlayout_????(shader_stream_type_enum::in, "in_varname");

// in glsl define as eg
// out VertexData {
//	   vec4 variable;
// } gs_out;
// 
// For C++ emulation specify this as a vector eg std::vector<out_VertexData<glm::vec4>> gs_out;
// gs_out is a data identifer that can be of any name and any valid glsl data type or structure 
// within the vector datatype
//vlayout_????  out_varname = vlayout_????(shader_stream_type_enum::out, "out_varname");