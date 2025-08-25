#include "afw_vertex_colors.h"

template<class C>
afw_vertex_colors_base_class<C>::afw_vertex_colors_base_class() {
	colors_changed = false;
	use_colors = true;
}

//--------------------------------------------------------------
template<class C>
bool afw_vertex_colors_base_class<C>::have_colorsChanged() {
	if (colors_changed) {
		colors_changed = false;
		return true;
	}
	else {
		return false;
	}
}


//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::add_color(const C& c) {
	colors.push_back(c);
	colors_changed = true;
	//bFacesDirty = true;
}



//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::add_colors(const std::vector<C>& cols) {
	colors.insert(colors.end(), cols.begin(), cols.end());
	colors_changed = true;
	//bFacesDirty = true;
}



//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::add_colors(const C* cols, std::size_t amt) {
	colors.insert(colors.end(), cols, cols + amt);
	colors_changed = true;
	//bFacesDirty = true;
}

//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::remove_color(afw_index_type index) {
	if (index >= colors.size()) {
		//ofLogError("ofMesh") << "remove_color(): ignoring out of range index " << index << ", number of colors is" << colors.size();
	}
	else {
		colors.erase(colors.begin() + index);
		colors_changed = true;
		//bFacesDirty = true;
	}
}

template<class C>
void afw_vertex_colors_base_class<C>::remove_colors(afw_index_type start_index, afw_index_type end_index) {
	if (start_index >= colors.size() || end_index > colors.size()) {
		//ofLogError("ofMesh") << "remove_color(): ignoring out of range start_index " << start_index << " end_index " << end_index << ", number of colors is" << colors.size();
	}
	else {
		colors.erase(colors.begin() + start_index, colors.begin() + end_index);
		colors_changed = true;
		//bFacesDirty = true;
	}
}

template<class C>
C afw_vertex_colors_base_class<C>::get_color(afw_index_type i) const {
	return colors[i];
}

//--------------------------------------------------------------
template<class C>
std::size_t afw_vertex_colors_base_class<C>::get_number_of_colors() const {
	return colors.size();
}
//--------------------------------------------------------------
template<class C>
C* afw_vertex_colors_base_class<C>::get_colors_pointer() {
	return colors.data();
}

//--------------------------------------------------------------
template<class C>
std::vector<C>& afw_vertex_colors_base_class<C>::get_colors() {
	colors_changed = true;
	//bFacesDirty = true;
	return colors;
}
//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::set_color(afw_index_type index, const C& c) {
	colors[index] = c;
	colors_changed = true;
	//bFacesDirty = true;
}

//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::clear_colors() {
	colors.clear();
	colors_changed = true;
	//bFacesDirty = true;
}

//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::enable_colors() {
	use_colors = true;
}




//--------------------------------------------------------------
template<class C>
void afw_vertex_colors_base_class<C>::disable_colors() {
	use_colors = false;
}



//--------------------------------------------------------------
template<class C>
bool afw_vertex_colors_base_class<C>::using_colors() const {
	return use_colors;
}




