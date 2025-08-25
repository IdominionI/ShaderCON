#pragma once

#include <FrameWork/SCONFW/Panels/sfw_shader_panel_base.h>

class scon_geometry_data_panel_class : public sfw_shader_data_panel_base_class
{
public:
	scon_geometry_data_panel_class() {}

	~scon_geometry_data_panel_class() {}

	void display() {

		bool open = true;
		ImGuiWindowFlags window_flags = 0;

		ImGui::Begin("Geometry Data Display", &open, window_flags);

		float xpos = 10.0f;
		float ypos = 30.0f;

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("###gdpitbm1", tab_bar_flags))
		{
			if (ImGui::BeginTabItem("Buffer Data###gdpitbd")) {
				if (ImGui::BeginTabBar("###gdpitb", tab_bar_flags))
				{
					if (ImGui::BeginTabItem("gl_in###gdpitb01"))
					{
						if (gl_in && !gl_in->empty())
							display_gl_in(xpos, ypos);
						else
							ImGui::Text("No gl_in data defined.");
						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("in layouts###gdpitb02"))
					{
						if (input_layouts && !input_layouts->empty())
							display_layout_inputs(xpos, ypos);
						else
							ImGui::Text("No out layouts data defined.");
						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("out layouts###gdpitb03"))
					{
						if (output_layouts && !output_layouts->empty())
							display_layout_outputs(xpos, ypos);
						else
							ImGui::Text("No out layouts data defined.");
						ImGui::EndTabItem();
					}

					if (ImGui::BeginTabItem("gl_out###gdpitb04"))
					{
						if (gl_out && !gl_out->empty())
							display_gl_out(xpos, ypos);
						else
							ImGui::Text("No gl_out data defined.");
						ImGui::EndTabItem();
					}

					ImGui::EndTabBar();
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Uniform Data###gdpitud")) {
				//ImGui::Text("No Uniform data defined.");
				display_glsl_uniforms(xpos, ypos);
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}
		ImGui::End();

	}

	void display_gl_in(float xpos, float ypos) {
		//ImGui::Text("display_gl_in");
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		static int freeze_cols = 1;
		static int freeze_rows = 1;

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
		ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));

		//ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);

		ImGui::Separator();
		ypos += 60.0f;
		text("Display Rows", xpos + 140.0f, ypos); ypos += 20.0f;

		text("Max number rows to display :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("###dgglimr", ImGuiDataType_U32, &gl_in_max_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_max_display_rows(gl_in_from_row, gl_in_to_row, gl_in_max_rows);
		}

		ypos += 30.0f;
		text("From :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("To :###dgglidr", ImGuiDataType_U32, &gl_in_from_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_from_row(gl_in_from_row, gl_in_to_row, gl_in_max_rows);
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("###dgglidre", ImGuiDataType_U32, &gl_in_to_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_to_row(gl_in_from_row, gl_in_to_row, gl_in_max_rows);
		}

		ImGui::SameLine();
		text("Advance : lines", xpos + 305, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40.0f);
		ImGui::InputScalarN("###dgglodal", ImGuiDataType_U32, &gl_in_alines, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::SameLine();
		if (ImGui::Button("/\\###dggliblu")) {
			decrement_lines(gl_in_from_row, gl_in_to_row, gl_in_alines);
		}
		ImGui::SameLine();

		if (ImGui::Button("\\/###dgglibld")) {
			increment_lines(gl_in_from_row, gl_in_to_row, gl_in_alines);
		}
		ImGui::SameLine();
		text(" page", xpos + 525.0f, ypos);
		ImGui::SameLine();
		if (ImGui::Button("/\\###dgglibpu")) {
			page_down(gl_in_from_row, gl_in_to_row, gl_in_max_rows);
		}
		ImGui::SameLine();
		if (ImGui::Button("\\/###dgglibpd")) {
			page_up(gl_in_from_row, gl_in_to_row, gl_in_max_rows);
		}

		ypos += 30.0f;
		text("Freeze first n colums : ", xpos + 10, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputScalarN("###dgglfc", ImGuiDataType_U32, &freeze_cols, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);
		ypos += 20.0f;
		text("Freeze first n rows   : ", xpos + 10, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputScalarN("###dgglfr", ImGuiDataType_U32, &freeze_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::PopStyleVar(2);

		// When using ScrollX or ScrollY we need to specify a size for our table container!
		// Otherwise by default the table will fit all available space, like a BeginChild() call.
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * table_display_height);
		if (ImGui::BeginTable("Geomtry gl_in", 3, flags, outer_size))
		{
			ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
			ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()

			//printf("display_layout_inputs %i\n", input_layouts->size());
			ImGui::TableSetupColumn("Position");
			ImGui::TableSetupColumn("Point Size");
			ImGui::TableHeadersRow();

			for (int row = gl_in_from_row; row < gl_in_to_row && row < gl_in->size(); row++)
			{
				ImGui::TableNextRow();
				for (int column = 0; column < 3; column++)
				{
					// Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
					// Because here we know that:
					// - A) all our columns are contributing the same to row height
					// - B) column 0 is always visible,
					// We only always submit this one column and can skip others.
					// More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
					if (!ImGui::TableSetColumnIndex(column) && column > 0)
						continue;

					switch (column) {
					case 0: ImGui::Text("%d", row); break;
					case 1: {
						gl_PerVertex gl_in_data = (*gl_in)[row];
						ImGui::Text("%9.3f : %9.3f : %9.3f : %9.3f", gl_in_data.gl_Position.x, gl_in_data.gl_Position.y, gl_in_data.gl_Position.z, gl_in_data.gl_Position.w); break;
					}
					case 2: {
						gl_PerVertex gl_in_data = (*gl_in)[row];
						ImGui::Text("%9.3f", gl_in_data.gl_PointSize); break;
					}
					}

				}
			}
			ImGui::EndTable();
		}
	}

	void display_layout_inputs(float xpos, float ypos) {
		//ImGui::Text("display_layout_inputs");
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		static int freeze_cols = 1;
		static int freeze_rows = 1;

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
		ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));

		//ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);

		ImGui::Separator();
		ypos += 60.0f;
		text("Display Rows", xpos + 140.0f, ypos); ypos += 20.0f;

		text("Max number rows to display :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("###dgglmr", ImGuiDataType_U32, &in_layout_max_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_max_display_rows(in_layout_from_row, in_layout_to_row, in_layout_max_rows);
		}

		ypos += 30.0f;
		text("From :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("To :###dggldr ", ImGuiDataType_U32, &in_layout_from_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_from_row(in_layout_from_row, in_layout_to_row, in_layout_max_rows);
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("###dggldre", ImGuiDataType_U32, &in_layout_to_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_to_row(in_layout_from_row, in_layout_to_row, in_layout_max_rows);
		}

		ImGui::SameLine();
		text("Advance : lines", xpos + 305.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40);
		ImGui::InputScalarN("###dggldal", ImGuiDataType_U32, &in_layout_alines, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::SameLine();

		if (ImGui::Button("/\\###dgglblu")) {
			decrement_lines(in_layout_from_row, in_layout_to_row, in_layout_alines);
		}
		ImGui::SameLine();

		if (ImGui::Button("\\/###dgglbld")) {
			increment_lines(in_layout_from_row, in_layout_to_row, in_layout_alines);
		}

		ImGui::SameLine();
		text("page", xpos + 525.0f, ypos);
		ImGui::SameLine();
		if (ImGui::Button("/\\###dgglbpu")) {
			page_down(in_layout_from_row, in_layout_to_row, in_layout_max_rows);
		}
		ImGui::SameLine();
		if (ImGui::Button("\\/###dgglbpd")) {
			page_up(in_layout_from_row, in_layout_to_row, in_layout_max_rows);
		}

		ypos += 30.0f;
		text("Freeze first n colums : ", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputScalarN("###dgglfc", ImGuiDataType_U32, &freeze_cols, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);
		ypos += 20.0f;
		text("Freeze first n rows   : ", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		ImGui::InputScalarN("###dgglfr", ImGuiDataType_U32, &freeze_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);



		ImGui::PopStyleVar(2);

		// When using ScrollX or ScrollY we need to specify a size for our table container!
		// Otherwise by default the table will fit all available space, like a BeginChild() call.
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * table_display_height);
		if (ImGui::BeginTable("Geomtry in_layout", 7, flags, outer_size))
		{
			ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
			ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()

			// Define the table colums for data to be displayed.
			// This must be done before any row data is displayed or get ImGui to crash
			for (int i = 0; i < input_layouts->size(); i++) {
				ImGui::TableSetupColumn((*input_layouts)[i]->layout_name.c_str());
			}
			ImGui::TableHeadersRow();

			//printf("display_layout_inputs %i\n", input_layouts->size());
			for (int row = in_layout_from_row; row < in_layout_to_row && row < (*input_layouts)[0]->get_data_size(); row++)
			{
				ImGui::TableNextRow();
				//for (int column = 0; column < 7; column++)
				for (int column = 0; column < input_layouts->size() + 1; column++)
				{
					// Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
					// Because here we know that:
					// - A) all our columns are contributing the same to row height
					// - B) column 0 is always visible,
					// We only always submit this one column and can skip others.
					// More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
					if (!ImGui::TableSetColumnIndex(column) && column > 0)
						continue;
					if (column == 0)
						ImGui::Text("%d", row);
					else
						display_data_element((*input_layouts)[column - 1], row);
				}
			}

			ImGui::EndTable();
		}
	}

	void display_layout_outputs(float xpos, float ypos) {
		//ImGui::Text("display_layout_outputs");
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		static int freeze_cols = 1;
		static int freeze_rows = 1;

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
		ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));

		//ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);

		ImGui::Separator();
		ypos += 60.0f;
		text("Display Rows", xpos + 140.0f, ypos); ypos += 20;

		text("Max number rows to display :", xpos + 10, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("###dglomr", ImGuiDataType_U32, &out_layout_max_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_max_display_rows(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
		}

		ypos += 30.0f;
		text("From :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("To :###dglodr ", ImGuiDataType_U32, &out_layout_from_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_from_row(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
		}

		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("###dglodre", ImGuiDataType_U32, &out_layout_to_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_to_row(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
		}

		ImGui::SameLine();
		text("Advance : lines", xpos + 305.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40);
		ImGui::InputScalarN("###dglodal", ImGuiDataType_U32, &out_layout_alines, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::SameLine();

		if (ImGui::Button("/\\###dgloblu")) {
			decrement_lines(out_layout_from_row, out_layout_to_row, out_layout_alines);
		}
		ImGui::SameLine();

		if (ImGui::Button("\\/###dglobld")) {
			increment_lines(out_layout_from_row, out_layout_to_row, out_layout_alines);
		}

		ImGui::SameLine();
		text("page", xpos + 525.0f, ypos);
		ImGui::SameLine();
		if (ImGui::Button("/\\###dglobpu")) {
			page_down(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
		}
		ImGui::SameLine();
		if (ImGui::Button("\\/###dglobpd")) {
			page_up(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
		}

		ypos += 30.0f;
		text("Freeze first n colums : ", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputScalarN("###dglofc", ImGuiDataType_U32, &freeze_cols, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);
		ypos += 20.0f;
		text("Freeze first n rows   : ", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputScalarN("###dglofr", ImGuiDataType_U32, &freeze_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::PopStyleVar(2);

		// When using ScrollX or ScrollY we need to specify a size for our table container!
		// Otherwise by default the table will fit all available space, like a BeginChild() call.
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * table_display_height);
		if (ImGui::BeginTable("Geomtry out_layout", int(output_layouts->size()) + 1, flags, outer_size))
		{
			ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
			ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()

			// Define the table colums for data to be displayed.
			// This must be done before any row data is displayed or get ImGui to crash
			for (int i = 0; i < output_layouts->size(); i++) {
				ImGui::TableSetupColumn((*output_layouts)[i]->layout_name.c_str());
			}
			ImGui::TableHeadersRow();

			//printf("display_layout_inputs %i\n", input_layouts->size());
			// Display data for each data element by column and row
			for (int row = out_layout_from_row; row < out_layout_to_row && row < (*output_layouts)[0]->get_data_size(); row++)
			{
				ImGui::TableNextRow();

				for (int column = 0; column < output_layouts->size() + 1; column++)
				{
					// Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
					// Because here we know that:
					// - A) all our columns are contributing the same to row height
					// - B) column 0 is always visible,
					// We only always submit this one column and can skip others.
					// More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
					if (!ImGui::TableSetColumnIndex(column) && column > 0)
						continue;
					if (column == 0)
						ImGui::Text("%d", row);
					else
						display_data_element((*output_layouts)[column - 1], row);
				}
			}
			ImGui::EndTable();
		}

	}

	void display_gl_out(float xpos, float ypos) {
		//ImGui::Text("display_gl_out");
		static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
		static int freeze_cols = 1;
		static int freeze_rows = 1;

		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * 0.60f));
		ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * 0.60f));

		//ImGui::CheckboxFlags("ImGuiTableFlags_Resizable", &flags, ImGuiTableFlags_Resizable);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollX", &flags, ImGuiTableFlags_ScrollX);
		//ImGui::CheckboxFlags("ImGuiTableFlags_ScrollY", &flags, ImGuiTableFlags_ScrollY);

		ImGui::Separator();
		ypos += 60.0f;
		text("Display Rows", xpos + 140.0f, ypos); ypos += 20.0f;

		text("Max number rows to display :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("###dgglomr", ImGuiDataType_U32, &gl_out_max_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_max_display_rows(gl_out_from_row, gl_out_to_row, gl_out_max_rows);
		}

		ypos += 30.0f;
		text("From :", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100.0f);
		if (ImGui::InputScalarN("To :###dgglodrf ", ImGuiDataType_U32, &gl_out_from_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_from_row(gl_out_from_row, gl_out_to_row, gl_out_max_rows);
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		if (ImGui::InputScalarN("###dgglodrt", ImGuiDataType_U32, &gl_out_to_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
			on_change_to_row(gl_out_from_row, gl_out_to_row, gl_out_max_rows);
		}

		//if (gl_out_to_row < gl_out_from_row ) gl_out_to_row = gl_out_from_row + gl_out_max_rows;
		//ypos += 20;
		ImGui::SameLine();
		text("Advance : lines", xpos + 305.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(40.0f);
		ImGui::InputScalarN("###dgglodal", ImGuiDataType_U32, &gl_out_alines, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

		ImGui::SameLine();
		if (ImGui::Button("/\\###dggloblu")) {
			decrement_lines(gl_out_from_row, gl_out_to_row, gl_out_alines);
		}
		ImGui::SameLine();

		if (ImGui::Button("\\/###dgglobld")) {
			increment_lines(gl_out_from_row, gl_out_to_row, gl_out_alines);
		}
		ImGui::SameLine();
		text(" page", xpos + 525.0f, ypos);
		ImGui::SameLine();
		if (ImGui::Button("/\\###dgglobpu")) {
			page_down(gl_out_from_row, gl_out_to_row, gl_out_max_rows);
		}
		ImGui::SameLine();
		if (ImGui::Button("\\/###dgglobpd")) {
			page_up(gl_out_from_row, gl_out_to_row, gl_out_max_rows);
		}

		ypos += 30.0f;
		text("Freeze first n colums : ", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputScalarN("###dgglofc", ImGuiDataType_U32, &freeze_cols, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);
		ypos += 20.0f;
		text("Freeze first n rows   : ", xpos + 10.0f, ypos);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::InputScalarN("###dggloifr", ImGuiDataType_U32, &freeze_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);



		ImGui::PopStyleVar(2);

		// When using ScrollX or ScrollY we need to specify a size for our table container!
		// Otherwise by default the table will fit all available space, like a BeginChild() call.
		const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

		ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * table_display_height);
		if (ImGui::BeginTable("Geomtry gl_out",3, flags, outer_size))
		{
			ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
			ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()

			//printf("gl_out size %i\n", gl_out->size());

			ImGui::TableSetupColumn("Position");
			ImGui::TableSetupColumn("Point Size");
			ImGui::TableHeadersRow();

			for (int row = gl_out_from_row; row < gl_out_to_row && row < gl_out->size(); row++)
			{
				ImGui::TableNextRow();

				for (int column = 0; column < 3; column++)
				{
					// Both TableNextColumn() and TableSetColumnIndex() return true when a column is visible or performing width measurement.
					// Because here we know that:
					// - A) all our columns are contributing the same to row height
					// - B) column 0 is always visible,
					// We only always submit this one column and can skip others.
					// More advanced per-column clipping behaviors may benefit from polling the status flags via TableGetColumnFlags().
					if (!ImGui::TableSetColumnIndex(column) && column > 0)
						continue;

					switch (column) {
					case 0: ImGui::Text("%d", row); break;
					case 1: {
						gl_PerVertex gl_out_data = (*gl_out)[row];
						ImGui::Text("%9.3f : %9.3f : %9.3f : %9.3f", gl_out_data.gl_Position.x, gl_out_data.gl_Position.y, gl_out_data.gl_Position.z, gl_out_data.gl_Position.w); break;
					}
					case 2: {
						gl_PerVertex gl_out_data = (*gl_out)[row];
						ImGui::Text("%9.3f", gl_out_data.gl_PointSize); break;
					}
					}

				}
			}

			ImGui::EndTable();
		}

	}


private:

};