#pragma once

#include <FrameWork/AFW/Graphics/afw_node_base.h>
#include <FrameWork/AFW/Geometry/afw_geometry.h>

#include <FrameWork/OGLFW/Shader/oglfw_shader.h>

#include <FrameWork/OGLFW/Memory/oglfw_VAO_base.h>
#include <FrameWork/OGLFW/Memory/oglfw_VBO_base.h>

class oglfw_entity_object_geometry_base_class : public geometry_info_struct {
public:
	oglfw_entity_object_geometry_base_class() {};
	~oglfw_entity_object_geometry_base_class() {};

	oglfw_VBO_class		vertex_VBO; // Mandatory
	oglfw_VAO_class		vertex_VAO; // Mandatory

	oglfw_shader_class *shader = nullptr;
	//ofMaterial* material = nullptr;

	bool         internal_shader   = false;
	bool         internal_material = false;

	bool        display_faces	  = true;
	bool        display_wireframe = false;
	bool        display_normals   = false;
	bool        display_vertices  = false;

	draw_mode_enum geometry_draw_mode = draw_mode_enum::UFW_UNDEFINED;

	virtual bool initialise() {return false;}

	virtual bool load_geometry_data(const std::string& filepath, float x_off) { return false; }
	virtual bool save_geometry_data(const std::string& filepath) { return false; }

	virtual bool add_geometry_data() { return false; }

	virtual bool update_geometry() { return false; }
	virtual bool update_uniforms() { return false; }

	virtual void render_geometry() {}

	// add_vertex function required but cannot be defined as a virtual function as the function paramter
	// needs to be spcified and thus cannot define this function with a different paramater data type
	// Using a template just unneccessarily complicates things as well and causes problems
	//virtual void add_vertex() = 0; // !!!!!!!!!!!! If define this as a pure virtual function just causes too many problems

	virtual void define_buffers() {};
	virtual void delete_buffers() { };

	bool has_internal_shader()   { return internal_shader; }
	bool has_internal_material() { return internal_material; }

	void set_geometry_draw_mode(draw_mode_enum draw_mode) { geometry_draw_mode = draw_mode; }

	// --------------------------------------------------
	oglfw_VBO_class get_VBO() {
		return vertex_VBO;
	}

	oglfw_VAO_class get_VAO() {
		return vertex_VAO;
	}
	// ------------- SHADER FUNCTIONS -----------------

	oglfw_shader_class *create_shader() {
		shader = new oglfw_shader_class;
		internal_shader = true;
		return shader;
	}

	void delete_shader() {
		if (internal_shader && shader != NULL) { // delete internal shader
			shader->delete_program();
		}

		shader = NULL;  // if have external shader do not unload it but simply assign shader to use to NULL;
		internal_shader = false;
	}

	void define_geometry_shader(oglfw_shader_class *_shader) {
		delete_shader();

		shader = _shader;
		internal_shader = true;
	}



private:

};
