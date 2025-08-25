#pragma once

#include <string>

#include <thirdparty/ImGUI/imgui_docking/imgui.h>
#include <thirdparty/ImGUI/imgui_docking/imgui_internal.h>

#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>
#include <FrameWork/AFW/Tools/afw_dialogs.h>

#define SHADERED_MAX_PATH 260

class graphics_pipline_panel_class
{
public:
	graphics_pipline_panel_class() {}
	~graphics_pipline_panel_class() {}


    std::string VBPath = "";

    std::string VSPath = "";
    std::string GSPath = "";
    std::string FSPath = "";

    bool use_geometry_shader = false;

	void display() {
        bool open = true;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_None;

        ImGui::Begin("Graphics Pipeline", &open, window_flags);

        const char* buffer_data_type_items[] = { "undefined","points", "triangles", "indexed triangles" };// This cannot be defined outside this function

        float xpos = 10;
        float ypos = 30;

        ImGui::Separator();

        text("BUFFER DATA", xpos + 250.0f, ypos);

        ypos += 30;

        text("Vertex Buffer   : ", xpos, ypos);
        ImGui::PushItemWidth(entry_width);
        text_input("###vbuff", VBPath, xpos + text_width, ypos, 300);// Change to non editable text

        ImGui::SameLine();
        ImGui::PushItemWidth(text_width);
        if (ImGui::Button("...##pui_vbbtn")) {
            m_dialogShaderType = "buffer";

            //  !!!!!!!!! not yet implemented !!!!!!!!!!!!!
            //file_pathname = load_vertex_buffer_object();

            //if (file_pathname) {
            //    VBPath = (char*)file_pathname;
            //}
        }


        ypos += 30;
        text("Buffer data type   : ", xpos, ypos);
        ImGui::PushItemWidth(160.0f);

        ImGui::SameLine();
        if (ImGui::Combo("###invocation", &invocation, &Funcs::ItemGetter, buffer_data_type_items, IM_ARRAYSIZE(buffer_data_type_items))) {

        }

        ypos += 30;


        ImGui::Separator();

        text("SHADERS", xpos + 250.0f, ypos);

        ypos += 30;
        /* vertex shader path */
        ImGui::PushItemWidth(text_width);

        text("Vertex Shader   : ", xpos, ypos);
        ImGui::PushItemWidth(entry_width);
        text_input("###vspath", VSPath, xpos + text_width, ypos, 300);

        ImGui::SameLine();
        ImGui::PushItemWidth(text_width);
        if (ImGui::Button("...##pui_vsbtn")) {
            m_dialogShaderType = "Vertex";
            char const* patterns[] = { "*.glsl","*.vert","*.vs" };
            file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

            if (file_pathname) {
                VSPath = (char*)file_pathname;
            }
        }

        ypos += 30;

        text("Geometry Shader : ", xpos, ypos);
        ImGui::PushItemWidth(entry_width);
        text_input("###gspath", GSPath, xpos + text_width, ypos, 300);

        ImGui::SameLine();
        ImGui::PushItemWidth(text_width);
        if (ImGui::Button("...##pui_gsbtn")) {
            //m_dialogPath = item->VSPath;
           // dialog_path = GSPath;
            m_dialogShaderType = "Geometry";
            //ifd::FileDialog::Instance().Open("PropertyShaderDlg", "Select a shader", "GLSL & HLSL {.glsl,.hlsl,.vert,.vs,.frag,.fs,.geom,.gs,.comp,.cs,.slang,.shader},.*");
            char const* patterns[] = { "*.glsl","*.geo","*.gs" };
            file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

            if (file_pathname) {
                GSPath = (char*)file_pathname;
            }
        }

        ImGui::SameLine();
        if (checkbox("##pui_gscb", "", xpos + text_width + entry_width + 50.0f, ypos, 0.0f, 0.0f, use_geometry_shader)) {
            // code to include/edit geometry shader 
        }

        ypos += 30;

        text("Fragment Shader : ", xpos, ypos);
        ImGui::PushItemWidth(entry_width);
        text_input("###fspath", FSPath, xpos + text_width, ypos, 300);

        ImGui::SameLine();
        ImGui::PushItemWidth(text_width);
        if (ImGui::Button("...##pui_fsbtn")) {
            //m_dialogPath = item->VSPath;
           // dialog_path = GSPath;
            m_dialogShaderType = "Fragment";
            //ifd::FileDialog::Instance().Open("PropertyShaderDlg", "Select a shader", "GLSL & HLSL {.glsl,.hlsl,.vert,.vs,.frag,.fs,.geom,.gs,.comp,.cs,.slang,.shader},.*");
            char const* patterns[] = { "*.glsl","*.frag","*.fs" };
            file_pathname = vwDialogs::open_file(nullptr, patterns, 1);

            if (file_pathname) {
                FSPath = (char*)file_pathname;
            }
        }

        ImGui::Separator();

        ImGui::End();

		


	}



private:





    std::string dialog_path = "";




    float text_width = 130.0f;
    float entry_width = 300.0f;

    std::string m_dialogShaderType;

    enum class dialog_shader_type_enum { none, buffer, vertex, geometry, fragmant };
    dialog_shader_type_enum dialog_shader_type = dialog_shader_type_enum::none;

    int invocation = 1;

    struct Funcs { static bool ItemGetter(void* data, int n, const char** out_str) { *out_str = ((const char**)data)[n]; return true; } };

    char const* file_pathname = nullptr;

};
