
#include <unordered_map>

#include "glm/glm.hpp"

#include "afw_vertex_attributes.h"

//--------------------------------------------------------------
template<class VA>
afw_vertex_attribute_base_class<VA>::afw_vertex_attribute_base_class() {
	//draw_mode = draw_mode_enum::UFW_PRIMITIVE_TRIANGLES;
	attributes_changed = false;
}

//--------------------------------------------------------------
template<class VA>
//afw_vertex_attribute_base_class<V>::afw_vertex_attribute_base_class(draw_mode_enum draw_mode, std::vector<V>& vertices) {
afw_vertex_attribute_base_class<VA>::afw_vertex_attribute_base_class(const std::vector<VA> &vertices) {
	//set_draw_mode(draw_mode);
	add_vertices(vertices);
	attributes_changed = false;
}

//--------------------------------------------------------------
template<class VA>
void afw_vertex_attribute_base_class<VA>::delete_vertex_attributes() {
	if (!vertex_attributes.empty()) {
		attributes_changed = true;
		vertex_attributes.clear();
	}
}

//--------------------------------------------------------------
template<class VA>
bool afw_vertex_attribute_base_class<VA>::have_vertex_attributes_changed() {
	if (attributes_changed) {
		attributes_changed = false;
		return true;
	}
	else {
		return false;
	}
}

//ADDERS
//--------------------------------------------------------------
template<class VA>
void afw_vertex_attribute_base_class<VA>::add_vertex_attribute(const VA &va) {
	vertex_attributes.push_back(va);
	attributes_changed = true;
	//bFacesDirty = true;
}

//--------------------------------------------------------------
template<class VA>
void afw_vertex_attribute_base_class<VA>::add_vertex_attributes(const std::vector<VA> &verts) {
	vertex_attributes.insert(vertex_attributes.end(), verts.begin(), verts.end());
	attributes_changed = true;
	//bFacesDirty = true;
}

//--------------------------------------------------------------
template<class VA>
void afw_vertex_attribute_base_class<VA>::add_vertex_attributes(const VA &verts, std::size_t amt) {
	vertex_attributes.insert(vertex_attributes.end(), verts, verts + amt);
	attributes_changed = true;
	//bFacesDirty = true;
}

//--------------------------------------------------------------
//template<class V>
//void afw_vertex_attribute_base_class<V>::addTriangle(afw_index_type index1, afw_index_type index2, afw_index_type index3) {
//	add_index(index1);
//	add_index(index2);
//	add_index(index3);
//}

//REMOVERS
//--------------------------------------------------------------
template<class VA>
void afw_vertex_attribute_base_class<VA>::remove_vertex_attribute(afw_index_type index) {
	if (index >= vertex_attributes.size()) {
		//ofLogError("ofMesh") << "remove_vertex(): ignoring out of range index " << index << ", number of vertices is" << vertices.size();
	}
	else {
		vertex_attributes.erase(vertex_attributes.begin() + index);
		attributes_changed = true;
		//bFacesDirty = true;
	}
}

template<class VA>
void afw_vertex_attribute_base_class<VA>::remove_vertex__attributes(afw_index_type start_index, afw_index_type end_index) {
	if (start_index >= vertex_attributes.size() || end_index > vertex_attributes.size()) {
		//ofLogError("ofMesh") << "remove_vertex(): ignoring out of range start_index " << start_index << " end_index " << end_index << ", number of vertices is" << vertices.size();
	}
	else {
		vertex_attributes.erase(vertex_attributes.begin() + start_index, vertex_attributes.begin() + end_index);
		vertex_attributes = true;
		//bFacesDirty = true;
	}
}

//GETTERS

//--------------------------------------------------------------
//template<class V>
//V afw_vertex_attribute_base_class<V>::getCentroid() const {
//	if (vertices.size() == 0) {
//		//ofLogWarning("ofMesh") << "getCentroid(): mesh has no vertices, returning glm::vec3(0, 0, 0)";
//		return glm::vec3(0, 0, 0);
//	}
//
//	V sum;
//	for (afw_index_type i = 0; i < vertices.size(); i++) {
//		sum += vertices[i];
//	}
//	sum /= vertices.size();
//	return sum;
//}

//SETTERS

//--------------------------------------------------------------
//template<class V>
//void afw_vertex_attribute_base_class<V>::set_draw_mode(draw_mode_enum m) {
//	draw_mode = m;
//}

//--------------------------------------------------------------
template<class VA>
void afw_vertex_attribute_base_class<VA>::set_vertex_attribute(afw_index_type index, const VA &v) {
	vertex_attributes[index] = v;
	attributes_changed = true;
}



//--------------------------------------------------------------
//template<class VA>
//void afw_vertex_attribute_base_class<VA>::clear_vertex_attributes() {
//	vertex_attributes.clear();
//	attributes_changed = true;
//}

/*
//--------------------------------------------------------------
template<class V>
void afw_vertex_attribute_base_class<V>::mergeDuplicate_vertices() {

	std::vector<V> verts = get_vertices();
	//std::vector<afw_index_type> indices = get_indices();

	//get indexes to share single point - TODO: try j < i
	for (afw_index_type i = 0; i < indices.size(); i++) {
		for (afw_index_type j = 0; j < indices.size(); j++) {
			if (i == j) continue;

			afw_index_type i1 = indices[i];
			afw_index_type i2 = indices[j];
			const V& v1 = verts[i1];
			const V& v2 = verts[i2];

			//if (v1 == v2 && i1 != i2) {
			//	indices[j] = i1;
			//	break;
			//}
		}
	}

	//indices array now has list of unique points we need
	//but we need to delete the old points we're not using and that means the index values will change
	//so we are going to create a new list of points and new indexes - we will use a map to map old index values to the new ones
	std::vector <V> newPoints;
	//std::vector <afw_index_type> new_indexes;
	std::unordered_map <afw_index_type, bool> ptCreated;
	std::unordered_map <afw_index_type, afw_index_type> old_indexNew_index;

	//std::vector<ofFloat_color> new_colors;
	//std::vector<ofFloat_color>& colors = get_colors();
	//std::vector<T> newTCoords;
	//std::vector<T>& tcoords = getTexCoords();
	//std::vector<N> new_normals;
	//std::vector<N>& normals = get_normals();

	for (afw_index_type i = 0; i < indices.size(); i++) {
		ptCreated[i] = false;
	}

	for (afw_index_type i = 0; i < indices.size(); i++) {
		//afw_index_type index = indices[i];
		const auto& p = verts[index];

		if (ptCreated[index] == false) {
			old_indexNew_index[index] = newPoints.size();
			newPoints.push_back(p);
			//if (has_colors()) {
			//	new_colors.push_back(colors[index]);
			//}
			//if (hasTexCoords()) {
			//	newTCoords.push_back(tcoords[index]);
			//}
			//if (has_normals()) {
			//	new_normals.push_back(normals[index]);
			//}

			ptCreated[index] = true;
		}

		//ofLogNotice("ofMesh") << "[" << i << "]: old " << index << " --> " << old_indexNew_index[index];
		//new_indexes.push_back(old_indexNew_index[index]);
	}

	verts.clear();
	verts = newPoints;

	//indices.clear();
	//indices = new_indexes;

	//clear_indices();
	//add_indices(indices);
	clear_vertices();
	add_vertices(verts);

	//if (has_colors()) {
	//	clear_colors();
	//	add_colors(new_colors);
	//}

	//if (hasTexCoords()) {
	//	clearTexCoords();
	//	addTexCoords(newTCoords);
	//}

	//if (has_normals()) {
	//	clear_normals();
	//	add_normals(new_normals);
	//}

}
*/