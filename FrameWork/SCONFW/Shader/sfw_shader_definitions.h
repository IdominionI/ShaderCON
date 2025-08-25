#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <FrameWork/OGLFW/Shader/oglfw_shader_definitions.h>

enum class shader_stream_type_enum { undefined, in, out };

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



enum class glsl_data_type_enum {
	undefined,
	Bool, Int, UInt, Float, Double,
	bvec2, bvec3, bvec4,
	ivec2, ivec3, ivec4,
	uvec2, uvec3, uvec4,
	vec2, vec3, vec4,
	dvec2, dvec3, dvec4,
	mat2, mat3, mat4,
	custom
};// Add others as required

struct  gl_PerVertex {
	glm::vec4 gl_Position;
	float gl_PointSize;// to use this must enable  GL_PROGRAM_POINT_SIZE via glEnable(GL_PROGRAM_POINT_SIZE);
	//float gl_ClipDistance[]; //uncomment if ever going to use
};