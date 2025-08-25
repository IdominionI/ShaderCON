#pragma once
#include <string>
#include <vector>

#include <Framework/OGLFW/Objects/oglfw_camera_base.h>
#include <Framework/AFW/Application/afw_global.h> // Need to check that indeed unique ids are generated and stored

//#include "../Widgets/parameter_widget_base.h" // NOT YET IMPLEMENTED

#include <Framework/AFW/Tools/afw_id_key_manager.h>

inline id_key_manager_class<idu_type> camera_id_key;// This needed as global so all catagories in all editors have a unique category id

class scene_cameras_manager_class {
public:
	scene_cameras_manager_class() {
		selected_camera = add_camera();// Need at least one camera in the scene to be able to view things
		selected_camera->display_active = true;
	}

	~scene_cameras_manager_class() {
		for (size_t i = cameras.size()-1; i > -1; i--) {
			remove_camera(cameras[i]->id);
		}

		cameras.clear();
		selected_camera = NULL;
	}

	std::vector<oglfw_camera_base_class *>  cameras;
	oglfw_camera_base_class *selected_camera = NULL;

	// Camera data managemement;

	oglfw_camera_base_class *add_camera(bool new_camera_selected = true) {
		oglfw_camera_base_class *new_camera = new oglfw_camera_base_class;
		if (new_camera != NULL) {
			new_camera->id = camera_id_key.get_available_id_key();
			cameras.push_back(new_camera);
			if (new_camera_selected) {
				selected_camera = new_camera;
				selected_camera->display_active = true;
			}
		}
		return new_camera;
	}

	//oglfw_camera_base_class *add_camera(vw_camera_settings_struct_type camera_settings,bool new_camera_selected = true) {
	//	oglfw_camera_base_class *new_camera = add_camera(new_camera_selected);
	//	if(new_camera != NULL) new_camera->define_camera_settings(camera_settings);
	//	return new_camera;
	//}

	oglfw_camera_base_class* add_camera(camera_attributes_struct_type camera_settings, bool new_camera_selected = true) {
		oglfw_camera_base_class *new_camera = add_camera(new_camera_selected);
		if (new_camera != NULL) new_camera->define_camera_settings(camera_settings);
		return new_camera;
	}

	bool remove_camera(oglfw_camera_base_class *camera_to_remove) {
		if (camera_to_remove != NULL) {
			return remove_camera(camera_to_remove->id);
		}
		return false;
	}

	bool remove_camera(id_type camera_id) {
		if (cameras.size() < 2) return false; // Need at least one camera in the scene to be able to view things
		
		int index = get_camera_index(camera_id);
		if (index > -1) {
			delete cameras[index];
			cameras.erase(cameras.begin() + index);
			camera_id_key.assign_free_id_key(camera_id);// Free entity ID number to be reused when a new entity is created
			return true;
		}
		return false;
	}

	oglfw_camera_base_class *get_camera(id_type camera_id) {
		for (oglfw_camera_base_class *camera : cameras) {
			if (camera->id == camera_id) return camera;
		}
		return NULL;
	}

	bool enable_camera(id_type camera_id, bool enable = true) {
		oglfw_camera_base_class *camera = get_camera(camera_id);
		if (camera != NULL) {
			camera->enabled = enable;
			return true;
		}
		return false;
	}

	bool select_camera(id_type camera_id) {
		oglfw_camera_base_class *camera = get_camera(camera_id);
		if (camera != NULL) {
			//selected_camera->display_active = false;
			selected_camera = camera;
			//selected_camera->display_active = true;
			return true;
		}
		return false;
	}

	// !!!!!!!!!!!!!!!! Scene cameras management !!!!!!!!!!!!!!!!!!!!!!!!!

	// Much more needs to be done here
	
	// Not sure need this anymore
	//void scene_cameras_render_setup() {
	//	for (oglfw_camera_base_class* camera : cameras) {
	//		if (camera->display_active) {
	//			camera->begin();
	//		}
	//	}
	//}

	// FOLLOWING YET TO BE IMPLEMENTED
/*
	void disable_camera_mouse_input() {
		for (oglfw_camera_base_class* camera : cameras) {
			if (camera->display_active) {
				camera->disableMouseInput();
			}
		}
	}

	void enable_camera_mouse_input() {
		for (oglfw_camera_base_class* camera : cameras) {
			if (camera->display_active) {
				camera->enableMouseInput();
			}
		}
	}

	void disable_camera_keyboard_input() {
		for (oglfw_camera_base_class* camera : cameras) {
			if (camera->display_active) {
				camera->disable_keyboard_input();
			}
		}
	}

	void enable_camera_keyboard_input() {
		for (oglfw_camera_base_class* camera : cameras) {
			if (camera->display_active) {
				camera->enable_keyboard_input();
			}
		}
	}
*/
	// Function to display cameras overlays on screen
	// Not sure need this anymore
	//void scene_cameras_render() {
	//	for (oglfw_camera_base_class* camera : cameras) {
	//		if (camera->display_overlay) {
	//			camera->drawFrustum();
	//		}
	//	}
	//}

	// Not sure need this anymore
	//void scene_cameras_render_exit() {
	//	for (oglfw_camera_base_class* camera : cameras) {
	//		if (camera->display_active) {
	//			camera->end();
	//		}
	//	}
	//}


	// FOLLOWING YET TO BE IMPLEMENTED
	//void display_paramater_data_widget(oglfw_camera_base_class *camera) {
	//	ImGui::SetCursorPosX(100);
	//	ImGui::SetCursorPosY(100);
	//	ImGui::Text("Scene Camera Parameters");
	//};//To DO

	//parameter_widget_base_class   parameter_widget;



protected:

private:
	int get_camera_index(id_type camera_id) {
		for (size_t i=0; i < cameras.size(); i++) {
			if (cameras[i]->id == camera_id) return i;
		}
		return -1;
	}



};