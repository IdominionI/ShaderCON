#pragma once

#include <vector>

#include "FrameWork/AFW/Common/afw_definitions.h"

//template<class V, class N, class C, class T>
template<class VA>
class afw_vertex_attribute_base_class {
public:
    //using VertexType   = VA;

	afw_vertex_attribute_base_class();
	//afw_vertex_attribute_base_class(draw_mode_enum draw_mode, std::vector<V>& vertices);
	afw_vertex_attribute_base_class(const std::vector<VA> &vertex_attributes);

	virtual ~afw_vertex_attribute_base_class() {}

	//void set_draw_mode(draw_mode_enum m_draw_mode);

	//draw_mode_enum get_draw_mode() { return draw_mode; }


	void add_vertex_attribute(const VA &attribute);
	void add_vertex_attributes(const std::vector<VA> &attributes);
	void add_vertex_attributes(const VA &attributes, std::size_t amt);

	void remove_vertex_attribute(afw_index_type index);
	void remove_vertex__attributes(afw_index_type startIndex, afw_index_type endIndex);

	void set_vertex_attribute(afw_index_type index, const VA &v);

	std::size_t get_number_of_vertex_attributes() {return vertex_attributes.size();}
	VA* get_vertex_attribute_data() { return vertex_attributes.data();}
	VA get_vertex_attribute(afw_index_type i) { return vertex_attributes[i]; }
	//std::vector<VA> get_vertex_attributes{ return vertex_attributes;} // simply does not work and creates compile errors 
																		//	but no error message to indicate as such

	bool has_vertex_attribute() { return !vertex_attributes.empty(); }
	// For some reason compilor would not recognise these functions in the .cpp file !!!!
	bool have_vertex_attributes_changed();
	//{
	//	if (!vertex_attributes.empty()) {
	//		attributes_changed = true;
	//		vertex_attributes.clear();
	//	}
	//}

	void delete_vertex_attributes();
	//{
	//	if (attributes_changed) {
	//		attributes_changed = false;
	//		return true;
	//	}
	//	else {
	//		return false;
	//	}
	//}

	//void mergeDuplicate_vertices(); // to be modified and defined as a virtual function 
	

	//void delete_vertex_attribute_data() { vertex_attributes.clear() };

protected:
	//draw_mode_enum draw_mode;

	bool attributes_changed;

	std::vector<VA> vertex_attributes;

private:

};