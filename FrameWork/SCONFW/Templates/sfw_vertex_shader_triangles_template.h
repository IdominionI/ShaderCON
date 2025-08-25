#pragma once

#include <Framework/SCONFW/Shader/sfw_vertex_shader_base.h>

#include <FrameWork/SCONFW/Process/sfw_buffer_data_input.h> // Replace this with user defined sconfw_buffer_data_input_base_class derived class if needed

class sfw_vertex_shader_triangles : public sconfw_vertex_shader_base
{
public:
	sfw_vertex_shader_triangles() {}

	~sfw_vertex_shader_triangles() {}

	void initialise(unsigned int v, std::vector<glm::vec3>& vertex_data) {
		glsl_version = v;
		//vertex.data.clear();
		//vertex.data.shrink_to_fit();
		position.data = vertex_data;

		gl_in = {};
		gl_out = {};
		// vvvvvvvvvvvvvv Add functions to query iput data here vvvvvvvvvvvvvvvvvvvvvvvvv

		add_input_layout(&position);

		//add_output_layout(&overtex);
		add_output_layout(&colors);

		add_uniform(&modelViewProjectionMatrix);
		add_uniform(&min_height);
		add_uniform(&max_height);
		add_uniform(&display_as_points);

		//add_variable(&test_var);

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}




	// vvvvvvvvvvvvvvv BEGIN GLSL EMULATION vvvvvvvvvvvvvvvvvvv This line must not be deleted or modified
	//must have as a min or something that defines the input data
	// ---------------------------- Shader in/out Layout definitions ------------------------------
	vlayout_vec3 position = vlayout_vec3(0, shader_stream_type_enum::in, GLSL_DT_VEC3, "position"); // The only way to initialise the construct
	//vlayout_vec3 overtex = vlayout_vec3(0, shader_stream_type_enum::out, GLSL_DT_VEC3,"overtex"); // The only way to initialise the construct// ++++++

	vlayout_vec4 colors = vlayout_vec4(0, shader_stream_type_enum::out, GLSL_DT_VEC4, "colors");
	// ---------------------------------------------------------------------------------------------
 
	// ---------------------------- Shader uniform definitions -----------------------------
	uniform_mat4 modelViewProjectionMatrix = uniform_mat4(GLSL_DT_MAT4, "modelViewProjectionMatrix");

	// -------------- User Defined Uniforms ----------------------

	uniform_float min_height = uniform_float(GLSL_DT_FLOAT, "min_height");
	uniform_float max_height = uniform_float(GLSL_DT_FLOAT, "max_height");

	uniform_int display_as_points = uniform_int(GLSL_DT_FLOAT, "display_as_points"); ;
	// --------------------------------------------------------------------------------------------
	
	// ----------------------------- Shader variable definitions ----------------------------

	//float min_height      = -5.0;
	//float max_height      = 5.0;
	glm::vec3 vertex;
	glm::vec4 light_color     = glm::vec4(1.0,0.0,0.0,1.0);
	//vec4 raw_color       = vec4(1.0,0.0,0.0,1.0);
	glm::vec4 raw_color;

	float x;
	float y;
	float z;

	float r;
	float g;
	float b;

	float height;

	// ---------------------------------------------------------------------------------------------


	// ========================= DEFINE SHADER FUNCTIONS HERE =========================

	glm::vec4  map_height_color() {
		height = 2.0;

		if (vertex.z >= min_height.value && vertex.z <= max_height.value)
			height = (vertex.z - min_height.value) / (max_height.value - min_height.value);


		// RGB color cube color space along cube edges 
		if (height <= 0.1666667) {
			r = (0.1666667f - height) / 0.1666667f;
			g = 0.0f;
			b = 1.0f;
		}
		else {
			if (height > 0.1666667 && height <= 0.3333333) {
				r = 0.0;
				g = (height - 0.1666667f) / 0.1666667f;
				b = 1.0;
			}
			else {
				if (height > 0.3333333 && height <= 0.5) {
					b = (0.5f - height) / 0.1666667f;
					r = 0.0f;
					g = 1.0f;
				}
				else {
					if (height > 0.5 && height <= 0.6666667) {
						b = 0.0f;
						r = (height - 0.5f) / 0.1666667f;
						g = 1.0f;
					}
					else {
						if (height > 0.6666667 && height <= 0.8333333) {
							b = 0.0f;
							r = 1.0f;
							g = (0.8333333f - height) / 0.1666667f;
						}
						else {
							if (height <= 1.0) {
								r = 1.0f;
								b = (height - 0.8333333f) / 0.1666667f;
								g = (height - 0.8333333f) / 0.1666667f;
							}
							else {
								r = 0.5f;
								g = 0.5f;
								b = 0.5f;
							}
						}
					}
				}
			}
		}
		return glm::vec4(r, g, b, 1.0f);
		//     varyingColor = vec4(r,g,b,1.0);
	}


	// ================================================================================

	// all glsl main code emulation  must go into scon_main_code()
	void scon_main_code(size_t i) override {
//printf("gl_position before  %i : %f : %f : %f \n", i,vertex.data[i].x,vertex.data[i].y, vertex.data[i].z);
		if (display_as_points.value == 0) { // do not display as points
			gl_Position = glm::vec4(position.data[i], 1.0f); // required as a min to pass unprocessed camera projection vertex data
		}
		else {// diaplay as points
			gl_Position = modelViewProjectionMatrix.value * glm::vec4(position.data[i], 1.0f); // required as a min
		}

		vertex = position.data[i];
		raw_color = map_height_color();// required as a min not working properly for unknown reason need to fix up

		//gl_color = glm::vec4(0.5f, 1.0f, 1.0f, 1.0f);
		gl_color = raw_color;

		colors.data[i] = gl_color;

//printf("gl_position after %i : %f : %f : %f \n",i, gl_Position.x, gl_Position.y, gl_Position.z);
	}

	// ^^^^^^^^^^^^^^^^ END GLSL EMULATIOMN ^^^^^^^^^^^^^^^^^^^^ This line must not be deleted or modified

	void scon_main() override {
		number_vertices = unsigned int(get_number_of_vertices());// Modify this if needed

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
			gl_out[i].gl_Position = gl_Position;
			gl_out[i].gl_PointSize = gl_PointSize;
			color_out[i] = gl_color;
		}

		// vvvvvvvvvvvvvv Add functions to query output data here vvvvvvvvvvvvvvvvvvvvvvvvv



		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}

	size_t get_number_of_vertices() {
		return position.data.size(); // This must be modified if name or structure of input vertex position data is different
	}

protected:

};
