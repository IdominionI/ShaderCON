#pragma once

#include <Framework/SCONFW/Shader/sfw_geometry_shader_base.h>

class sfw_geometry_shader_line : public sconfw_geometry_shader_base
{
public:
	sfw_geometry_shader_line() {}
	~sfw_geometry_shader_line() {}

	void initialise(unsigned int v) {
		glsl_version = v;

		// vvvvvvvvvvvvvv Add functions to query iput data here vvvvvvvvvvvvvvvvvvvvvvvvv
		// ---------------------------- Shader in/out Layout definitions ------------------------------
		add_input_layout(&colors);
		add_output_layout(&colors_out);

		// ---------------------------- Shader uniform definitions -----------------------------
		//add_uniform(&mvpMatrix);
		add_uniform(&u_view);
		add_uniform(&u_projection);

		add_uniform(&camera_position);
		add_uniform(&camera_lookat);

		add_uniform(&xy_grid_color);
		add_uniform(&xz_grid_color);
		add_uniform(&yz_grid_color);

		add_uniform(&display_xy_plane);
		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}

	// vvvvvvvvvvvvvvv BEGIN GLSL EMULATION vvvvvvvvvvvvvvvvvvv This line must not be deleted or modified
	
	// ---------------------------- Shader in/out Layout definitions ------------------------------
	glayout_in  in_data  = glayout_in(geom_layout_data_in_enum::points);    // Change this as needed
	//glayout_out out_data = glayout_out(geom_layout_data_enum::points, 1); // Change this as needed
	glayout_out out_data = glayout_out(geom_layout_data_out_enum::line_strip, 12); // Change this as needed

	vlayout_vec4 colors = vlayout_vec4(0, shader_stream_type_enum::in, GLSL_DT_VEC4, "colors");
	vlayout_vec4 colors_out = vlayout_vec4(0, shader_stream_type_enum::out, GLSL_DT_VEC4, "colors");
	// ---------------------------------------------------------------------------------------------

	// ---------------------------- Shader uniform definitions -----------------------------
	//uniform_mat4 mvpMatrix = uniform_mat4(GLSL_DT_MAT4, "mvpMatrix");
	uniform_mat4 u_view       = uniform_mat4(GLSL_DT_MAT4, "u_view");
	uniform_mat4 u_projection = uniform_mat4(GLSL_DT_MAT4, "u_projection");
	
	uniform_vec3 camera_position = uniform_vec3(GLSL_DT_VEC3, "camera_position");
	uniform_vec3 camera_lookat = uniform_vec3(GLSL_DT_VEC3, "camera_lookat");

	uniform_vec4 xy_grid_color   = uniform_vec4(GLSL_DT_VEC4, "xy_grid_color");
	uniform_vec4 xz_grid_color   = uniform_vec4(GLSL_DT_VEC4, "xz_grid_color");
	uniform_vec4 yz_grid_color   = uniform_vec4(GLSL_DT_VEC4, "yz_grid_color");

	uniform_int display_xy_plane = uniform_int(GLSL_DT_INT, "display_xy_plane");
	uniform_int display_xz_plane = uniform_int(GLSL_DT_INT, "display_xz_plane");
	uniform_int display_yz_plane = uniform_int(GLSL_DT_INT, "display_yz_plane");

	//uniform_vec3 grid_center_location = uniform_vec3(GLSL_DT_VEC3, "grid_center_location");

	// ---------------------------------------------------------------------------------------------

	// ----------------------------- Shader variable definitions ----------------------------
	int   x_level = 0, y_level = 0, z_level = 0;

	float x_grid_interval = 1.0f;
	float y_grid_interval = 1.0f;
	float z_grid_interval = 1.0f;

	float major_xalpha = 1.0f, minor_xalpha = 0.0f;
	float major_yalpha = 1.0f, minor_yalpha = 0.0f;
	float major_zalpha = 1.0f, minor_zalpha = 0.0f;

	float cost_log10 = 1 / log(10);

	glm::vec3 grid_center_location = glm::vec3{ 0.0f,0.0f,0.0f };

	// ---------------------------------------------------------------------------------------------

	// ---  Define function variables that want to be displayed in shader variable display panel----



	// ========================= DEFINE SHADER FUNCTIONS HERE =========================
	float log_10(float x) { // OpenGl does not have this math function !!!!
		//return cost_log10.value * log(x);
		return cost_log10 * log(x);
	}

	float ax_level;
	int cx_level;

	float define_minor_grid_alpha(float minor_alpha) {
		// Simple linear fade gradient
		if (minor_alpha > 0.25f && minor_alpha < 0.5f)
			minor_alpha = 0.5f - (0.5f - minor_alpha)*0.4f;
		else
			if (minor_alpha > 0.1f && minor_alpha < 0.25f)
				minor_alpha = 0.4f - (.25f - minor_alpha)*1.0f;
			else
				if (minor_alpha > 0.0f && minor_alpha < 0.1f)
					minor_alpha = 0.25f - (0.1f - minor_alpha) * 0.04f;

		return minor_alpha;
	}

	void define_grid_intervals() {
		// Grid center algorithm
		ax_level = log_10(glm::length(camera_position.value - grid_center_location));
		float ay_level = ax_level;
		float az_level = ax_level;

		// Perpedicular algorithm
		//float ax_level = log_10(abs(float(camera_position.value.z - grid_center_location.z)));
		//float ay_level = log_10(abs(float(camera_position.value.x - grid_center_location.x)));
		//float az_level = log_10(abs(float(camera_position.value.y - grid_center_location.y)));

		cx_level = int(ax_level);
		int cy_level = int(ay_level);
		int cz_level = int(az_level);

		//int max_level = std::max(cx_level, std::max(cy_level, cz_level));
		int max_level = cx_level;

		x_grid_interval = pow(10, max_level);
		y_grid_interval = pow(10, max_level);
		z_grid_interval = pow(10, max_level);

		// Grid center algorithm
		minor_xalpha = 1.0f - abs(glm::length(camera_position.value - grid_center_location))/ pow(10.0f, float(cx_level+1));

		// Perpedicular algorithm
		//minor_xalpha = 1.0f - abs(float(camera_position.value.z - grid_center_location.z)) / pow(10.0f, float(cx_level + 1));
		//minor_yalpha = 1.0f - abs(float(camera_position.value.x - grid_center_location.x)) / pow(10.0f, float(cy_level + 1));
		//minor_zalpha = 1.0f - abs(float(camera_position.value.y - grid_center_location.y)) / pow(10.0f, float(cz_level + 1));

		minor_xalpha = define_minor_grid_alpha(minor_xalpha);
		minor_yalpha = minor_xalpha;
		minor_zalpha = minor_xalpha;

		//major_xalpha = 1.0f - minor_xalpha;
		major_xalpha = 1.0f;
		major_yalpha = major_xalpha;
		major_zalpha = major_xalpha;


	}

	// ================================================================================
	// all glsl main code must go into scon_main_code()
	void scon_main_code(size_t i, size_t j) override {
		//ImGui::Begin("Geometry Variables");


//printf("scon_main_code 000 : %i \n", colors.data.size());
		// data element must be present for each output color definition for each vertex
		// Ths is not a glsl expression but a emulation of colors_out = colors.data[i];
		//colors_out.data_element = colors.data[j];
		//colors_out.data_element = xy_grid_color.value;

		glm::mat4 mvpMatrix = u_projection.value * u_view.value;

		glm::vec4 x_center = glm::vec4(in_data.data[i][j].gl_Position.x * 10, 0.0, 0.0, 1.0);
		glm::vec4 y_center = glm::vec4(0.0, in_data.data[i][j].gl_Position.x * 10, 0.0, 1.0);
		glm::vec4 z_center = glm::vec4(0.0, 0.0, in_data.data[i][j].gl_Position.x * 10, 1.0);

		define_grid_intervals();


		// ****************** X-Y PLANE *******************
		if (display_xy_plane.value != 0) {
			glm::vec4 xy_xcenter = x_center;
			glm::vec4 xy_ycenter = y_center;

			xy_xcenter.x = x_center.x * z_grid_interval;
			xy_ycenter.y = y_center.y * z_grid_interval;


			float a = abs(xy_xcenter.x - grid_center_location.x) / (z_grid_interval * 10.0f);
			if (abs(a-floor(a)) < .00001f) {
				xy_grid_color.value.a   = major_xalpha;
			}
			else {
				xy_grid_color.value.a   = minor_xalpha;
			}

			colors_out.data_element = xy_grid_color.value;

			// add_data_element must be present for each ouput color definition for each vertex
		    // This is not a glsl expression but a hidden method in glsl
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xy_xcenter + glm::vec4(0.0, -50.0f * z_grid_interval, grid_center_location.z, 1.0));
			EmitVertex();

			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xy_xcenter + glm::vec4(0.0, 50.0f * z_grid_interval, grid_center_location.z, 1.0));
			EmitVertex();

			EndPrimitive();

			// ######### DISPLAY ALL VARIABLES THAT HAVE A VERTEX INDEX BETWEEN THE FROM AND TO ROW VALUES #######
			// ######### USING ImGui Statements #######
			if (j >= from_row && j <= to_row) { // must be present

				ImGui::Text("Vertex %i", j);
				ImGui::SameLine();
				ImGui::Text("a : %9f3 ::", a);
				ImGui::SameLine();
				ImGui::Text("abs : %9f3 :  ", abs(a - floor(a)));
//				ImGui::SameLine();
				//ImGui::Text("1 : %9f3 :  ", xy_xcenter.x);
				//ImGui::SameLine();
				//ImGui::Text("2 : %9f3 :  ", grid_center_location.x);
				//ImGui::SameLine();
				//ImGui::Text("3 : %9f3 :  ", z_grid_interval);
				//ImGui::SameLine();
				//ImGui::Text("xy_ycenter : %9f3 :  ", a);

			} // must be present


			//colors_out.data_element = xy_grid_color.value;
			//colors_out.data_element = glm::vec4(0.0f, 0.0f, 1.0f, minor_xalpha);
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xy_ycenter + glm::vec4(-50.0f * z_grid_interval, 0.0, grid_center_location.z, 1.0));
			EmitVertex();

			//colors_out.data_element = xy_grid_color.value;
			//colors_out.data_element = glm::vec4(0.0f, 0.0f, 1.0f, minor_xalpha);
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xy_ycenter + glm::vec4(50.0f * z_grid_interval, 0.0, grid_center_location.z, 1.0));
			EmitVertex();

			EndPrimitive();
		}

		// ****************** X-Z PLANE *******************
		if (display_xz_plane.value != 0) {
			glm::vec4 xz_xcenter = x_center;
			glm::vec4 xz_zcenter = z_center;

			xz_xcenter.x = x_center.x * y_grid_interval;
			xz_zcenter.z = z_center.z * y_grid_interval;

			float a = abs(xz_zcenter.z - grid_center_location.z) / (y_grid_interval * 10.0f);
			if (abs(a - floor(a)) < .00001f) {
				xz_grid_color.value.a = major_zalpha;
			}
			else {
				xz_grid_color.value.a = minor_zalpha;
			}

			colors_out.data_element = xz_grid_color.value;

			//varyingColor = xz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xz_xcenter + glm::vec4(0.0, grid_center_location.y, -50.0 * y_grid_interval, 0.0));
			EmitVertex();

			//varyingColor = xz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xz_xcenter + glm::vec4(0.0, grid_center_location.y, 50.0 * y_grid_interval, 0.0));
			EmitVertex();

			EndPrimitive();

			//varyingColor = xz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xz_zcenter + glm::vec4(-50.0 * y_grid_interval, grid_center_location.y, 0.0, 0.0));
			EmitVertex();

			//varyingColor = xz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (xz_zcenter + glm::vec4(50.0 * y_grid_interval, grid_center_location.y, 0.0, 0.0));
			EmitVertex();

			EndPrimitive();
		}

		// ****************** Y-Z PLANE *******************
		if (display_yz_plane.value != 0) {
			glm::vec4 yz_ycenter = y_center;
			glm::vec4 yz_zcenter = z_center;

			yz_ycenter.y = y_center.y * x_grid_interval;
			yz_zcenter.z = z_center.z * x_grid_interval;

			float a = abs(yz_ycenter.y - grid_center_location.y) / (x_grid_interval * 10.0f);
			if (abs(a - floor(a)) < .00001f) {
				yz_grid_color.value.a = major_yalpha;
			}
			else {
				yz_grid_color.value.a = minor_yalpha;
			}

			colors_out.data_element = yz_grid_color.value;

			//varyingColor = yz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (yz_zcenter + glm::vec4(grid_center_location.x, -50.0 * x_grid_interval, 0.0, 0.0));
			EmitVertex();

			//varyingColor = yz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (yz_zcenter + glm::vec4(grid_center_location.x, 50.0 * x_grid_interval, 0.0, 0.0));
			EmitVertex();

			EndPrimitive();

			//varyingColor = yz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (yz_ycenter + glm::vec4(grid_center_location.x, 0.0, -50.0 * x_grid_interval, 0.0));
			EmitVertex();

			//varyingColor = yz_grid_color;
			colors_out.add_data_element();
			gl_Position = mvpMatrix * (yz_ycenter + glm::vec4(grid_center_location.x, 0.0, 50.0 * x_grid_interval, 0.0));
			EmitVertex();

			EndPrimitive();
		}

//printf("scon_main_code 002 \n");
		// add_data_element must be present for each ouput color definition for each vertex
		// This is not a glsl expression but a hidden method in glsl
		//colors_out.add_data_element();
//printf("scon_main_code 003 \n");
		//gl_Position = in_data.data[i][j].gl_Position; // required as a min
//printf("scon_main_code 004 \n");
		//EmitVertex();
//printf("scon_main_code 005 \n");
		//EndPrimitive();

		//ImGui::End();
		//ImGui::End;
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

		if (number_vertices < 1) {
			// error message here
			return;
		}

		clear_buffer_out_data();

		out_data.data.clear();
		out_data.data.shrink_to_fit();

		colors_out.data.clear();
		colors_out.data.shrink_to_fit();


//printf("scon_main 004 \n");

		ImGui::Begin("Geometry Variables"); // MUST BE PRESENT

		variable_display_control(in_data.data[0].size()); // MUST BE PRESENT TO ENABLE VARIABLE DISPLAY CONTROLS

		for (size_t i = 0; i < in_data.data.size(); i++) {
//printf("scon_main 004A : %i\n", in_data.data[i].size());
			for (size_t j = 0; j < in_data.data[i].size(); j++) {

				scon_main_code(i, j);
				out_data.data.push_back(primitive_out);
				primitive_out.clear();
			}
		}

		ImGui::End(); // MUST BE PRESENT

//printf("scon_main 005 %i\n", out_data.data.size());
		create_out_data(out_data);
	}

protected:
	
};
