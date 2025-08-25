#pragma once

#include <glm/glm.hpp>

//#include <ofLight.h> // to be implemented

#include "../Shader/oglfw_shader.h"

struct scene_settings_struct_type {
	glm::vec4 global_ambient_color = { 0.5f,0.5f,0.5f,1.0f };

	float global_ambience = 0.5f;

	glm::vec3 lighting_dir_vector = { 1.0f,0.0f,0.0f };
	float intensity = 0.75;

	//ofLight *global_light = NULL; // to be implemented

	void update_shader_uniforms(GLint shader_id) {
//std::cout << "vw_camera_base::update_shader_uniforms : shader_id 0000 " << shader_id << std::endl;

		if (shader_id < 0) {
			std::cout << "vw_camera_base::update_shader_uniforms : shader_id < 0 " << shader_id << std::endl;
			return;
		}

		oglfw_shader_class shader;

		shader.set_vec3(shader_id, lighting_dir_vector,"lighting_direction");
		
		shader.set_vec4(shader_id, global_ambient_color,"light_color");

		shader.set_f1(shader_id, global_ambience,"global_ambience");

		shader.set_f1(shader_id, intensity,"lighting_intensity");




		// If need other global setting to be used by shader glsl code add below 

//std::cout << "vw_camera_base::update_shader_uniforms 1111 : " << renderer->camera_forward_dir.x << " : " << renderer->camera_forward_dir.y << " : " << renderer->camera_forward_dir.z << " : " << std::endl;

	}

};