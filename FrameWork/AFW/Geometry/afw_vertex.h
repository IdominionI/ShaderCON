#pragma once

#include <vector>



#include "FrameWork/AFW/Common/afw_definitions.h"

//template<class V, class N, class C, class T>
/*
	For all classes that use a template, need to have all public functions
	in the header file or have linkage problems in compiling the code

	Potential problem if have circular referencing of class header files
*/

template<class V>
class afw_vertices_base_class {
public:
    //using VertexType   = V;

	afw_vertices_base_class() {
		draw_mode = draw_mode_enum::UFW_PRIMITIVE_TRIANGLES;
		vertices_changed = false;
		//indices_changed  = false;
		//use_indices      = false;
	}

	afw_vertices_base_class(std::vector<V>& vertices) {
		//afw_vertices_base_class<V>::afw_vertices_base_class(draw_mode_enum draw_mode, std::vector<V>& vertices) {
			//use_indices = false;
			//set_draw_mode(draw_mode);
		add_vertices(vertices);
	}

	virtual ~afw_vertices_base_class() {}

	//void set_draw_mode(draw_mode_enum m_draw_mode);

	draw_mode_enum get_draw_mode() { return draw_mode; }

	void add_vertex(const V& v) {
			vertices.push_back(v);
			vertices_changed = true;
			//bFacesDirty = true;
	}

	void add_vertices(const std::vector<V>& verts) {
		vertices.insert(vertices.end(), verts.begin(), verts.end());
		vertices_changed = true;
		//bFacesDirty = true;
	}

	void add_vertices(const V* verts, std::size_t amt) {
		vertices.insert(vertices.end(), verts, verts + amt);
		vertices_changed = true;
		//bFacesDirty = true;
	}
	/// \brief Add the vertices, normals, texture coordinates and indices of one mesh onto another mesh.
	/// Everything from the referenced mesh is simply added at the end
	/// of the current mesh's lists.
	void append(const afw_vertices_base_class<V> &mesh) {
		afw_index_type prevNum_vertices = static_cast<afw_index_type>(vertices.size());
		if (mesh.get_number_of_vertices()) {
			vertices.insert(vertices.end(), mesh.get_vertices().begin(), mesh.get_vertices().end());
		}

		//if (mesh.get_number_of_indices()) {
		//	for (auto index : mesh.get_indices()) {
		//		indices.push_back(index + prevNum_vertices);
		//	}
		//}
	}

	void remove_vertex(afw_index_type index) {
		if (index >= vertices.size()) {
			//ofLogError("ofMesh") << "remove_vertex(): ignoring out of range index " << index << ", number of vertices is" << vertices.size();
		}
		else {
			vertices.erase(vertices.begin() + index);
			vertices_changed = true;
			//bFacesDirty = true;
		}
	}

	void remove_vertices(afw_index_type start_index, afw_index_type end_index) {
		if (start_index >= vertices.size() || end_index > vertices.size()) {
			//ofLogError("ofMesh") << "remove_vertex(): ignoring out of range start_index " << start_index << " end_index " << end_index << ", number of vertices is" << vertices.size();
		}
		else {
			vertices.erase(vertices.begin() + start_index, vertices.begin() + end_index);
			vertices_changed = true;
			//bFacesDirty = true;
		}
	}

	void set_vertex(afw_index_type index, const V& v) {
		vertices[index] = v;
		vertices_changed = true;
		//indices_changed = true;
		//bFacesDirty = true;
	}

	std::size_t get_number_of_vertices() {return vertices.size();}
	V* get_vertices_data() { return vertices.data();}
	V get_vertex(afw_index_type i) { return vertices[i]; }
	std::vector<V>& get_vertices() { return vertices;}

	bool has_vertices() { return !vertices.empty(); }

	bool have_vertices_changed() {
		if (vertices_changed) {
			vertices_changed = false;
			return true;
		}
		else {
			return false;
		}
	}

	void clear_vertices() {
		vertices.clear();
		vertices_changed = true;
	}

	//void mergeDuplicate_vertices();

	void delete_vertex_data() {
		if (!vertices.empty()) {
			vertices_changed = true;
			vertices.clear();
		}
	}

	std::vector<V> vertices;
	bool vertices_changed;
protected:
	draw_mode_enum draw_mode;



	//std::vector<V> vertices;

private:

};