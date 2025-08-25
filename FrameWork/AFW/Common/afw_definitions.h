#pragma once

#include <vector>
#include <string>

typedef int id_type;
typedef int data_type_id;

typedef unsigned int idu_type;
typedef unsigned int index_type;

typedef std::string string_type;

#define INVALID_ID        -1
#define NULL_CATEGORY_ID  UINT_MAX

enum class entity_object_type_enum { none, camera, light, object, node };

#define ENTITY_TYPE_CAMERA      100
#define ENTITY_TYPE_LIGHT       101
#define ENTITY_TYPE_OBJECT      102
#define ENTITY_TYPE_EDITOR_NODE 103

typedef std::vector<char> char_buffer_data_type;

enum class draw_mode_enum {
	UFW_UNDEFINED,
	UFW_PRIMITIVE_TRIANGLES,
	UFW_PRIMITIVE_TRIANGLE_STRIP,
	UFW_PRIMITIVE_TRIANGLE_FAN,
	UFW_PRIMITIVE_LINES,
	UFW_PRIMITIVE_LINE_STRIP,
	UFW_PRIMITIVE_LINE_LOOP,
	UFW_PRIMITIVE_POINTS,
};

typedef unsigned int afw_index_type;

