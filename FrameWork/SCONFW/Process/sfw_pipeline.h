#pragma once



#include "sfw_buffer_data_input.h"
#include <Framework/SCONFW/Shader/sfw_vertex_shader_base.h>
#include <Framework/SCONFW/Shader/sfw_geometry_shader_base.h>

#include <FrameWork/AFW/Geometry/afw_vertex.h>
// Uncommemnt following when needed
//#include <FrameWork/AFW/Geometry/afw_vertex_indices.h>
//#include <FrameWork/AFW/Geometry/afw_vertex_colors.h>
//#include <FrameWork/AFW/Geometry/afw_vertex_attributes.h>

#include <FrameWork/OGLFW/Geometry/oglfw_entity_object_geometry.h>
#include <FrameWork/OGLFW/Memory/oglfw_VAO_base.h>

#include <FrameWork/SCONFW/Shader/sfw_shader_base.h>

#include <Framework/SCONFW/Objects/sfw_camera.h>


class sconfw_pipeline_base_class
{
public:
	sconfw_pipeline_base_class() {}
	~sconfw_pipeline_base_class() {}

	void define_sconfw_camera(oglfw_camera_base_class *selected_camera) { sconfw_camwera.selected_camera = selected_camera; }

protected:
	sconfw_camera_class sconfw_camwera;


	sconfw_buffer_data_input_base_class buffer_data_input;	// defined custom user data buffer class
	sconfw_vertex_shader_base			vertex_shader;      // defined custom user vertex emulation shader class
	sconfw_geometry_shader_base			geometry_shader;	// defined custom user geometry emulation shader class. Comment out if if not used.

	void virtual perform_pipeline_procedure(Model& mesh_object, size_t mesh_id = 0) {}

	void virtual render_geometry(){}

private:

};
