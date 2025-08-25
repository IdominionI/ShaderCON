#pragma once

//#include "ofNode.h"
#include <FrameWork/AFW/Graphics/afw_node_base.h>

#include "../Shader/oglfw_shader_components.h"
#include "../Widgets/oglfw_parameter_widget_base.h"

#include "oglfw_object_material.h"

#include "../Geometry/oglfw_entity_object_geometry.h" // ++++++++++++

// ----------------------------------------------------------
//	Base class that defines any and all Virtual Worlds object
//  with univeral common functions to manage and display data
// ----------------------------------------------------------

class oglfw_object_base_class : public afw_node_base_class {
public:
	//oglfw_object_base_class() {}
	//~oglfw_object_base_class() {}

	int  object_category_id = INVALID_ID;
	int  object_type_id     = INVALID_ID;

	bool display_object = true;
	bool active_object  = false;

	vw_object_material_struct_type material; // +++++++++++


	parameter_widget_base_class *parameter_widget = nullptr;// parameter_widget needs to be a pointer to work


	// Function to define object geometry data to display in a viewport
	virtual bool define_geometry_data() { return false; }
	virtual bool update_object() { return false; }
	virtual void delete_object() {}// ++++

	virtual void clear_shader_variables() {}
	virtual void update_shader() {}
	virtual void update_shader_variables() {}

	virtual bool perform_animation_frame_functions() { return false; }

	oglfw_entity_object_geometry_base_class *geometry = nullptr; // ++++++++++++

	
	virtual void display_paramater_data_widget() {

		if (parameter_widget == NULL) {
			// ERROR message
//std::cout << "oglfw_object_base_class : display_paramater_data_widget : parameter_widget == NULL\n";
			return;
		}
//std::cout << "oglfw_object_base_class : display_paramater_data_widget\n";
		parameter_widget->current_selected_object_id = id;
		parameter_widget->object_to_execute          = this;

		parameter_widget->display_parameters_widget();
//std::cout << "oglfw_object_base_class : display_paramater_data_widget\n";;
	}

	virtual void update_boundary_limits() {} // This needs to be within ofNode geometry class

	// add more virtual functions as needed 

	virtual material_struct_type get_shader_parameters() { 
		material_struct_type a; 
		return a;
	};

	virtual bool save_generation_data(std::string file_pathname) { return false; }// Change (std::string to std::path
	virtual bool import_generation_data(std::string file_pathname) { return false; }// Change (std::string to std::path

protected:


private:
};
