#include "afw_vertex_indices.h"

//--------------------------------------------------------------

afw_vertex_indices_base_class::afw_vertex_indices_base_class() {
	indices_changed = false;
	use_indices = false;
}

//--------------------------------------------------------------

afw_vertex_indices_base_class::afw_vertex_indices_base_class(std::vector<afw_index_type>& indices) {
	add_indices(indices);
	indices_changed = false;
	use_indices = false;
}

//--------------------------------------------------------------

bool afw_vertex_indices_base_class::have_indicesChanged() {
	if (indices_changed) {
		indices_changed = false;
		return true;
	}
	else {
		return false;
	}
}

//--------------------------------------------------------------

void afw_vertex_indices_base_class::add_index(afw_index_type i) {
	indices.push_back(i);
	indices_changed = true;
}

//--------------------------------------------------------------

void afw_vertex_indices_base_class::add_indices(const std::vector<afw_index_type>& inds) {
	indices.insert(indices.end(), inds.begin(), inds.end());
	indices_changed = true;
}

//--------------------------------------------------------------

void afw_vertex_indices_base_class::add_indices(const afw_index_type* inds, std::size_t amt) {
	indices.insert(indices.end(), inds, inds + amt);
	indices_changed = true;
}

//--------------------------------------------------------------

void afw_vertex_indices_base_class::remove_index(afw_index_type index) {
	if (index >= indices.size()) {
		//ofLogError("ofMesh") << "remove_index(): ignoring out of range index " << index << ", number of indices is" << indices.size();
	}
	else {
		indices.erase(indices.begin() + index);
		indices_changed = true;
	}
}

void afw_vertex_indices_base_class::remove_indices(afw_index_type start_index, afw_index_type end_index) {
	if (start_index >= indices.size() || end_index > indices.size()) {
		//ofLogError("ofMesh") << "remove_index(): ignoring out of range start_index " << start_index << " end_index " << end_index << ", number of indices is" << indices.size();;
	}
	else {
		indices.erase(indices.begin() + start_index, indices.begin() + end_index);
		indices_changed = true;
	}
}

//--------------------------------------------------------------
void afw_vertex_indices_base_class::set_index(afw_index_type index, afw_index_type  val) {
	indices[index] = val;
	indices_changed = true;
}

//--------------------------------------------------------------

void afw_vertex_indices_base_class::clear_indices() {
	indices.clear();
	indices_changed = true;
}
