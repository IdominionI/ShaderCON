#pragma once

#include "sfw_shader_base.h"


#define GLSL_LAYOUT_NO_ERROR              0
#define GLSL_LAYOUT_NOT_FOUND_ERROR       1
#define GLSL_LAYOUT_DATA_TYPE_MATCH_ERROR 2
#define GLSL_LAYOUT_DATA_COPY_ERROR       3

//typedef std::vector<glm::vec3>     g_points_type;
//typedef std::vector<glm::vec3>	   g_line_strip_type;
//
//typedef std::vector<triangle_type> g_triangle_type;
//typedef std::vector<glm::vec3>	   g_triangle_strip_type;

 // to be added when required code to implement 
 // line_strip, line_loop, triangle_strip, triangle_fan
 // lines_adjacency, line_strip_adjacenncy, triangle_adjacency, triangle_strip_adjacenncy
enum class geom_layout_data_in_enum { undefined, points,lines, triangles};

enum class geom_layout_data_out_enum { undefined, points,line_strip, triangle_strip};

typedef std::vector<gl_PerVertex> g_vertex;
typedef std::vector<g_vertex>	  g_primitive;

typedef std::vector<glm::vec4> g_colors;
typedef std::vector<g_colors>  g_primitive_colors;

struct glayout_in {
	glayout_in(geom_layout_data_in_enum gdt) { data_type = gdt; }

	shader_stream_type_enum  stream_type = shader_stream_type_enum::in;
	geom_layout_data_in_enum data_type   = geom_layout_data_in_enum::undefined;

	std::vector<g_vertex> data = {};
	// or should this be ????
	//std::vector<g_primitive> data = {};
};

struct glayout_out {
	glayout_out(geom_layout_data_out_enum gdt, unsigned int ndv) {
		data_type = gdt;
		max_data_values = ndv;
	}

	shader_stream_type_enum stream_type = shader_stream_type_enum::out;
	geom_layout_data_out_enum   data_type   = geom_layout_data_out_enum::undefined;
	unsigned int max_data_values = 0;

	std::vector<g_primitive> data = {};
};

// ----------------------------------------------------------

class sconfw_geometry_shader_base : public sconfw_shader_base
{
public:
	sconfw_geometry_shader_base() {}
	~sconfw_geometry_shader_base() {}
	
	void define_gl_in(std::vector<gl_PerVertex> &vgl_out, std::vector<glm::vec4> &vcolor_out) {
		gl_in    = vgl_out;
		color_in = vcolor_out;
	}

	bool first_initialisation = true;
	void define_in_layouts(std::vector<vlayout_base_struct*> output_layouts) {
		int layout_error = GLSL_LAYOUT_NO_ERROR;

		for (vlayout_base_struct* o_layout : output_layouts) {
			layout_error = GLSL_LAYOUT_NOT_FOUND_ERROR;
			for (vlayout_base_struct* i_layout : input_layouts) {
				if (i_layout->layout_name == o_layout->layout_name) {
					if (i_layout->data_type != o_layout->data_type) {
						layout_error = GLSL_LAYOUT_DATA_TYPE_MATCH_ERROR;
						if (first_initialisation) {
							printf("GLSL_LAYOUT_DATA_TYPE_MATCH_ERROR : %i : %i\n", o_layout->data_type, i_layout->data_type);
						}
					}
					else {
						layout_error = GLSL_LAYOUT_NO_ERROR;
						if (first_initialisation) {
							printf("GLSL_LAYOUT_NO_ERROR : %s\n", o_layout->layout_name.c_str());
						}
						define_input_data(i_layout, o_layout);
					}
				}
			}

			if (layout_error == GLSL_LAYOUT_NOT_FOUND_ERROR && first_initialisation)
				printf("GLSL_LAYOUT_NOT_FOUND_ERROR : %s\n", o_layout->layout_name.c_str());
		}
		first_initialisation = false;
	}

	void define_input_data(vlayout_base_struct* i_layout, vlayout_base_struct* o_layout) {
		switch (i_layout->data_type) {
			case GLSL_DT_VEC4: {
				std::vector<glm::vec4>* in_olayou_data = (std::vector<glm::vec4>*)(o_layout->get_data_vector());
				//printf("define_input_data AAA : %i \n", in_odata->size());

				std::vector<glm::vec4>* in_ilayout_data = (std::vector<glm::vec4>*)(i_layout->get_data_vector());
				//printf("define_input_data BBB : %i \n", in_idata->size());

				in_ilayout_data->clear();

				for (size_t i = 0; i < in_olayou_data->size(); i++) {
					void* odata_element = o_layout->get_data_element(i);
					glm::vec4 data_element = *(glm::vec4*)(odata_element);
					in_ilayout_data->push_back(data_element);
				}

				break;
			}

						 // ########## ADD OTHERS HERE AS REQUIRED #################
		}
	}

protected:
	g_vertex     vertices_out;
	g_primitive  primitive_out;

	void EmitVertex() {
		gl_PerVertex vertex;
		vertex.gl_Position  = gl_Position;
		vertex.gl_PointSize = gl_PointSize;
		vertices_out.push_back(vertex);
	}

	void EndPrimitive() {
		primitive_out.push_back(vertices_out);
		vertices_out.clear();
	}

	// +++++++++++++++++++
	void initialise_in_data(glayout_in& in_data) {
		in_data.data.clear();
		in_data.data.shrink_to_fit();

		switch (in_data.data_type) {
			case geom_layout_data_in_enum::points: {
				g_vertex points;

				for (gl_PerVertex vertex : gl_in) {
					points.push_back(vertex);
				}

				in_data.data.push_back(points);

				break;
			}

		 // ########## ADD OTHER DATA TYPES HERE AS REQUIRED #################
		}
	}

	void create_out_data(glayout_out& out_data) {
		gl_out.clear();
		gl_out.shrink_to_fit();

		switch (out_data.data_type) {
			case geom_layout_data_out_enum::points         : create_points_out(out_data); break;
			case geom_layout_data_out_enum::line_strip     : create_lines_out(out_data); break;
			case geom_layout_data_out_enum::triangle_strip : create_triangles_out(out_data); break;
		}


		//printf("create_out_data 000 : %i\n", gl_out.size());
		//printf("create_out_data 111 : %i\n", colors_out.data.size());

				//create_colors_out();
	}

	void create_points_out(glayout_out& out_data) {
		for (size_t i = 0; i < out_data.data.size(); i++) {
			for (size_t j = 0; j < out_data.max_data_values && j < out_data.data[i].size(); j++) {
				for (size_t k = 0; k < out_data.data[i][j].size(); k++) {
					gl_out.push_back(out_data.data[i][j][k]);
				}
			}
		}
	}

	void create_lines_out(glayout_out& out_data) {
		for (size_t i = 0; i < out_data.data.size(); i++) {
			for (size_t j = 0; j < out_data.max_data_values && j < out_data.data[i].size(); j++) {
				for (size_t k = 0; k < out_data.data[i][j].size(); k++) {
					gl_out.push_back(out_data.data[i][j][k]);
				}
			}
		}

//printf("create_line_strip_out %i\n", gl_out.size());
	}

	void create_triangles_out(glayout_out& out_data) {
		for (size_t i = 0; i < out_data.data.size(); i++) {
			for (size_t j = 0; j < out_data.max_data_values && j < out_data.data[i].size(); j++) {
				for (size_t k = 0; k < out_data.data[i][j].size(); k++) {
					gl_out.push_back(out_data.data[i][j][k]);
				}
			}
		}

//printf("create_triangle_strip_out %i\n", gl_out.size());
	}

	//void create_colors_out() {


		//for (size_t i = 0; i < out_data.data.size(); i++) {
		//	for (size_t j = 0; j < out_data.max_data_values && j < out_data.data[i].size(); j++) {
		//		for (size_t k = 0; k < out_data.data[i][j].size(); k++) {
		//			gl_out.push_back(out_data.data[i][j][k]);
		//		}
		//	}
		//}
	//}


	size_t get_number_of_vertices(glayout_in in_data) {
		return in_data.data.size();
	}
	// ++++++++++++++++++++++


private:

};

