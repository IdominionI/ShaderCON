#pragma once

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <FrameWork/OGLFW/Objects/model.h>

/*
struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Binormal;
	glm::vec4 Color;
};
*/

class sconfw_buffer_data_input_base_class
{
public:
	sconfw_buffer_data_input_base_class(){}
	~sconfw_buffer_data_input_base_class() {}

	float default_point_size = 1.0f;


	// The following long way is to emulate actual code for real openGL utilisation
	// ------------ For 3D model 
	void define_buffers(Model& mesh_object, size_t mesh_id = 0) {
		//generate_buffers();
		//bind_buffers();
		define_buffer_data(mesh_object, mesh_id);
		//define_VBO_buffer_data_layout();
	}

	void define_buffer_data(Model & mesh_object, size_t mesh_id = 0) {
		initialise_buffer_data(mesh_object, mesh_id);
	}
	// -------------
	
	// ---- ADD USER CUSTOM CODE TO DEFINE DATA PER VETEX TO TRANSMIT TO SCONFW SHADER HERE ----


	// ------------------------------------------------------------------------------------------

	std::vector<glm::vec3> positions   = {};
	std::vector<float>     point_sizes = {};
	//std::vector<std::vector<float>>  clip_distance = {}; //uncomment if ever going to use
	
	//Data vertex indices for index draw
	std::vector<unsigned int> indices = {};

	// default attributes
	std::vector<glm::vec4>    colors  = {};
	std::vector<glm::vec3>    normals = {};

protected:
	bool no_normals = false;

	void initialise_buffer_data(Model& mesh_object, size_t mesh_id = 0) { // One mesh at a time
		no_normals = true;
		clear_data();

		//for (Model::Mesh mesh : mesh_object.Meshes){
		for (Model::Mesh::Vertex vertex : mesh_object.Meshes[mesh_id].Vertices) {
			positions.push_back(vertex.Position);
			point_sizes.push_back(default_point_size);

			colors.push_back(vertex.Color);
			normals.push_back(vertex.Normal);
			if (vertex.Normal != glm::vec3(0.0f, 0.0f, 0.0f)) no_normals = false;
		}


		if (no_normals) {
			normals.clear();
			normals.shrink_to_fit();
		}

		indices = mesh_object.Meshes[mesh_id].Indices;
		//}
	}

	void clear_data() {
		positions.clear();
		point_sizes.clear();
		indices.clear();
		colors.clear();
		normals.clear();

		positions.shrink_to_fit();
		point_sizes.shrink_to_fit();
		indices.shrink_to_fit();
		colors.shrink_to_fit();
		normals.shrink_to_fit();
	}

private:
	

};
