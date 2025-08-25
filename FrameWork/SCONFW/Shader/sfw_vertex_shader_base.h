#pragma once

#include "sfw_shader_base.h"

// Until find need to specify a vertex shader base class, none is defined
class sconfw_vertex_shader_base: public sconfw_shader_base
{
public:
	sconfw_vertex_shader_base() {}
	~sconfw_vertex_shader_base() {}


	//void add_input_layout(vlayout_base_struct *input_layout) {
	//	input_layouts.push_back(input_layout);
	//}

	//void add_output_layout(vlayout_base_struct* output_layout) {
	//	output_layouts.push_back(output_layout);
	//}

//protected:

	//std::vector<vlayout_base_struct *> input_layouts;
	//std::vector<vlayout_base_struct *> output_layouts;
};