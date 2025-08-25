#pragma once

#include <Framework/AFW/Application/afw_global.h>

#include <FrameWork/OGLFW/Objects/oglfw_reference_grid.h>
#include <FrameWork/OGLFW/Objects/oglfw_object_base.h>
#include <FrameWork/OGLFW/Scene/oglfw_scene.h>

#include <Source/VW/Editor/Main_Window/Panels/log_panel.h>

// --------------------------------------
#include <FrameWork/OGLFW/Shader/oglfw_shader_components.h>

// --------------------------------------

#define SCENE_CATEGORY_REFGRID "refgrid"

class reference_grid_glsl_class : public oglfw_object_base_class {
public:
	reference_grid_glsl_class() {
		ref_grid = new oglfw_ref_grid_class;
		geometry = ref_grid;
	}
	~reference_grid_glsl_class() {
		delete_object();
	}

	oglfw_ref_grid_class* ref_grid = nullptr;

	vw_scene_class* vw_scene = nullptr;

	void delete_object() {
		delete geometry;
		geometry = nullptr;
	}

	bool define_scene_object_category() {
		int category_id = vw_scene->scene_entities_manager.get_objects_category_index(SCENE_CATEGORY_REFGRID);
		if (object_category_id == -1) {
//std::cout << "ref_grid_object_class:define_scene_object_category :  define_scene_object_category == -1 \n";
			object_category_id = vw_scene->scene_entities_manager.define_new_entity_category(SCENE_CATEGORY_REFGRID);
//std::cout << "ref_grid_object_class:define_scene_object_category :  define_scene_object_category = " + std::to_string(object_category_id) << std::endl;
		}
		else
			object_category_id = object_category_id;

		return true;
	}

	bool create_scene_ref_grid_object() {
		define_scene_object_category();

		if (!define_initial_shader_program()) {
			std::cout << "ref_grid_object_class:: create_scene_ref_grid_object :: !define_initial_shader_program() !!!!! : " << std::endl;
			return false;
		}

		geometry->initialise();

		if (!vw_scene->scene_entities_manager.add_object(this, object_category_id)) {// This is where hcp_voxel->id is defined
			std::cout << "ref_grid_object_class:: create_scene_ref_grid_object :: !vw_scene->scene_entities_manager.add_object !!!!! : " << std::endl;
			return false;
		}

		object_type_id = ENTITY_TYPE_OBJECT;

		if (geometry->shader) {
			if (!geometry->shader->shader_compile_successful) {
				std::cout << "ref_grid_object_class:: create_scene_ref_grid_object:ref_grid  Shaders not loaded !!!!! : " << std::endl;
				//std::string s = "jjjj\n";
				//cout << s << std::endl;
				std::cout << geometry->shader->compile_log << std::endl;
				std::cout << "ref_grid_object_class:: create_scene_ref_grid_object:ref_grid  Shaders not loaded !!!!! END : " << std::endl;
			}
			else {
				//cout << " Shaders loaded ^^^^^ : " << shader.getProgram() << " : " << std::endl;
				//std::cout << "ref_grid_object_class:: create_scene_ref_grid_object:  ref_grid Shaders loaded ^^^^^ : " << std::endl;
				//cout << entity_object03->geometry->shader->compile_log << std::endl;
				return true;
			}
		}
		else {
			std::cout << "ref_grid_object_class:: create_scene_ref_grid_object:  ref_grid Shader not created : " << std::endl;
			return false;
		}

		return true;

	}


	bool define_initial_shader_program() {
		//material_struct_type* shader_material = &voxel_hcp_object->voxel_object_data.shader_parameters;
		oglfw_shader_class* shader = geometry->shader;// +++++
		bool shaders_loaded = false;

		//shader_material->vertex_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		//shader_material->default_vertex_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_vertex_file).c_str();
		//shader_material->geometry_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_geometry_file).c_str();
		//shader_material->default_geometry_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_geometry_file).c_str();
		//shader_material->fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();
		//shader_material->default_fragment_shader_file_pathname = (default_shader_file_directory + voxel_hcp_default_fragment_file).c_str();

//printf("define_initial_shader_program :Vertex glsl file %s\n"   , shader_material->default_vertex_shader_file_pathname.string().c_str() );
//printf("define_initial_shader_program :Geometry glsl file %s\n" , shader_material->default_point_shader_file_pathname.string().c_str());
//printf("define_initial_shader_program :Fragment glsl file %s\n" , shader_material->default_fragment_shader_file_pathname.string().c_str());

		if (shader == NULL) {
			shader = geometry->create_shader();
		}
		else {
			if (geometry->has_internal_shader()) {
				geometry->delete_shader();
				shader = geometry->create_shader();

				if (shader == NULL) {
					//if (log_panel != NULL) log_panel->code_log.AddLog("ERROR : Could not create shader program.\n");
					printf("ERROR :define_initial_shader_program : Could not create shader program.\n");
					return false;
				}
			}
		}

		//if (!ofIsGLProgrammableRenderer()) {
		//	if (log_panel != NULL) log_panel->code_log.AddLog("ERROR : No openframworks progrmmable renderer available to display grapichs.\n");
		//	return false;
		//}

		geometry->initialise();// Critical to have this function initialted or nothing will be displayed using a geometry shader.

		//shader_material->glsl_vertex_shader_file_pathname = shader_material->default_vertex_shader_file_pathname;
		//shader_material->glsl_geometry_shader_file_pathname = shader_material->default_geometry_shader_file_pathname;
		//shader_material->glsl_fragment_shader_file_pathname = shader_material->default_fragment_shader_file_pathname;

		GLuint sprogram = shader->create_glsl_shader_program("Assets/Shaders/grid_vert.glsl", "Assets/Shaders/grid_geometry.glsl", "Assets/Shaders/grid_frag.glsl");// +++++++++++
		//GLuint sprogram = shader->create_glsl_shader_program("assets/shaders/grid_vert.glsl", "", "assets/shaders/grid_frag.glsl");// +++++++++++
		if (sprogram == 0) {
			printf("Failed to create initial reference grid shader program\n");
			
			// For some strange c++ thing, the compilor gives error expected an identifier if have LogLevel::ERROR confusing as this does not occur else where
			afw_globalc::get_current_logger()->log(LogLevel::DEBUG, shader->compile_log);

			return false;
		}
		//else {
		//	printf(" define_initial_shader_program ###### Create shader program\n");
			//shader_material->shader_program_id = shader->get_program();
			//if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_material->shader_program_id);

		//}

		//shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname, shader_material->glsl_geometry_shader_file_pathname);
		//shaders_loaded = shader->load(shader_material->glsl_vertex_shader_file_pathname, shader_material->glsl_fragment_shader_file_pathname);// Testing only  comment out or delete when testing complete

		//if (!shaders_loaded) {
		//	//printf("voxel_hcp_render_class: shader program not created\n");
		//	//printf("compile log \n %s \n", shader_db_manager.shader.compile_log.c_str());

		//	if (log_panel != NULL) log_panel->code_log.AddLog("INFO : Shader program compilation failed\n %s\n", shader->compile_log.c_str());
		//	return false;
		//}
		//else {
		//	//printf("voxel_hcp_render_class : shader program created : %i\n", shader_id);
		//	shader_material->shader_program_id = shader->getProgram();
		//	if (log_panel != NULL) log_panel->application_log.AddLog("INFO : Shader program created of ID : %i\n", shader_material->shader_program_id);
		//}//shader program created

		return true;
	}

private:

};

