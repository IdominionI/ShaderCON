#pragma once

#include <string>

#include <thirdparty/ImGUI/imgui_docking/imgui.h>
#include <thirdparty/ImGUI/imgui_docking/imgui_internal.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include <FrameWork/SCONFW/Shader/sfw_shader_data_types.h>

class sfw_shader_data_panel_base_class{
public:
	sfw_shader_data_panel_base_class() {}
	~sfw_shader_data_panel_base_class() {}

		// layout data to display
	std::vector<vlayout_base_struct*> *input_layouts  = nullptr;
	std::vector<vlayout_base_struct*> *output_layouts = nullptr;
	std::vector<gl_PerVertex>		  *gl_in		  = nullptr;
	std::vector<gl_PerVertex>		  *gl_out		  = nullptr;

	// Uniform data to display
	std::vector<glsl_base_uniform_struct*> *uniforms  = nullptr;

	// vvvvvvvvvvv glsl uniform display functions vvvvvvvvvvvvvvv

	void display_glsl_uniforms(float& xpos, float& ypos) {
		if (!uniforms || uniforms->empty()) {
			ImGui::Text("No Uniform data defined.");
			return;
		}

		ImGui::Text("Shader Uniform Data");
		ImGui::Separator();
		ypos += 30;

		for (glsl_base_uniform_struct* uniform : (*uniforms)) {
			switch (uniform->data_type) {
				case GLSL_DT_INT   : display_int_uniform(uniform, xpos, ypos); break;
				case GLSL_DT_FLOAT : display_float_uniform(uniform, xpos, ypos); break;
				case GLSL_DT_VEC3  : display_vec3_uniform(uniform, xpos, ypos); break;
				case GLSL_DT_VEC4  : display_vec4_uniform(uniform, xpos, ypos); break;
				case GLSL_DT_MAT4  : display_mat4_uniform(uniform, xpos, ypos); break;


					// ------------ add others here ----------------

				default: ImGui::Text("###");
			}
		}
	}

	void display_int_uniform(glsl_base_uniform_struct* uniform, float& xpos, float& ypos) {
		ImGui::Text(uniform->uniform_name.c_str());
		ImGui::SameLine();
		ImGui::Text(":: int ");
		ypos += 20;

		int value = *(int*)(uniform->get_uniform_data());

		ImGui::Text("%9i", value);
	}

	void display_float_uniform(glsl_base_uniform_struct* uniform, float& xpos, float& ypos) {
		ImGui::Text(uniform->uniform_name.c_str());
		ImGui::SameLine();
		ImGui::Text(":: float ");
		ypos += 20;

		float value = *(float*)(uniform->get_uniform_data());

		ImGui::Text("%9.3f", value);
	}

	void display_vec3_uniform(glsl_base_uniform_struct* uniform, float& xpos, float& ypos) {
		ImGui::Text(uniform->uniform_name.c_str());
		ImGui::SameLine();
		ImGui::Text(":: Vec3 ");
		ypos += 20;

		glm::vec3 value = *(glm::vec3*)(uniform->get_uniform_data());

		ImGui::Text("%9.3f : %9.3f : %9.3f", value[0], value[1], value[2]);
	}

	void display_vec4_uniform(glsl_base_uniform_struct* uniform, float& xpos, float& ypos) {
		ImGui::Text(uniform->uniform_name.c_str());
		ImGui::SameLine();
		ImGui::Text(":: Vec3 ");
		ypos += 20;

		glm::vec4 value = *(glm::vec4*)(uniform->get_uniform_data());

		ImGui::Text("%9.3f : %9.3f : %9.3f : %9.3f", value[0], value[1], value[2], value[3]);
	}

	void display_mat4_uniform(glsl_base_uniform_struct* uniform, float& xpos, float& ypos) {
		ImGui::Text(uniform->uniform_name.c_str());
		ImGui::SameLine();
		ImGui::Text(":: Mat4 ");
		ypos += 20;

		glm::mat4 value = *(glm::mat4*)(uniform->get_uniform_data());

		for (int i = 0; i < 4; i++) {
			ImGui::Text("%9.3f : %9.3f : %9.3f : %9.3f", value[i][0], value[i][1], value[i][2], value[i][3]);
		}
	}

	// ^^^^^^^^^^^^ glsl uniform display functions ^^^^^^^^^^^^^^

	void display_data_element(vlayout_base_struct* vlayout, size_t index) {
		//	printf("display_data_element\n");

		void* vdata_element = vlayout->get_data_element(index);

		switch (vlayout->data_type) {
			case GLSL_DT_VEC3: {
				glm::vec3 data_element = *(glm::vec3*)(vdata_element);

				ImGui::Text("%9.3f : %9.3f : %9.3f", data_element.x, data_element.y, data_element.z);
				break;
			}

			case GLSL_DT_VEC4: {
				glm::vec4 data_element = *(glm::vec4*)(vdata_element);

				ImGui::Text("%9.3f : %9.3f : %9.3f : %9.3f ", data_element.x, data_element.y, data_element.z, data_element.w);
				break;
			}

			 // ------------ add others here ----------------

		default: ImGui::Text("###");
		}
	}

	// ---------------------- Change disply paramter functions ----------------------

	void on_change_max_display_rows(int& from_row, int& to_row, int& max_rows) {
		if (max_rows < 1) max_rows = abs(max_rows);
		to_row = from_row + max_rows;
	}

	void on_change_from_row(int& from_row, int& to_row, int max_rows) {
		if (from_row < 0) from_row = 0;
		if (from_row > to_row) {
			from_row = to_row;
			to_row = from_row + max_rows;
		}
	}

	void on_change_to_row(int& from_row, int& to_row, int max_rows) {
		if (to_row - from_row > max_rows || to_row < from_row) to_row = from_row + max_rows;
	}

	void decrement_lines(int& from_row, int& to_row, int alines) {
		if (from_row > 0) {
			int displayed_rows = to_row - from_row;

			if (from_row - alines < 0) {
				from_row = 0;
				to_row = displayed_rows;
			}
			else {
				from_row -= alines;
				to_row -= alines;
			}
		}
	}


	// Until decide how to confidently deterime size of vector elements to display use the following
	//void increment_lines(int &from_row, int &to_row, int alines,int number_lines) {
	void increment_lines(int& from_row, int& to_row, int alines) {
		//if (number_lines < 1) return;

		//int displayed_rows = to_row - from_row;

		//if (to_row + alines > number_lines) {
		//	to_row   = number_lines;
		//	from_row = number_lines - displayed_rows;
		//} else {
		from_row += alines;
		to_row += alines;
		//}
	}

	void page_down(int& from_row, int& to_row, int max_rows) {
		if (from_row > max_rows) {
			from_row -= max_rows;
			to_row -= max_rows;
		}
		else {
			from_row = 0;
			to_row = max_rows;
		}
	}

	// Until decide how to confidently deterime size of vector elements to display use the following
	void page_up(int& from_row, int& to_row, int max_rows) {
		from_row = to_row;
		to_row = from_row + max_rows;
	}

protected:

	int table_display_height = 20;

	int gl_in_alines = 1;
	int gl_in_max_rows = 20;
	int gl_in_from_row = 0;
	int gl_in_to_row = gl_in_max_rows;

	int gl_out_alines = 1;
	int gl_out_max_rows = 20;
	int gl_out_from_row = 0;
	int gl_out_to_row = gl_out_max_rows;

	int in_layout_alines = 1;
	int in_layout_max_rows = 20;
	int in_layout_from_row = 0;
	int in_layout_to_row = in_layout_max_rows;

	int out_layout_alines = 1;
	int out_layout_max_rows = 20;
	int out_layout_from_row = 0;
	int out_layout_to_row = out_layout_max_rows;
private:

};
