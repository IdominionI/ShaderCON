#pragma once

#include <vector>

#include "FrameWork/AFW/Common/afw_definitions.h"

template<class C>
class afw_vertex_colors_base_class{
public:
	using ColorType = C;

	afw_vertex_colors_base_class();

	virtual ~afw_vertex_colors_base_class(){}

	void set_color(afw_index_type index, const C& c);

	void add_color(const C& c);
	void add_colors(const std::vector<C>& cols);
	void add_colors(const C* cols, std::size_t amt);

	void remove_color(afw_index_type index);
	void remove_colors(afw_index_type start_index, afw_index_type end_index);

	std::vector<C>& get_colors();
	C get_color(afw_index_type i) const;

	C* get_colors_pointer();

	std::size_t get_number_of_colors() const;

	bool have_colorsChanged();

	void clear_colors();

	void enable_colors();
	void disable_colors();
	bool using_colors() const;
protected:
	bool colors_changed;
	bool use_colors;

	std::vector<C> colors;
private:

};

