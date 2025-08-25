#pragma once

#include <Framework/SCONFW/Shader/sfw_vertex_shader_base.h>

#include <FrameWork/SCONFW/Process/sfw_buffer_data_input.h> // Replace this with user defined sconfw_buffer_data_input_base_class derived class if needed

class sfw_vertex_template_shader_base : public sconfw_vertex_shader_base
{
public:
	sfw_vertex_template_shader_base() {}

	~sfw_vertex_template_shader_base() {}

	void initialise (unsigned int v, std::vector<glm::vec3>& vertex_data) {
		glsl_version = v;
		//vertex.data.clear();
		//vertex.data.shrink_to_fit();
		vertex.data  = vertex_data;

		gl_in  = {};
		gl_out = {};
		// vvvvvvvvvvvvvv Add functions to query iput data here vvvvvvvvvvvvvvvvvvvvvvvvv

		add_input_layout(&vertex);

		//add_output_layout(&overtex);
		add_output_layout(&colors);

		add_uniform(&modelViewProjectionMatrix);

		//add_variable(&test_var);

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}

	// vvvvvvvvvvvvvvv BEGIN GLSL EMULATION vvvvvvvvvvvvvvvvvvv This line must not be deleted or modified
	//must have as a min or something that defines the input data
	//vlayout_vec3 vertex = vlayout_vec3(0, shader_stream_type_enum::in, glsl_data_type_enum::vec3,"vertex"); // The only way to initialise the construct
	vlayout_vec3 vertex = vlayout_vec3(0, shader_stream_type_enum::in, GLSL_DT_VEC3,"vertex"); // The only way to initialise the construct
	//vlayout_vec3 overtex = vlayout_vec3(0, shader_stream_type_enum::out, GLSL_DT_VEC3,"overtex"); // The only way to initialise the construct// ++++++

	vlayout_vec4 colors = vlayout_vec4(0, shader_stream_type_enum::out, GLSL_DT_VEC4, "colors");

	//uniform<glm::mat4> u_view = {};
	//uniform<glm::mat4> modelViewProjectionMatrix = uniform<glm::mat4>(GLSL_DT_MAT4,"modelViewProjectionMatrix");
	uniform_mat4 modelViewProjectionMatrix = uniform_mat4(GLSL_DT_MAT4,"modelViewProjectionMatrix");
	//uniform<glm::mat4> u_projection = {};

	//variable_float test_var = variable_float(GLSL_DT_FLOAT,"test_var");

	// all glsl main code emulation  must go into scon_main_code()
	void scon_main_code(size_t i) override{
//printf("gl_position before  %i : %f : %f : %f \n", i,vertex.data[i].x,vertex.data[i].y, vertex.data[i].z);
		gl_Position = glm::vec4(vertex.data[i], 1.0f); // required as a min to pass unprocessed camera projection vertex data
		//gl_Position = modelViewProjectionMatrix.value * glm::vec4(vertex.data[i], 1.0f); // required as a min to pass processed camera projection vertex data
		gl_color = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);

		//test_var.value = 200.345f;// Test variable only : comment out or delete when not needed

		//colors.data[i] = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);
		colors.data[i] = gl_color;
		//overtex.data[i] = { gl_Position.x,gl_Position.y,gl_Position.z };// ++++++ testing only
		//gl_color = glm::vec4(1.0f*float(i)/ float(number_vertices), 1.0f * float(i)/float(number_vertices), 1.0f * float(i) / float(number_vertices), 1.0f);
//printf("gl_position after %i : %f : %f : %f \n",i, gl_Position.x, gl_Position.y, gl_Position.z);
	}

	// ^^^^^^^^^^^^^^^^ END GLSL EMULATIOMN ^^^^^^^^^^^^^^^^^^^^ This line must not be deleted or modified

	void scon_main() override{
		number_vertices = get_number_of_vertices();// Modify this if needed

		clear_buffer_out_data();

		//overtex.data.clear();// ++++++
		//overtex.data.shrink_to_fit();// ++++++
		//overtex.data.assign(number_vertices, glm::vec3());// ++++++

		colors.data.clear();// ++++++
		colors.data.shrink_to_fit();// ++++++
		colors.data.assign(number_vertices, glm::vec4());// ++++++

		gl_out.assign(number_vertices, gl_PerVertex());
		color_out.assign(number_vertices, gl_color);

		for (size_t i = 0; i < number_vertices; i++) {
			scon_main_code(i);
			gl_out[i].gl_Position  = gl_Position;
			gl_out[i].gl_PointSize = gl_PointSize;
			color_out[i]           = gl_color;
		}

		// vvvvvvvvvvvvvv Add functions to query output data here vvvvvvvvvvvvvvvvvvvvvvvvv

		

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}

	size_t get_number_of_vertices() {
		return vertex.data.size(); // This must be modified if name or structure of input vertex position data is different
	}

protected:

};
