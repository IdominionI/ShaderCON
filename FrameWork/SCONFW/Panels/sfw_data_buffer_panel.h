#pragma once

#include <vector>
#include <string>

#include <thirdparty/ImGUI/imgui_docking/imgui.h>

#include <FrameWork/OGLFW/Objects/model.h>
#include <FrameWork/AFW/Tools/afw_dialogs.h>

#include "Framework/OGLFW/Shader/shader_variable.h"

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include <FrameWork/SCONFW/Process/sfw_pipeline.h>

struct column_data_struct {
    std::string         column_name;
    glsl_data_type_enum data_type;
    std::vector<void*>  data;
};

class data_buffer_panel_class : public sfw_shader_data_panel_base_class {
public:
    data_buffer_panel_class() {}
    ~data_buffer_panel_class() {}

    bool changed_buffer_data = false;

    sconfw_pipeline_template_base_class* sconfw_pipeline = nullptr;

    void display() {
        bool open = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

        const char* buffer_data_type_items[] = { "undefined","points","lines", "triangles", "indexed triangles" };// This cannot be defined outside this function

        if (!sconfw_pipeline) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Do not have the graphics pipeline defined to import/create vertex buffer data.\n");
            ImGui::Text("Do not have the graphics pipeline defined to import/create vertex buffer data.");
            return;
        }

        ImGui::Begin("Buffer Data", &open, window_flags);

        ImGui::Separator();
        ImGui::Text("Controls: ");

        if (ImGui::Button("CLEAR ALL##objprev_clearbuf")) {
            clear_all_data();
        }

        ImGui::SameLine();

        if (ImGui::Button("Load Data From 3D Model")) {
            //m_dialogActionType = 2;
            char const* patterns[] = { "*.ply","*.obj","*.stl" };
            file_pathname = vwDialogs::open_file(nullptr, patterns, 1);
            //if (file_pathname) load_buffer_data = true;
            if (file_pathname) load_buffer_from_model(file_pathname);
        }

        ImGui::SameLine();
        if (ImGui::Button("Load Buffer Data")) {
            //m_dialogActionType = 3;
            //char const* patterns[] = { "*.buf","*" };
            //file_pathname = vwDialogs::open_file(nullptr, patterns, 1);
            //if (file_pathname) load_buffer_data = true;
        }

        ImGui::SameLine();
        if (ImGui::Button("Save Buffer Data")) {
            //m_dialogActionType = 3;
            //char const* patterns[] = { "*.buf","*" };
            //file_pathname = vwDialogs::open_file(nullptr, patterns, 1);
            //if (file_pathname) load_buffer_data = true;
        }

        ImGui::Separator();

        //ypos += 30;
        text("Buffer data type   : ", ImGui::GetCursorPosX()+10, ImGui::GetCursorPosY());
        ImGui::PushItemWidth(160.0f);

        ImGui::SameLine();
        if (ImGui::Combo("###invocation", &invocation, &Funcs::ItemGetter, buffer_data_type_items, IM_ARRAYSIZE(buffer_data_type_items))) {
            switch (invocation) { // Highly dependent upon order of buffer_data_type_items
                case 0 : buffer_vertex_data_type = geom_layout_data_in_enum::undefined;     break;
                case 1 : buffer_vertex_data_type = geom_layout_data_in_enum::points;        break;
                case 2 : buffer_vertex_data_type = geom_layout_data_in_enum::lines;         break;
                case 3 : buffer_vertex_data_type = geom_layout_data_in_enum::triangles;     break;
                //case 4 : buffer_vertex_data_type = geom_layout_data_enum::triangles_strip;break;
                default : buffer_vertex_data_type = geom_layout_data_in_enum::undefined;
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Use Buffer Data")) {
            if (columns_data.size() < 1) {
                afw_globalc::get_current_logger()->log(LogLevel::INFO, "No buffer data defined to use.\n");
                return;
            }

            for (size_t col = 0; col < columns_data.size(); col++) {
                if (columns_data[col].data.size() < 1) {
                    afw_globalc::get_current_logger()->log(LogLevel::INFO, "No buffer data defined to use.\n");
                    return;
                }
            }

            switch (buffer_vertex_data_type) {
                case geom_layout_data_in_enum::points :{
                    sconfw_pipeline->initialise_vertex_shader();
                    sconfw_pipeline->initialise_geometry_shader(); // Only use if using geometry shader and only after initialise_buffer_data
                    break;
                }
            }
        }

        ImGui::Separator();

        // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
        float xpos = 10;
        float ypos = 10;
        //ImVec2 cur_pos;
        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
        static int freeze_cols = 1;
        static int freeze_rows = 1;

        //xpos = ImGui::GetCursorPosX();
        ypos = ImGui::GetCursorPosY();

        if (columns_data.size() > 0)
            total_number_vertices = columns_data[0].data.size();
        else
            total_number_vertices = 0;

        std::string row_header = "Display vertex data : " + std::to_string(total_number_vertices) + "  vertices";

        text(row_header.c_str(), xpos + 140.0f, ypos); ypos += 20;
        //text("Display Rows", xpos + 140.0f, ypos); ypos += 20;

        text("Max number rows to display :", xpos + 10, ypos);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputScalarN("###dlimr", ImGuiDataType_U32, &out_layout_max_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
            on_change_max_display_rows(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
        }

        ypos += 30;
        text("From :", xpos + 10, ypos);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputScalarN("To :###dlidr ", ImGuiDataType_U32, &out_layout_from_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
            on_change_from_row(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
        }

        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        if (ImGui::InputScalarN("###dlidre", ImGuiDataType_U32, &out_layout_to_row, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None)) {
            on_change_to_row(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
        }

        ImGui::SameLine();
        text("Advance : lines", xpos + 305, ypos);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(40);
        ImGui::InputScalarN("###dlidal", ImGuiDataType_U32, &out_layout_alines, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

        ImGui::SameLine();

        if (ImGui::Button("/\\###dliblu")) {
            decrement_lines(out_layout_from_row, out_layout_to_row, out_layout_alines);
        }
        ImGui::SameLine();

        if (ImGui::Button("\\/###dlibld")) {
            increment_lines(out_layout_from_row, out_layout_to_row, out_layout_alines);
        }

        ImGui::SameLine();
        text("page", xpos + 525, ypos);
        ImGui::SameLine();
        if (ImGui::Button("/\\###dlibpu")) {
            page_down(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
        }
        ImGui::SameLine();
        if (ImGui::Button("\\/###dlibpd")) {
            page_up(out_layout_from_row, out_layout_to_row, out_layout_max_rows);
        }

        ypos += 30;
        text("Freeze first n colums : ", xpos + 10, ypos);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputScalarN("###dlifc", ImGuiDataType_U32, &freeze_cols, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);
        ypos += 20;
        text("Freeze first n rows   : ", xpos + 10, ypos);
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100);
        ImGui::InputScalarN("###dlifr", ImGuiDataType_U32, &freeze_rows, 1, NULL, NULL, "%d", ImGuiInputTextFlags_None);

        //ImGui::PopStyleVar(2);


        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        ImGui::Separator();

        if(columns_data.size() > 0){
            // When using ScrollX or ScrollY we need to specify a size for our table container!
            // Otherwise by default the table will fit all available space, like a BeginChild() call.
            const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();

            ImVec2 outer_size = ImVec2(0.0f, TEXT_BASE_HEIGHT * table_display_height);
            if (ImGui::BeginTable("Vertex data Buffer", int(columns_data.size()) + 1, flags, outer_size))
            {

                ImGui::TableSetupScrollFreeze(freeze_cols, freeze_rows);
                ImGui::TableSetupColumn("Vertex #", ImGuiTableColumnFlags_NoHide); // Make the first column not hideable to match our use of TableSetupScrollFreeze()

                // Define the table colums for data to be displayed.
                // This must be done before any row data is displayed or get ImGui to crash
                for (int i = 0; i < columns_data.size(); i++) {
                    ImGui::TableSetupColumn(columns_data[i].column_name.c_str());
                }
                ImGui::TableHeadersRow();

                //printf("display_layout_inputs %i\n", input_layouts->size());
                // Display data for each data element by column and row
                for (int row = out_layout_from_row; row < out_layout_to_row && row < columns_data[0].data.size(); row++)
                {
                    ImGui::TableNextRow();

                    for (int column = 0; column < columns_data.size() + 1; column++)
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
                            display_column_data_element(columns_data[column - 1],column, row);
                    }
                }
                ImGui::EndTable();
            }

            
        }


        ImGui::End();
    }


    void display_column_data_element(column_data_struct &column_data,size_t column, size_t row) {
        switch (column_data.data_type) {
            case glsl_data_type_enum::Float: {
                float *data = (float*)(column_data.data[row]);
                std::string eid = "##valuedit" + std::to_string(column) + "_" + std::to_string(row);
                //ImGui::DragFloat(eid.c_str(), v, 0.01f);
                ImGui::DragFloat(eid.c_str(), data, 0.01f);
                break;
            }

            case glsl_data_type_enum::vec3 : {
                glm::vec3 *data = (glm::vec3*)(column_data.data[row]);
                //float *v = &data->x;
                float *v[3] = { &data->x,&data->y,&data->z };
                std::string eid = "##valuedit" + std::to_string(column) + "_" + std::to_string(row);
                //ImGui::DragFloat(eid.c_str(), v, 0.01f);
                ImGui::DragFloat3(eid.c_str(), *v, 0.01f);
                break;
            }

            case glsl_data_type_enum::vec4: {
                glm::vec4* data = (glm::vec4*)(column_data.data[row]);
                //float *v = &data->x;
                float* v[4] = { &data->x,&data->y,&data->z };
                std::string eid = "##valuedit" + std::to_string(column) + "_" + std::to_string(row);
                //ImGui::DragFloat(eid.c_str(), v, 0.01f);
                ImGui::DragFloat4(eid.c_str(), *v, 0.01f);
                break;
            }
        }
    }

    bool load_buffer_from_model(const std::string& pathname) {

        afw_globalc::get_current_logger()->log(LogLevel::INFO, "Loading buffer data from a 3D model");

        Model mdl;
        bool ret = mdl.LoadFromFile(pathname);

        if (!ret) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Could not read initial buffer data. Aborted\n");
            return false;
        }

        // Load vetex data into a buffer like structure to be used in a glsl emulator program
        if (!sconfw_pipeline) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Do not have the graphics pipeline defined to import buffer data.\n");
            return false;
        }

        //sconfw_pipeline->initialise_buffer_data(mdl);
        // Load mesh data into a std::vextor style buffer for display and editing
        sconfw_pipeline->buffer_data_input.define_buffers(mdl, 0);

        define_file_import_data();

        return true;
    }

protected:
    void clear_all_data() {
        for (size_t col = 0; col < columns_data.size(); col++) {
            columns_data[col].data.clear();
            columns_data[col].data.shrink_to_fit();
        }

        columns_data.clear();
    }

    std::vector<column_data_struct> columns_data = {};

    size_t define_file_import_data() {
        size_t number_columns = 0;
        clear_all_data();

        if (sconfw_pipeline->buffer_data_input.positions.size() > 0) {
            column_data_struct column_data;
            number_columns += 1;
            column_data.column_name = "Position";
            column_data.data_type   = glsl_data_type_enum::vec3;

            for (size_t i = 0; i < sconfw_pipeline->buffer_data_input.positions.size(); i++) {
                column_data.data.push_back(&sconfw_pipeline->buffer_data_input.positions[i]);
            }

            columns_data.push_back(column_data);
        }

        if (sconfw_pipeline->buffer_data_input.colors.size() > 0) {
            column_data_struct column_data;
            number_columns += 1;
            column_data.column_name = "Color";
            column_data.data_type = glsl_data_type_enum::vec4;
            for (size_t i = 0; i < sconfw_pipeline->buffer_data_input.colors.size(); i++) {
                column_data.data.push_back(&sconfw_pipeline->buffer_data_input.colors[i]);
            }

            columns_data.push_back(column_data);
        }

        if (sconfw_pipeline->buffer_data_input.normals.size() > 0) {
            column_data_struct column_data;
            number_columns += 1;
            column_data.column_name = "Normal";
            column_data.data_type = glsl_data_type_enum::vec3;
            for (size_t i = 0; i < sconfw_pipeline->buffer_data_input.normals.size(); i++) {
                column_data.data.push_back(&sconfw_pipeline->buffer_data_input.normals[i]);
            }

            columns_data.push_back(column_data);
        }

        if (sconfw_pipeline->buffer_data_input.point_sizes.size() > 0) {
            column_data_struct column_data;
            number_columns += 1;
            column_data.column_name = "Point Size";
            column_data.data_type = glsl_data_type_enum::Float;
            for (size_t i = 0; i < sconfw_pipeline->buffer_data_input.point_sizes.size(); i++) {
                column_data.data.push_back(&sconfw_pipeline->buffer_data_input.point_sizes[i]);
            }

            columns_data.push_back(column_data);
        }

        return number_columns;
    }



private:
    char const* file_pathname = nullptr;

    bool   load_buffer_data = false;
    size_t total_number_vertices = 0;

    int invocation = 1;// points
    geom_layout_data_in_enum buffer_vertex_data_type = geom_layout_data_in_enum::points;
    struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

};