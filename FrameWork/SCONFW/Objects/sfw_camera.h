#pragma once

#include <FrameWork/OGLFW/Objects/oglfw_camera_base.h>

#include <FrameWork/SCONFW/Shader/sfw_shader_base.h>

class sconfw_camera_class //: public oglfw_camera_base_class
{
public:
	sconfw_camera_class() {}
	~sconfw_camera_class() {}

	oglfw_camera_base_class *selected_camera = nullptr;

	void update_view_uniform(uniform_mat4 &view) {
		view.value = selected_camera->get_view_matrix();
	}

	void update_modelviewprojection_uniform(uniform_mat4 &modelview) {
		modelview.value = selected_camera->get_projection_matrix() * selected_camera->get_view_matrix();
	}

	void update_projecion_uniform(uniform_mat4 &projecion) {
		projecion.value = selected_camera->get_projection_matrix();
	}

	void update_location_uniform(uniform_vec3 &position){ 
		position.value = selected_camera->position;
	}

	void update_loockat_uniform(uniform_vec3& loockat_location) {
		loockat_location.value = selected_camera->look_at_location;
	}

	void update_forward_uniform(uniform_vec3& loockat_location) {
		loockat_location.value = selected_camera->get_forward_dir();
	}

	void update_up_uniform(uniform_vec3& loockat_location) {
		loockat_location.value = selected_camera->get_up_dir();
	}

	void update_right_uniform(uniform_vec3& loockat_location) {
		loockat_location.value = selected_camera->get_right();
	}

	//define later 
	//shader.set_vec3(shader_id, getPosition(), "camera_position");
	//shader.set_vec3(shader_id, get_forward_dir(), "camera_front_vector");
	//shader.set_vec3(shader_id, get_up_dir(), "camera_up_vector");
	//shader.set_vec3(shader_id, get_right(), "camera_right_vector");

private:

};
