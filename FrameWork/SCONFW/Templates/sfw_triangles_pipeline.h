#pragma once

#include <FrameWork/OGLFW/Shader/oglfw_shader.h>

#include <Framework/SCONFW/Process/sfw_pipeline.h>

#include <FrameWork/SCONFW/Templates/sfw_vertex_shader_triangles_template.h>
#include <FrameWork/SCONFW/Templates/sfw_geometry_shader_triangles_template.h>

class sconfw_pipeline_template_base_class : public sconfw_pipeline_base_class
{
public:
	sconfw_pipeline_template_base_class() {}
	~sconfw_pipeline_template_base_class() {}

	float lighting_dir_vector[3] = { 0.0f,90.0f,0.0f };
	float intensity = 0.75;
	ImVec4 light_color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 255.0f / 255.0f);
	bool ref_color = false;
	ImVec4 ref_color_v = { 1.0f, 0.0f, 1.0f, 0.5f };
	float min_height = -2.0f;
	float max_height = 9.0f;

	bool initialised = false;

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CRITICAL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// !!!!!!!!!!! DEFINE THE VERTEX AND GEOMETRY EMULATION CLASSES HERE !!!!!!!!!!!!!

	sconfw_buffer_data_input_base_class buffer_data_input;	// defined custom user data buffer class
	sfw_vertex_shader_triangles			vertex_shader;      // defined custom user vertex emulation shader class
	sfw_geometry_shader_triangle		geometry_shader;	// defined custom user geometry emulation shader class. Comment out if if not used.

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	bool perform_pipeline_procedure() {
		//initialise_buffer_data(mesh_object, mesh_id);// uncomment if have dynamic changing buffer data

		// update emulation vertex uniform values
		sconfw_camwera.update_modelviewprojection_uniform(vertex_shader.modelViewProjectionMatrix);

		//vertex_shader.min_height.value = -2.0f;
		//vertex_shader.max_height.value = 10.0f;
		vertex_shader.display_as_points.value = 0;


		//Custom user geometry emulation shader class. Mandatory and must be present
		vertex_shader.scon_main();

		//printf("perform_pipeline_procedure 000 : %f : %f : %f\n", sconfw_camwera.selected_camera->position.x, sconfw_camwera.selected_camera->position.y, sconfw_camwera.selected_camera->position.z);
		//printf("gl_out vertices : %i : %i\n", vertex_shader.gl_out.size(), vertex_shader.color_out.size());

		if (vertex_shader.color_out.size() != vertex_shader.gl_out.size()) {
			//error message here
			printf("vertex_shader.color_out.size() != vertex_shader.gl_out.size()\n");
			return false;
		}

		//define_output_data(vertex_shader.gl_out,vertex_shader.color_out);// UNCOMMENT WHEN NOT USING GEOMETRY_SHADER;

		// vvvvvvvvvvvv Custom user geometry emulation shader class. vvvvvvvvvvvvvvv
		// COMMENT OUT THIS SECTION IF GEOMETRY SHADER IS NOT USED.

		// ------------------- Define emulation geometry shader  input and output layouts --------------
		geometry_shader.define_gl_in(vertex_shader.gl_out, vertex_shader.color_out);
		geometry_shader.define_in_layouts(vertex_shader.output_layouts);

		// !!!!!!!!!!! update of uniforms goes here !!!!!!!!!!!!!!
		// -------------------------- Update emulation geometry shader uniform values -------------------------
		//sconfw_camwera.update_modelviewprojection_uniform(geometry_shader.mvpMatrix);
		sconfw_camwera.update_view_uniform(geometry_shader.u_view);
		sconfw_camwera.update_projecion_uniform(geometry_shader.u_projection);
		sconfw_camwera.update_location_uniform(geometry_shader.uCameraPos);
		sconfw_camwera.update_forward_uniform(geometry_shader.uCamera_forward);
		sconfw_camwera.update_up_uniform(geometry_shader.uCamera_up);
		sconfw_camwera.update_right_uniform(geometry_shader.uCamera_right);
		sconfw_camwera.update_loockat_uniform(geometry_shader.camera_lookat);

		geometry_shader.hexSize.value = 0.5f;

		//geometry_shader.lighting_direction = { 1.0f,0.0f,0.0f };
		//geometry_shader.lighting_intensity = 0.75f;
		//geometry_shader.light_color = {1.0f ,1.0f ,1.0f ,1.0f};


		if (!initialised) { 
			geometry_shader.light_color = { light_color.x,light_color.y,light_color.z,light_color.w };
			geometry_shader.lighting_direction = { lighting_dir_vector[0],lighting_dir_vector[1] ,lighting_dir_vector[2] };
			geometry_shader.lighting_intensity = intensity;
			vertex_shader.min_height.value = min_height;
			vertex_shader.max_height.value = max_height;
			initialised = true;
		}


		ImGui::Begin("Triangle light uniforms");
			float x_pos = 10.0f, y_pos = 40.0f;

			y_pos += 40;

			ImGui::PushStyleColor(ImGuiCol_Button, light_color);// Chanage all the buttons to the light color until a ImGui::PopStyleColor
			if (ex_button("Light Color###plc", x_pos + 130, y_pos, 100, 30)) {
				ImGui::OpenPopup("Light Color###plcpu");
			}
			ImGui::PopStyleColor(1);// This must be present and the number represents the number of previous buttons that have been colored or will have a crash

			if (ImGui::BeginPopup("Light Color###plcpu")) {
				ImGuiColorEditFlags flags = ImGuiColorEditFlags_None;
				flags |= ImGuiColorEditFlags_NoAlpha; flags |= ImGuiColorEditFlags_NoSidePreview; flags |= ImGuiColorEditFlags_PickerHueWheel;
				ImGui::ColorPicker4("Light Color###cp", (float*)&light_color, flags, ref_color ? &ref_color_v.x : NULL);
				geometry_shader.light_color = { light_color.x,light_color.y,light_color.z,light_color.w };
				//global_light->setAmbientColor({ light_color.x,light_color.y,light_color.z,light_color.w });

				ImGui::EndPopup();
			}

			y_pos += 40;
			text(" Global Lighting\n   Orientation", x_pos + 123, y_pos);
			y_pos += 35;
			text("Pitch           Yaw             Roll", x_pos + 50, y_pos);
			y_pos += 20;
			ImGui::SetCursorPosX(x_pos + 5);
			ImGui::SetCursorPosY(y_pos);
			if (ImGui::SliderFloat3("###lpdv", lighting_dir_vector, -180.0f, 180.0f, "%.2f")) {
				//global_light->setOrientation(glm::vec3{ lighting_dir_vector[0],lighting_dir_vector[1],lighting_dir_vector[2] });
				geometry_shader.lighting_direction = { lighting_dir_vector[0],lighting_dir_vector[1] ,lighting_dir_vector[2] };
			}

			y_pos += 30;
			text("Global Light Intesity", x_pos + 105, y_pos);
			y_pos += 20;
			ImGui::SetCursorPosX(x_pos + 140);
			ImGui::SetCursorPosY(y_pos);
			ImGui::SetNextItemWidth(100);
			if (ImGui::SliderFloat("###plal", &intensity, 0.0f, 1.0f, "%.2f")) {
				geometry_shader.lighting_intensity = intensity;
			}

			y_pos += 30;
			text("Min Height", x_pos + 105, y_pos);
			y_pos += 20;
			ImGui::SetCursorPosX(x_pos + 140);
			ImGui::SetCursorPosY(y_pos);
			ImGui::SetNextItemWidth(100);
			if (ImGui::SliderFloat("###plalmin", &min_height, -10.0f, 20.0f, "%.2f")) {
				vertex_shader.min_height.value = min_height;
			}

			y_pos += 30;
			text("Max Height", x_pos + 105, y_pos);
			y_pos += 20;
			ImGui::SetCursorPosX(x_pos + 140);
			ImGui::SetCursorPosY(y_pos);
			ImGui::SetNextItemWidth(100);
			if (ImGui::SliderFloat("###plalmax", &max_height, -10.0f, 20.0f, "%.2f")) {
				vertex_shader.max_height.value = max_height;
			}

		ImGui::End();


		geometry_shader.scon_main();

		define_output_data(geometry_shader.gl_out, geometry_shader.colors_out.data);
		// ^^^^^^^^^^^^^^  END CUSTOM GEOMETRY SHADER SECTION ^^^^^^^^^^^^^^^

		define_buffers();

		render_geometry();

		delete_buffers();// Very imprtant to perform this or will experience a memory leak

		return true;
	}

	// perform this once only if testing for static non changing vvertex data
	void initialise_buffer_data(Model& mesh_object, size_t mesh_id = 0) {

		// Custom user data buffer class
		buffer_data_input.define_buffers(mesh_object, mesh_id);

		//Custom user vertex emulation shader class : define glsl version and vertex position data
		vertex_shader.initialise(450, buffer_data_input.positions);
	}

	// This must be used for every vertex shader emulation that has data loaded from a geometry model
	// or buffer data. Not complete as only vertex position data is transmitted to vertex shader. Need
	// to accomodate for all model and buffer data to be transmitted to the vitual vertex shader function initialise.
	void initialise_vertex_shader() {
		vertex_shader.initialise(450, buffer_data_input.positions);
	}

	// future to be expanded or have additional functions to include layout name and data etc 
	void initialise_vertex_shader(std::vector<glm::vec3> positions) {
		vertex_shader.initialise(450, positions);
	}

	// Only use if using geometry shader and only after initialise_buffer_data
	void initialise_geometry_shader() {
		geometry_shader.initialise(450);
	}

	GLuint create_output_shader_program(std::string output_vert_shader_file, std::string output_frag_shader_file) {
		return output_shader_program.create_glsl_shader_program(output_vert_shader_file, "", output_frag_shader_file);
	}

	void define_output_data(std::vector<gl_PerVertex>& gl_out, std::vector<glm::vec4>& color_out) {
		clear_output_data();

		for (size_t i = 0; i < gl_out.size(); i++) {
			output_postion.push_back(gl_out[i].gl_Position);
			output_point_size.push_back(gl_out[i].gl_PointSize);
			output_color.push_back(color_out[i]);
			//printf("pipeline : define_output_data %u : %f : %f : %f \n", i, output_postion[i].x, output_postion[i].y, output_postion[i].z);
			//printf("pipeline : define_output_data %u : %f : %f : %f : %f \n", i, output_color[i].x, output_color[i].y, output_color[i].z, output_color[i].w);
		}
	}

	void clear_output_data() {
		output_postion.clear();
		output_postion.shrink_to_fit();

		output_color.clear();
		output_color.shrink_to_fit();

		output_point_size.clear();
		output_point_size.shrink_to_fit();
	}

	void render_geometry() override {
		if (!output_shader_program.get_program()) {
			printf("render_geometry !output_shader_program.get_program() % i\n", output_shader_program.get_program());
		}

		//if (output_color.size() == 0) printf("output color size = 0\n");

		//for (size_t i=0; i < output_color.size(); i++) {
		//	printf("perform_pipeline_procedure :render_geometry: %i : %f : %f : %f : %f \n",i, output_color[i].r, output_color[i].g, output_color[i].b, output_color[i].a);
		//}


		if (output_shader_program.get_program()) {
			//printf("render_geometry have output_shader_program %i\n", output_shader_program.get_program());

						//update_uniforms();
			output_shader_program.use(output_shader_program.get_program());

			vertex_VAO.Bind();

			// Following two lines needed if to have color Alpha values to take effect
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glDrawArrays(GL_POINTS, 0, output_postion.size()); GCE;

//printf("render_geometry have output_shader_program %i\n", output_postion.size());
			//glDrawArrays(GL_LINE_STRIP, 0, output_postion.size()); GCE;

			//glDrawArrays(GL_LINES, 0, output_postion.size()); GCE;
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(output_postion.size())); GCE;


			vertex_VAO.Unbind();


			output_shader_program.release();
		}
	}

	oglfw_shader_class output_shader_program;

protected:
	std::vector<glm::vec4> output_postion = {};
	std::vector<glm::vec4> output_color = {};
	std::vector<float>     output_point_size = {};

	unsigned long ofs1 = 0, ofs2 = 0;

	oglfw_VBO_class		vertex_VBO; // Mandatory
	oglfw_VAO_class		vertex_VAO; // Mandatory

private:
	void define_buffers() {
		// to correctly render an object the defining the VBO, VAO and 
		// textures etc must be performrd in the correct order as here

		generate_buffers();

		bind_buffers();

		define_buffer_data();

		define_VBO_buffer_data_layout();
	}

	void  generate_buffers() {
		vertex_VBO.generate(); ///Generate VBO
		vertex_VAO.generate(); ///Generate VAO

		//EBO.generate();        ///Generate EBO
	}

	void bind_buffers() {
		vertex_VAO.Bind(); ///First Bind the VAO, so that all the configuration is saved in this VAO
		vertex_VBO.Bind(); ///Bind the VBO to GL_ARRAY_BUFFER
	}

	void unbind_buffers() {
		vertex_VAO.Unbind(); ///First Bind the VAO, so that all the configuration is saved in this VAO
		vertex_VBO.Unbind(); ///Bind the VBO to GL_ARRAY_BUFFER
	}

	void define_buffer_data() {
		///Populate VBO with data

		vertex_VBO.define_empty_VBO_buffer(GL_ARRAY_BUFFER, unsigned int (output_postion.size()) * (sizeof(glm::vec4) + sizeof(float) + sizeof(glm::vec4)));

		glBufferSubData(GL_ARRAY_BUFFER, 0, output_postion.size() * sizeof(glm::vec4), output_postion.data());
		glBufferSubData(GL_ARRAY_BUFFER, output_postion.size() * sizeof(glm::vec4), output_point_size.size() * sizeof(float), output_point_size.data());
		glBufferSubData(GL_ARRAY_BUFFER, output_postion.size() * sizeof(glm::vec4) + output_point_size.size() * sizeof(float), output_color.size() * sizeof(glm::vec4), output_color.data());


	}

	void define_VBO_buffer_data_layout() {
		//Set the info of how the VBO must be read

		GLuint position_location = output_shader_program.getAttributeLocation("v_position");
		GLuint point_size_location = output_shader_program.getAttributeLocation("v_point_size");
		GLuint color_location = output_shader_program.getAttributeLocation("v_color");

		if (position_location < 0) {
			printf("Vertex point size shader input location of v_position could note be found.\n");
			return;
		}

		if (point_size_location < 0) {
			printf("Vertex position shader input location of v_point_size could note be found.\n");
			return;
		}

		if (color_location < 0) {
			printf("Vertex color shader input location of v_color could note be found.\n");
			return;
		}

		//printf("point_size_location :: %u\n", point_size_location);

		// position attribute
		vertex_VAO.SetVec4(position_location, sizeof(glm::vec4), (const GLvoid*)(0)); // vertex Layout location 0

		// point_size attribute
		// Alternative is to comment out references to vertex point size if not in use and set ofs1 to zero
		//ofs1 = 0;
		ofs1 = unsigned long(output_postion.size()) * sizeof(float);
		// For some crazy reason cannot get the correct location of v_point_size !!!!!
		////vertex_VAO.SetFloat(point_size_location, sizeof(float), (const GLvoid*)(ofs1));// vertex Layout location 1
		vertex_VAO.SetFloat(1, sizeof(float), (const GLvoid*)(ofs1));// vertex Layout location 1
		//vertex_VAO.SetFloat(1, sizeof(float), &ofs1);// vertex Layout location 1 // looks  like this works same as above line of code without compilor warnings

		// color attribute
		ofs2 = ofs1 + unsigned long(output_color.size()) * sizeof(glm::vec4);
		vertex_VAO.SetVec4(color_location, sizeof(glm::vec4), (const GLvoid*)(ofs2));// vertex Layout location 2
		//vertex_VAO.SetVec4(color_location, sizeof(glm::vec4), &ofs2);// vertex Layout location 2 // This does not work. DO NOT USE
		
	}

	void delete_buffers() {
		vertex_VBO.Delete();
		vertex_VAO.Delete();
	}

};
