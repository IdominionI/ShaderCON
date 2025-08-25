#pragma once

#include <Framework/SCONFW/Shader/sfw_geometry_shader_base.h>

class sfw_geometry_template_shader_base : public sconfw_geometry_shader_base
{
public:
	sfw_geometry_template_shader_base() {}
	~sfw_geometry_template_shader_base() {}

	void initialise(unsigned int v) {
		glsl_version = v;

		// vvvvvvvvvvvvvv Add functions to query iput data here vvvvvvvvvvvvvvvvvvvvvvvvv

		add_input_layout(&colors);

		add_output_layout(&colors_out);

		add_uniform(&test_projmatrix);

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}

	// vvvvvvvvvvvvvvv BEGIN GLSL EMULATION vvvvvvvvvvvvvvvvvvv This line must not be deleted or modified
	
	glayout_in  in_data  = glayout_in(geom_layout_data_in_enum::points);    // Change this as needed
	glayout_out out_data = glayout_out(geom_layout_data_out_enum::points,1); // Change this as needed

	vlayout_vec4 colors     = vlayout_vec4(0, shader_stream_type_enum::in, GLSL_DT_VEC4, "colors");
	vlayout_vec4 colors_out = vlayout_vec4(0, shader_stream_type_enum::out, GLSL_DT_VEC4, "colors");

	uniform_mat4 test_projmatrix = uniform_mat4(GLSL_DT_MAT4, "TestProjectionMatrix");
	//uniform<glm::mat4> u_projection = {};

	variable_float test_geom_var = variable_float(GLSL_DT_FLOAT, "test_geom_var");

	// all glsl main code must go into scon_main_code()
	void scon_main_code(size_t i, size_t j) override {
//printf("scon_main_code 000 : %i \n", colors.data.size());
		// data element must be present for each ouput color definition for each vertex
		// Ths is not a glsl expression but a emulation of colors_out = colors.data[i];
		colors_out.data_element = colors.data[j];

//printf("scon_main_code 002 \n");
		// add_data_element must be present for each ouput color definition for each vertex
		// This is not a glsl expression but a hidden method in glsl
		colors_out.add_data_element(); 
//printf("scon_main_code 003 \n");
		gl_Position = in_data.data[i][j].gl_Position; // required as a min
//printf("scon_main_code 004 \n");
		EmitVertex();
//printf("scon_main_code 005 \n");
		EndPrimitive();
	}

	// ^^^^^^^^^^^^^^^^ END GLSL EMULATIOMN ^^^^^^^^^^^^^^^^^^^^ This line must not be deleted or modified

	void scon_main()  override {
//printf("scon_main 000 : %i\n", gl_in.size());

		if (gl_in.empty()) {
			//printf( "Geometry Shader : No input data defined !!\n" );
			return;
		}

//printf("scon_main 001 \n");
		initialise_in_data(in_data);
//printf("scon_main 002 \n");
		number_vertices = get_number_of_vertices(in_data);// Modify this if needed
//printf("scon_main 003 : %i\n", number_vertices);
		clear_buffer_out_data();
		
		out_data.data.clear();
		out_data.data.shrink_to_fit();
		
		colors_out.data.clear();
		colors_out.data.shrink_to_fit();


//printf("scon_main 004 \n");

		//for (size_t i = 0; i < number_vertices; i++) {
		for (size_t i = 0; i < in_data.data.size(); i++) {
//printf("scon_main 004A : %i\n", in_data.data[i].size());
			for (size_t j = 0; j < in_data.data[i].size(); j++) {

				scon_main_code(i,j);
				out_data.data.push_back(primitive_out);
				primitive_out.clear();
			}
		}
//printf("scon_main 005 %i\n", out_data.data.size());
		create_out_data(out_data);
	}

protected:

};