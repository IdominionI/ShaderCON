#pragma once

#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

#include "afw_vertex.h"

enum class geometry_type_enum { undefined, null, points, mesh, triangles, lines };
enum class base_geometry_type_enum { undefined, null, D1, D2, D3, D4, line, plane, surface, surface_4D, hex_2D, hcp_3D, grid_2D, grid_3D, grid_4D, polar, spherical, point_cloud, field, tensor };
enum class base_geometry_attribute_data_type { undefined, attr_bool, attr_int, attr_float, attr_double, attr_vec2, attr_vec3, attr_vec4, attr_string };// Add more datatypes as required eg glm::ivec2 etc

typedef float      D1_data_type;

typedef glm::vec2  D2_data_type;
typedef glm::vec2  plane_data_type;
typedef glm::vec2  polar_data_type;
typedef glm::vec2  hex_2D_data_type;
typedef glm::vec2  grid_2D_data_type;

typedef glm::vec3  D3_data_type;
typedef glm::vec3  surface_data_type;
typedef glm::vec3  hcp_3D_data_type;
typedef glm::vec3  grid_3D_data_type;
typedef glm::vec3  spherical_data_type;
typedef glm::vec3  point_cloud_data_type;

//Following geometry data types can be used to define a scalar field at a vertex loctation
typedef glm::vec4  D4_data_type;
typedef glm::vec4  grid_4D_data_type;
typedef glm::vec4  surface_4D_data_type;
typedef glm::vec4  field_data_type;

struct geometry_info_struct {
	void set_geometry_name(std::string _geometry_name) { geometry_name = _geometry_name; }
	void set_geometry_description(std::string _geometry_description) { geometry_description = _geometry_description; }
	void set_geometry_type(geometry_type_enum _geometry_type) { geometry_type = _geometry_type; }
	void set_base_geometry_type(base_geometry_type_enum _base_geometry_type) { base_geometry_type = _base_geometry_type; }
	void set_display_geometry(bool _display_geometry) { display_geometry = _display_geometry; }

	std::string             geometry_name = "";
	std::string             geometry_description = "";
	geometry_type_enum      geometry_type = geometry_type_enum::undefined;
	base_geometry_type_enum base_geometry_type = base_geometry_type_enum::D3;

	bool display_geometry = true;

};


template <class VT>
class afw_geometry_base_class : public afw_vertices_base_class <VT> {
public:

	using VBC = afw_vertices_base_class<VT>;

	afw_geometry_base_class() {}
	virtual ~afw_geometry_base_class() {}

	//void set_geometry_name       (std::string _geometry_name) { geometry_name = _geometry_name; }
	//void set_geometry_description(std::string _geometry_description) { geometry_description = _geometry_description; }
	//void set_geometry_type(geometry_type_enum _geometry_type) { geometry_type = _geometry_type; }
	//void set_base_geometry_type(base_geometry_type_enum _base_geometry_type) { base_geometry_type = _base_geometry_type; }
	//void set_display_geometry(bool _display_geometry) { display_geometry = _display_geometry; }

	//std::string             geometry_name		 = "";
	//std::string             geometry_description = "";
	//geometry_type_enum      geometry_type		 = geometry_type_enum::undefined;
	//base_geometry_type_enum base_geometry_type	 = base_geometry_type_enum::D3;

	//bool display_geometry = true;

	//std::vector<VT>                  vertices; // Vertex data of type T
	std::vector<glm::u64vec2>        edges;    // index of two vertex data points that make up a geometry edge
	std::vector<std::vector<size_t>> faces;    // index of vertex data points that make up a geometry face :: May need to change from size_t to uint32_t or uint64_t

	//size_t get_number_of_vertices() { return vertices.size(); }
	size_t get_number_of_edges()	{ return edges.size(); }
	size_t get_number_of_faces()	{ return faces.size(); }

	// -------------------------------------
	// Add geometry data
	// -------------------------------------

	//void add vertex(VT vertex) {
	//	vertices.push_back(vertex);
	//}

	void add_edge(size_t vertex1_index, size_t vertex2_index) {
		edges.push_back(glm::u64vec2(vertex1_index, vertex2_index));
	}

	void add_triangle_face(glm::u64vec3 triangle) {
		std::vector<size_t> face;
		face.push_back(triangle.x);
		face.push_back(triangle.y);
		face.push_back(triangle.z);
		add_face(face);
	}

	// Disabled for now
	/*
	void add_quad_face(glm::u64vec4 quad) {
		std::vector<size_t> face;
		face.push_back(quad.x);
		face.push_back(quad.y);
		face.push_back(quad.z);
		face.push_back(quad.w);
		add_face(face);
	}

	void add_ngon_face(std::vector<size_t> ngon) {
		std::vector<size_t> face;

		for (int i = 0; i < ngon.size(); i++) {
			face.push_back(ngon[i]);
		}

		add_face(face);
	}
	*/
	void add_face(std::vector<size_t> face) {
		faces.push_back(face);
	}

	// -------------------------------------
	// Delete geometry data
	// -------------------------------------

	//template <class VT>
	void delete_geometry(bool delete_attributes = false) {
		//vertices.clear();
		//delete_vertex_data<VT>();
		VBC::delete_vertex_data();

		edges.clear();
		faces.clear();
		//vertex_attributes.delete_all_attribute_data(delete_attributes);
	}


	bool delete_geometry_vertex(size_t vertex_index) {
		//if (vertex_index >= vertices.size() ) return false;     // for some reason compilor comes up with identifier not found 
		//if (vertex_index >= vertices<VT>.size() ) return false; //  even though vertices is a public vector
		if (vertex_index >= get_number_of_vertices<VT>()) return false;

		//vertices.erase(vertices.begin() + vertex_index);
		remove_vertex<VT>(vertex_index);
		delete_all_edges_with_vertex_index(vertex_index);
		delete_all_faces_with_vertex_index(vertex_index);

		return true;
	}

	bool delete_all_edges_with_vertex_index(size_t vertex_index) {
		if (vertex_index >= edges.size()) return false;

		for (size_t i = 0; i < edges.size(); i++) {
			if (edges[i].x == vertex_index || edges[i].y == vertex_index)
				edges.erase(edges.begin() + i);
		}

		return true;
	}

	bool delete_all_faces_with_vertex_index(size_t vertex_index) {
		if (vertex_index >= faces.size()) return false;

		for (size_t i = 0; i < faces.size(); i++) {
			for (size_t j = 0; j < faces[i].size(); j++) {
				if (faces[i][j] == vertex_index)
					faces[i].erase(faces[i].begin() + i);
			}

		}

		return true;
	}




protected:

public:

};