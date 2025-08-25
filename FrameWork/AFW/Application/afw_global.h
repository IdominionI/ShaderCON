#pragma once

// This include of glew must be performed before anything else so as 
// to prevent the error GL.h included before glew error.
// Why on earth have not the authors of glew not taken account of this
// in writting their glew code to avoid this most common of compilatioon
// errors in dealing with glew and glfw that both use gl.h ????????????????? !!!!!!!!!!!!
//#define GLEW_STATIC
//#include <GL/glew.h>

//#include <FrameWork/VW_framework/Types/vw_type_definitions.h>
//
#include <Framework/AFW/Tools/afw_id_key_manager.h>
//
//#include <Universal_FW/ImNodes/Kernal/node.h>
//
//#include "../3D/vw_camera.h" // ++++++++
//#include <Framework/OGLFW/Objects/oglfw_camera_base.h> // ++++++++

// Singleton class to store global variables and functions
// to be used through out the application

#include <FrameWork/AFW/Utils/afw_logger.h>

class afw_globalc {
public:
	afw_globalc(const afw_globalc&) = delete;

	static afw_globalc& get() {
		static afw_globalc instance;
		return instance;
	}

	static afw_Logger_class *get_current_logger() { return get().current_logger; }
	static void set_current_logger(afw_Logger_class* n) { get().current_logger = n; }


	//static oglfw_camera_base_class *get_current_selected_camera() { return get().current_selected_camera; } // +++++++
	//static void						set_current_selected_camera(oglfw_camera_base_class *n) { get().current_selected_camera = n; } // +++++++
/*
	static glm::vec2 get_current_mouseXY() { return get().current_mouseXY; } // +++++++
	static void      set_current_mouseXY(glm::vec2 n) { get().current_mouseXY = n; } // +++++++

	static id_type get_current_selected_data_context_id() { return get().current_selected_data_context_id; }
	static void    set_current_selected_data_context_id(id_type n) { get().current_selected_data_context_id = n; }

	static id_type get_current_selected_entity_id() { return get().current_selected_entity_id; }
	static void    set_current_selected_entity_id(id_type n) { get().current_selected_entity_id = n; }

	static id_type get_current_selected_entity_type_id() { return get().current_selected_entity_type_id; }
	static void    set_current_selected_entity_type_id(id_type n) { get().current_selected_entity_type_id = n; }

	static id_type get_current_selected_entity_category_type_id() { return get().current_selected_entity_category_type_id; }
	static void    set_current_selected_entity_category_type_id(id_type n) { get().current_selected_entity_category_type_id = n; }

	//The get and set outliner selection global variables are so as to set the above gloabal entity and entity type
	//variables correctly between the outliner and node editor selection of scene entities. Without this the user
	//would need to deselect the node in the node editor before selecting an entity in the outliner widget, otherwise
	//no entity other than the selected node entity will be designated as the current selected entity. 
	static bool get_outliner_selection() { return get().outliner_selection; }
	static void set_outliner_selection(bool n) { get().outliner_selection = n; }

	static node_basis_class* get_current_selected_node() { return get().current_selected_node; }
	static void				 set_current_selected_node(node_basis_class* n) { get().current_selected_node = n; }

*/
	//static id_type  get_available_entity_id() { return get().entity_id_key.get_available_id_key(); }
	//static id_type  get_available_entity_id() { return get().entity_id_key.get_available_id_key(); }
	//static void     assign_free_entity_id(id_type entity_id) { get().entity_id_key.assign_free_id_key(entity_id); };
	//static void     reinitialise() { get().entity_id_key.reinitialise(); };

private:
	afw_globalc() {};

	afw_Logger_class *current_logger = nullptr;

	//oglfw_camera_base_class *current_selected_camera = nullptr; // +++++++
	//glm::vec2        current_mouseXY = { 0.0f,0.0f }; // +++++++
/*
	id_type current_selected_data_context_id = INVALID_ID;

	id_type current_selected_entity_id = INVALID_ID;
	id_type current_selected_entity_type_id = INVALID_ID;
	id_type current_selected_entity_category_type_id = INVALID_ID;

	bool outliner_selection = false;

	node_basis_class* current_selected_node = nullptr;

	// Not sure the following pointers is best here
	// to be reviewed and decided :: 
	// log panel to also be singleton class ?????
	//scene_manager_class *scene_manager = NULL;
	//log_panel_class     *log_panel     = NULL;
	//node_basis_class    *selected_node = NULL;
*/
	//id_key_manager_class<id_type> entity_id_key;

};
