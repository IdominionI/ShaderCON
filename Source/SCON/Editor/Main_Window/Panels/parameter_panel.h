#pragma once

#include <thirdparty/ImGUI/imgui_docking/imgui.h>
#include <FrameWork/AFW/ImGUI/ImGuiEx/imgui_widgets.h>

#include "../Widgets/parameters_widget.h"

#include "../Widgets/global_light_widget.h"
#include "../Widgets/camera_widget.h"
#include "../Widgets/viewport_properties_widget.h"

/*
							PARAMETER PANEL

	Parent class to define the application parameter panel that is the parent
	"window widget" for the application to display the entity parameter data
	into and enable whatever functions. procedures etc top be performed for
	that entity
*/

class parameter_panel_class {
public:
	parameter_panel_class() {

	}

	~parameter_panel_class() {}

	global_light_parameters_widget_class global_light_parameters_widget;
	camera_poperties_widget_class        camera_poperties_widget;
	viewport_properties_widget_class     viewport_properties_widget;

	log_panel_class *log_panel  = NULL; // Define the application log panel to display application messages to
	log_panel_class *code_panel = NULL; // Define the application log panel to display application shder or other code error messages to

	void show() {
		ImGui::Begin("Parameters");
			if (ImGui::BeginTabBar("##app_parameter_data", ImGuiTabBarFlags_None))
			{
				if (ImGui::BeginTabItem("Global Light##global_light_tab")) {
					display_global_light_parameters();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Camera##camera_tab")) {
					display_camera_parameters();
					ImGui::EndTabItem();
				}

				if (ImGui::BeginTabItem("Viewport##viewport_tab")) {
					display_viewport_parameters();
					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}

		ImGui::End();
	}

	void display_global_light_parameters() {
		title("Global Lighting");
		global_light_parameters_widget.display();
	}

	void display_camera_parameters() {
		title("Camera");
		camera_poperties_widget.display();
	}

	void display_viewport_parameters() {
		title("Viewport");
		viewport_properties_widget.display();
	}
};