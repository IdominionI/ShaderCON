#pragma once

#include <Framework/SCONFW/Shader/sfw_geometry_shader_base.h>

class sfw_geometry_shader_triangle : public sconfw_geometry_shader_base
{
public:
	sfw_geometry_shader_triangle() {}
	~sfw_geometry_shader_triangle() {}

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

		add_uniform(&uCameraPos);
		add_uniform(&uCamera_forward);
		add_uniform(&uCamera_up);
		add_uniform(&uCamera_right);
		add_uniform(&camera_lookat);

		// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	}

	// vvvvvvvvvvvvvvv BEGIN GLSL EMULATION vvvvvvvvvvvvvvvvvvv This line must not be deleted or modified
	
	// ---------------------------- Shader in/out Layout definitions ------------------------------
	glayout_in  in_data  = glayout_in(geom_layout_data_in_enum::points);    // Change this as needed
	//glayout_out out_data = glayout_out(geom_layout_data_enum::points, 1); // Change this as needed
	glayout_out out_data = glayout_out(geom_layout_data_out_enum::triangle_strip, 72); // Change this as needed

	vlayout_vec4 colors = vlayout_vec4(0, shader_stream_type_enum::in, GLSL_DT_VEC4, "colors");
	vlayout_vec4 colors_out = vlayout_vec4(0, shader_stream_type_enum::out, GLSL_DT_VEC4, "colors");
	// ---------------------------------------------------------------------------------------------

	// ---------------------------- Shader uniform definitions -----------------------------
	//uniform_mat4 mvpMatrix = uniform_mat4(GLSL_DT_MAT4, "mvpMatrix");
	uniform_mat4 u_view       = uniform_mat4(GLSL_DT_MAT4, "u_view");
	uniform_mat4 u_projection = uniform_mat4(GLSL_DT_MAT4, "u_projection");
	
	uniform_vec3 uCameraPos      = uniform_vec3(GLSL_DT_VEC3, "uCameraPos");
	uniform_vec3 uCamera_forward = uniform_vec3(GLSL_DT_VEC3, "uCamera_forward");
	uniform_vec3 uCamera_up      = uniform_vec3(GLSL_DT_VEC3, "uCamera_up");
	uniform_vec3 uCamera_right   = uniform_vec3(GLSL_DT_VEC3, "uCamera_right");
	uniform_vec3 camera_lookat   = uniform_vec3(GLSL_DT_VEC3, "camera_lookat");


	//flat in float hexSize[];
	uniform_float hexSize = uniform_float(GLSL_DT_FLOAT, "hexSize");

	// ............................ Universal reserved ..........................
	//Global settings
	uniform_vec4  global_ambient_color = uniform_vec4(GLSL_DT_VEC4, "global_ambient_color");
	uniform_float global_ambience      = uniform_float(GLSL_DT_FLOAT, "global_ambience");

	// Object material settings
	//uniform_vec4 mat_color    = uniform_vec4(GLSL_DT_VEC4, "mat_color");
	//uniform_vec4 mat_diffuse  = uniform_vec4(GLSL_DT_VEC4, "mat_diffuse");
	uniform_vec4 mat_ambient  = uniform_vec4(GLSL_DT_VEC4, "mat_ambient");
	uniform_vec4 mat_specular = uniform_vec4(GLSL_DT_VEC4, "mat_specular");
	//uniform_vec4 mat_emissive = uniform_vec4(GLSL_DT_VEC4, "mat_emissive");

	//uniform_float mat_shinines    = uniform_float(GLSL_DT_FLOAT, "mat_shinines");
	//uniform_float mat_metallic    = uniform_float(GLSL_DT_FLOAT, "mat_metallic");
	//uniform_float mat_roughness   = uniform_float(GLSL_DT_FLOAT, "mat_roughness");
	//uniform_float mat_reflectance = uniform_float(GLSL_DT_FLOAT, "mat_reflectance");

	// .............................................................................

	//hex surface display data
	//uniform_float hex_min_surface_display_value = uniform_float(GLSL_DT_FLOAT, "hex_min_surface_display_value");
	//uniform_float hex_max_surface_display_value = uniform_float(GLSL_DT_FLOAT, "hex_max_surface_display_value");

	//uniform_vec2  grid_origin = uniform_vec2(GLSL_DT_VEC2, "grid_origin");
	//uniform_float voxel_hcp_z_increment = uniform_float(GLSL_DT_FLOAT, "voxel_hcp_z_increment");


	//uniform_vec2  resolution = uniform_vec2(GLSL_DT_VEC2, "resolution");
	// ---------------------------------------------------------------------------------------------

	// ----------------------------- Shader variable definitions ----------------------------
	glm::mat4 mvpMatrix;

	float xp = 0.866025f;
	float sf = 2.0f; // Scaling factor : if not of value around 2.0 have point and hex columns not equating with each other. 

	glm::vec4 point_0; // = glm::vec4(0.0, 0.0, 0.0, 1.0);
	// vec4 point_1 = vec4(0.0,1.0* hexSize,0.0,1.0);
	// vec4 point_2 = vec4(xp* hexSize,0.5* hexSize,0.0,1.0)  ;
	// vec4 point_3 = vec4(xp* hexSize,-0.5* hexSize,0.0,1.0) ;
	// vec4 point_4 = vec4(0.0,-1.0* hexSize,0.0,1.0);
	// vec4 point_5 = vec4(-xp* hexSize,-0.5* hexSize,0.0,1.0);
	// vec4 point_6 = vec4(-xp* hexSize,0.5* hexSize,0.0,1.0) ;

	glm::vec4 point_1;// = glm::vec4(0.0, sf * hexSize.value, 0.0, 1.0);
	glm::vec4 point_2;// = glm::vec4(xp * hexSize.value * sf, 0.5 * hexSize.value * sf, 0.0, 1.0);
	glm::vec4 point_3;// = glm::vec4(xp * hexSize.value * 2.0, -0.5 * hexSize.value * sf, 0.0, 1.0);
	glm::vec4 point_4;// = glm::vec4(0.0, -sf * hexSize.value, 0.0, 1.0);
	glm::vec4 point_5;// = glm::vec4(-xp * hexSize.value * sf, -0.5 * hexSize.value * sf, 0.0, 1.0);
	glm::vec4 point_6;// = glm::vec4(-xp * hexSize.value * sf, 0.5 * hexSize.value * sf, 0.0, 1.0);

	// ............................ Universal reserved ..........................
	
	// Testing only comment out or delete when no needed
	// Testing only comment out or delete when no needed
	// Need to have as uniforms
	int   camera_lighting_type = 0;
	int   use_camera_lighting = 0;
	glm::vec3  lighting_camera_offset = glm::vec3(0.0, 0.0, 0.0);

	glm::vec4  light_color = glm::vec4(1.0, 1.0, 1.0, 1.0);
	glm::vec3  lighting_direction = glm::vec3(0.0, 0.0, 1.0);
	float ambience = .5;
	float specular_strength = .5;
	float lighting_intensity = .5;

	// .............................................................................

	// ---------------------------------------------------------------------------------------------

	// ========================= DEFINE SHADER FUNCTIONS HERE =========================

	void initialise_points() {
		point_0 = glm::vec4(0.0, 0.0, 0.0, 1.0);
		// vec4 point_1 = vec4(0.0,1.0* hexSize,0.0,1.0);
		// vec4 point_2 = vec4(xp* hexSize,0.5* hexSize,0.0,1.0)  ;
		// vec4 point_3 = vec4(xp* hexSize,-0.5* hexSize,0.0,1.0) ;
		// vec4 point_4 = vec4(0.0,-1.0* hexSize,0.0,1.0);
		// vec4 point_5 = vec4(-xp* hexSize,-0.5* hexSize,0.0,1.0);
		// vec4 point_6 = vec4(-xp* hexSize,0.5* hexSize,0.0,1.0) ;

		point_1 = glm::vec4(0.0, sf * hexSize.value, 0.0, 1.0);
		point_2 = glm::vec4(xp * hexSize.value * sf, 0.5 * hexSize.value * sf, 0.0, 1.0);
		point_3 = glm::vec4(xp * hexSize.value * 2.0, -0.5 * hexSize.value * sf, 0.0, 1.0);
		point_4 = glm::vec4(0.0, -sf * hexSize.value, 0.0, 1.0);
		point_5 = glm::vec4(-xp * hexSize.value * sf, -0.5 * hexSize.value * sf, 0.0, 1.0);
		point_6 = glm::vec4(-xp * hexSize.value * sf, 0.5 * hexSize.value * sf, 0.0, 1.0);
	}


	
	void use_lighting(glm::vec4 vertex, glm::vec3 vertex_normal, glm::vec4 raw_color) {

		glm::vec4 diffuse;
		float NdotL;
		glm::vec3 viewDir;
		glm::vec3 reflectDir;
		glm::vec4 light_intensity;
		glm::vec4 camera_light_intensity;
		glm::vec3 light_direction_vector;

		glm::vec3 camera_loc = uCameraPos.value;

		if (uCamera_forward.value.x == 0.0f && uCamera_forward.value.y == 0.0f && uCamera_forward.value.z == 0.0f) {
			//if(uCameraPos.x == 0.0f && uCameraPos.y == 0.0f && uCameraPos.z == 0.0f){
			//gs_out.varyingColor = vec4(1.0, 1.0, 0.0, 1.0);
			colors_out.data_element = glm::vec4(1.0, 1.0, 0.0, 1.0);
		}
		else {

			NdotL = glm::max(glm::dot(glm::normalize(vertex_normal), glm::normalize(-lighting_direction)), 0.0f);
			diffuse = NdotL * light_color;

			//viewDir = normalize(camera_loc - glm::vec3(vertex.xyz));
			viewDir = glm::normalize(camera_loc - glm::vec3(vertex.x, vertex.y,vertex.z));
			reflectDir = glm::reflect(glm::normalize(-lighting_direction), vertex_normal);

			float spec = float(glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 128));
			glm::vec4 specular = mat_specular.value * spec * light_color;

			//light_intensity = (mat_ambient.value + diffuse + specular) * lighting_intensity * 0.5;//*.25
			light_intensity = (mat_ambient.value + diffuse + specular) * glm::vec4(lighting_intensity * 0.5f);//*.25

			camera_light_intensity = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
			if (use_camera_lighting != 0) {
				if (camera_lighting_type == 0) {
					glm::vec3 light_camera_relative_x = uCamera_right.value * lighting_camera_offset.x;
					glm::vec3 light_camera_relative_y = uCamera_up.value * lighting_camera_offset.y;
					glm::vec3 light_camera_relative_z = uCamera_forward.value * lighting_camera_offset.z;

					glm::vec3 light_loc = uCameraPos.value + light_camera_relative_x + light_camera_relative_y + light_camera_relative_z;

					//light_direction_vector = -(light_loc - vertex.xyz);// spot light from camera pointing dir of camera
					light_direction_vector = -(light_loc - glm::vec3(vertex.x, vertex.y, vertex.z));// spot light from camera pointing dir of camera
				}
				else
					light_direction_vector = (uCamera_forward.value);//  global light pointing in direction of camera : not good

				NdotL = glm::max(dot(normalize(vertex_normal), normalize(-light_direction_vector)), 0.0f);
				diffuse = NdotL * light_color;

				//viewDir = glm::normalize(uCameraPos.value - glm::vec3(vertex.xyz));
				viewDir = glm::normalize(uCameraPos.value - glm::vec3(vertex.x, vertex.y, vertex.z));
				reflectDir = reflect(normalize(-light_direction_vector), normalize(vertex_normal));

				//float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
				float spec = float(glm::pow(glm::max(glm::dot(viewDir, reflectDir), 0.0f), 128));
				glm::vec4 specular = specular_strength * spec * light_color;

				camera_light_intensity = (diffuse + specular) * lighting_intensity; //*.5
			}

			//gs_out.varyingColor = vec4((light_intensity+camera_light_intensity),1.0)* raw_color;
			//gs_out.varyingColor = vec4((light_intensity + camera_light_intensity)) * raw_color;

			colors_out.data_element = glm::vec4((light_intensity + camera_light_intensity)) * raw_color;
			//colors_out.data_element = raw_color;
			colors_out.add_data_element();
		}
	}


	void add_top(glm::vec4 center, bool top,size_t j) {
		glm::vec3 vertex_normal = glm::vec3(0.0, 0.0, 1.0);

		if (!top)
			vertex_normal.z = -1;

		//use_lighting(center, vertex_normal, colors.data[j]);
		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_0);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_1);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_2);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_0);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_2);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_3);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_0);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_3);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_4);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_0);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_4);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_5);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_0);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_5);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_6);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_0);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_6);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (center + point_1);
		EmitVertex();

		EndPrimitive();

	}


	void add_sides(glm::vec4 center, size_t j) {
		glm::vec3 vertex_normal;
		glm::vec4 top = center;
		glm::vec4 bottom = center;
		bottom.z = 0.0;

		// Side 1
		vertex_normal = glm::vec3(0.25, 0.75, 0.0);

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_1);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_1);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_2);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_1);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_2);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_2);
		EmitVertex();

		EndPrimitive();

		// Side 2
		vertex_normal = glm::vec3(1.0, 0.0, 0.0);

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_2);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_2);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_3);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_2);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_3);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_3);
		EmitVertex();

		EndPrimitive();

		// Side 3
		vertex_normal = glm::vec3(0.25, -0.75, 0.0);

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_3);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_3);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_4);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_3);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_4);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_4);
		EmitVertex();

		EndPrimitive();

		// Side 4
		vertex_normal = glm::vec3(-0.25, -0.75, 0.0);

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_4);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_4);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_5);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_4);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_5);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_5);
		EmitVertex();

		EndPrimitive();

		// Side 5
		vertex_normal = glm::vec3(-1.0, 0.0, 0.0);

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_5);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_5);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_6);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_5);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_6);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_6);
		EmitVertex();

		EndPrimitive();

		// Side 6
		vertex_normal = glm::vec3(-0.25, 0.75, 0.0);

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_6);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_6);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_1);
		EmitVertex();

		EndPrimitive();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_6);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (top + point_1);
		EmitVertex();

		use_lighting(center, vertex_normal, colors.data[j]);
		gl_Position = mvpMatrix * (bottom + point_1);
		EmitVertex();

		EndPrimitive();
	}

	// ================================================================================
	// all glsl main code must go into scon_main_code()
	void scon_main_code(size_t i, size_t j) override {
		//mvpMatrix = modelViewProjectionMatrix;
		//mvpMatrix = u_projection.value;
		mvpMatrix = u_projection.value * u_view.value;

		//glm::vec4 center = gl_in[0].gl_Position;
		glm::vec4 center = in_data.data[i][j].gl_Position;

		initialise_points();

		center.x *= sf;
		center.y *= sf;
		center.z *= sf;

		// following do not work : need to fix as probably due to uniforms 
		// mat_specular and others not being set
		add_top(center, true,j); // top surface element
		add_sides(center,j);    // side surface element 

		add_top(glm::vec4(center.x, center.y, 0.0, 1.0), false,j); // bottom surface element


		// ######### DISPLAY ALL VARIABLES THAT HAVE A VERTEX INDEX BETWEEN THE FROM AND TO ROW VALUES #######
// ######### USING ImGui Statements #######
		if (j >= from_row && j <= to_row) { // must be present

			ImGui::Text("Vertex %i", j);
			ImGui::SameLine();
			ImGui::Text("pos : %9f3 :: %9f3 : %9f3 :", center.x, center.y, center.z);
			ImGui::SameLine();
			ImGui::Text("color : %9f3 :: %9f3 : %9f3 : %9f3 :", colors.data[j].x, colors.data[j].y, colors.data[j].z, colors.data[j].a);
			//ImGui::Text("abs : %9f3 :  ", abs(a - floor(a)));
			//				ImGui::SameLine();
							//ImGui::Text("1 : %9f3 :  ", xy_xcenter.x);
							//ImGui::SameLine();
							//ImGui::Text("2 : %9f3 :  ", grid_center_location.x);
							//ImGui::SameLine();
							//ImGui::Text("3 : %9f3 :  ", z_grid_interval);
							//ImGui::SameLine();
							//ImGui::Text("xy_ycenter : %9f3 :  ", a);

		} // must be present

//printf("scon_main_code 000 : %i \n", colors.data.size());
		// data element must be present for each output color definition for each vertex
		// Ths is not a glsl expression but a emulation of colors_out = colors.data[i];
		//colors_out.data_element = colors.data[j];
		//colors_out.data_element = xy_grid_color.value;

	

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
		number_vertices = unsigned int(get_number_of_vertices(in_data));// Modify this if needed
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

		variable_display_control(int(in_data.data[0].size())); // MUST BE PRESENT TO ENABLE VARIABLE DISPLAY CONTROLS

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
//printf("scon_main 005A %i\n", gl_out.size());
//printf("scon_main 005B %i\n", colors_out.data.size());
	}

protected:
	
};

