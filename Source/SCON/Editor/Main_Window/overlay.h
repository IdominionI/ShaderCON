#pragma once

#include <glm/glm.hpp>
#include <thirdparty/ImGUI/imgui_docking/imgui.h>

struct vw_cross_hair_overlay_struct {
    glm::vec2 view_dim = { 50.0f,50.0f }; //crosshair overlay ImGui window size;

    glm::vec2 p1 = { 0.0f,0.0f };
    glm::vec2 p2 = { 10.0f,10.0f };
    glm::vec2 p3 = { 0.0f,10.0f };
    glm::vec2 p4 = { 10.0f,0.0f };

    // Center of crosshair overlay ImGui window viewport
    void define_crosshair_line() {
        p1 = { view_dim.x / 2 - 10, view_dim.y / 2 };
        p2 = { view_dim.x / 2 + 10, view_dim.y / 2 };
        p3 = { view_dim.x / 2,      view_dim.y / 2 - 10 };
        p4 = { view_dim.x / 2,      view_dim.y / 2 + 10 };
    }

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings
                                    | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

    // cross hair color
    //ImVec4 cross_hair_colorf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
    ImU32  cross_hair_color = ImColor(ImVec4(1.0f, 1.0f, 0.4f, 1.0f));

    void set_cross_hair_overlay_color(ImVec4 ross_hair_colorf) {
        cross_hair_color = ImColor(ross_hair_colorf);
    }

    void set_cross_hair_overlay_color(glm::vec4 color) {
        set_cross_hair_overlay_color(ImVec4( color.r,color.g,color.b,color.a ));
    }
};

struct vw_gmbal_overlay_struct {
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings
                                    | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;


};

//struct vw_camera_info_overlay_struct {
//
//    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize 
//                                    | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
//};