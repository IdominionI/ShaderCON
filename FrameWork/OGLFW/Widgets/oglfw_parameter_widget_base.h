#pragma once

// !!!!!!!!!!! ############## NEED TO DECOUPLE THIS FROM OGLFW ################# !!!!!!!!!!!!!

#include <FrameWork/AFW/Common/afw_definitions.h>

#include <Source/SCON/Editor/Main_Window/Panels/log_panel.h> // This needs to be moved to a universal location

class oglfw_object_base_class;

class parameter_widget_base_class {
public:
	parameter_widget_base_class() {}
	~parameter_widget_base_class() {}

	id_type               current_selected_object_id = INVALID_ID;
	oglfw_object_base_class* object_to_execute = NULL;

	log_panel_class* log_panel = NULL;

	virtual void display_parameters_widget() {};

	virtual void set_parameter_widget_object_data() {};

	// add more virtual functions as required here

protected:

private:
};