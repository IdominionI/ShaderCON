#pragma once

#include <vector>

#include "FrameWork/AFW/Common/afw_definitions.h"

class afw_vertex_indices_base_class {
public:
	afw_vertex_indices_base_class();
	afw_vertex_indices_base_class(std::vector<afw_index_type>& indices);

	virtual ~afw_vertex_indices_base_class() {}

	//--------------------------------------------------------------
	//				 Vertex indice functions
	//--------------------------------------------------------------
	void set_index(afw_index_type index, afw_index_type  val);

	void add_index(afw_index_type i);
	void add_indices(const std::vector<afw_index_type>& inds);
	void add_indices(const afw_index_type* inds, std::size_t amt);

	void remove_index(afw_index_type index);
	void remove_indices(afw_index_type start_index, afw_index_type end_index);

	afw_index_type get_indice(afw_index_type i) {return indices[i];}

	std::vector<afw_index_type>& get_indices() { return indices; }
	afw_index_type* get_index_data_pointer() { return indices.data(); }
	std::size_t get_number_of_indices() { return indices.size(); }

	bool has_indices() { return !indices.empty(); }

	bool have_indicesChanged();

	void clear_indices();

	void enable_indices() { use_indices = true; }
	void disable_indices() { use_indices = false; }
	bool using_indices() { return use_indices; }
	//--------------------------------------------------------------
protected:
	bool indices_changed;
	bool use_indices;

	std::vector<afw_index_type> indices;

private:
};