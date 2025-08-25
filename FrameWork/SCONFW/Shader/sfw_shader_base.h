#pragma once

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include <FrameWork/SCONFW/Shader/sfw_shader_data_types.h>

class sconfw_shader_base {
public:
	sconfw_shader_base() {}
	~sconfw_shader_base() {}

	void define_glsl_version (unsigned int v) {	glsl_version = v;}

	void enable_point_size() {
		glEnable(GL_PROGRAM_POINT_SIZE); GCE
	}

	virtual void scon_main() {}

	virtual void scon_main_code(size_t i) {}
	
	virtual void scon_main_code(size_t i,size_t j) {}

	std::vector<gl_PerVertex> gl_in  = {};
	std::vector<gl_PerVertex> gl_out = {};

	std::vector<glm::vec4> color_in  = {};
	std::vector<glm::vec4> color_out = {};

	std::vector<glsl_base_uniform_struct *> uniforms;
	//std::vector<glsl_variable_base_struct*> variables;

	void add_uniform(glsl_base_uniform_struct *uniform) {
		uniforms.push_back(uniform);
	}

	//void add_variable(glsl_variable_base_struct *variable) {
	//	variables.push_back(variable);
	//}

	std::vector<vlayout_base_struct*> input_layouts;
	std::vector<vlayout_base_struct*> output_layouts;

	void add_input_layout(vlayout_base_struct* input_layout) {
		input_layouts.push_back(input_layout);
	}

	void add_output_layout(vlayout_base_struct* output_layout) {
		output_layouts.push_back(output_layout);
	}

protected:
	unsigned int glsl_version    = 0;
	unsigned int draw_mode       = 0;
	unsigned int number_vertices = 0;

	glm::vec4 gl_Position  = {0.0f,0.0f,0.0f,1.0f};
	float     gl_PointSize = 1.0f;
	glm::vec4 gl_color     = {1.0f,1.0f,1.0f,1.0f};

	void clear_buffer_in_data() {
		gl_in.clear();
		color_in.clear();
	}

	void clear_buffer_out_data() {
		gl_out.clear();
		color_out.clear();
	}

	// ==================== #### Variables and functions to display shader variables #### ====================
	// Variables and functions defined and used to control the display of shader variables within an 
	// ImGui::Begin ImGui::End() definition that is defined within an sfw shader class.
	// NOTE :: variable_display_control cannot be used on its own without use of an ImGui::Begin ImGui::End()
	//         declaration.

	// Within the shader scon_main_code function all variables to be displayed 

	int alines   = 1;
	int max_rows = 20;
	int from_row = 0;
	int to_row   = max_rows;

	// ---------------------- Change display varable paramter functions ----------------------
	
	void variable_display_control(int number_variables) {
		float xpos = 10;
		float ypos = 20;

		std::string row_header = "Variables data : " + std::to_string(number_variables) + "  vertices";

		text(row_header.c_str(), xpos + 140.0f, ypos); ypos += 20;

		text("Max number rows to display :", xpos + 10, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("###dlimr", ImGuiDataType_U32, &max_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_max_display_rows(from_row, to_row, max_rows);
		}

		ypos += 30;
		text("From :", xpos + 10, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("To :###dlidr ", ImGuiDataType_U32, &from_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_from_row(from_row, to_row, max_rows);
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("###dlidre", ImGuiDataType_U32, &to_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_to_row(from_row, to_row, max_rows);
		}

		ImGui::SameLine();
		text("Advance : lines", xpos + 305, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40);
		ImGui::InputScalarN("###dlidal", ImGuiDataType_U32, &alines, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::SameLine();

		if (ImGui::Button("/\\###dliblu")) {
			decrement_lines(from_row, to_row, alines);
		}
		ImGui::SameLine();

		if (ImGui::Button("\\/###dlibld")) {
			increment_lines(from_row, to_row, alines);
		}

		ImGui::SameLine();
		text("page", xpos + 525, ypos);
		ImGui::SameLine();
		if (ImGui::Button("/\\###dlibpu")) {
			page_down(from_row, to_row, max_rows);
		}
		ImGui::SameLine();
		if (ImGui::Button("\\/###dlibpd")) {
			page_up(from_row, to_row, max_rows);
		}

		ImGui::Separator();
	}


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


};

