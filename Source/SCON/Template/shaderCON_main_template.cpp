#define NOMINMAX // Need this to placed here to overcome microsoft min max intereference with std::min std::max

#define IMGUI_DEFINE_MATH_OPERATORS // Out of no where get this stupid ImGui error that this needs to be defined before imgui.h
                                    // Culprit is the use of the "thirdparty/ImGUI/ImGuizmo_quat/imGuIZMOquat.h"
                                    // Well after it was proved did not need to be set and used until started to add
                                    // some additional ImGui code.
                                    // However from experience this can cause compile errors and needs to be fixed 
                                    // by the authors of Imgui !!!! Preferbly to get rid of all ImGui math operators all together.
                                     
// ########### CRTICAL : ##############
// ### The following includes need to be present in the
// ### order given or can experience glew gl.h defined 
// ### before glew.h compile errors.
#include "SCON/Editor/Interface/theme.h"
#include <FrameWork/AFW/Interface/IconsFontAwesome4.h>

#include <FrameWork/AFW/Application/afw_app_base.h>

#include <FrameWork/OGLFW/Window/glfw_ogl_window.h>

// ------------------------------
// The following openGL error checking macro must be defined after #include <FrameWork/OGLFW/Window/glfw_ogl_window.h>
// and before all other includes, especially those that involve openGL functions calls or get a 

// Uncomment next line to exit application when openGL error occurs
//#define EXIT_ON_GL_ERROR
void gl_check_error(const char* function, const char* file, int line)
{
    GLenum error = 0;
    bool is_error = false;
    while ((error = glGetError()) != GL_NO_ERROR)
    {
        is_error = true;
        switch (error)
        {
            case GL_INVALID_ENUM:                  printf("INVALID_ENUM"); break;
            case GL_INVALID_VALUE:                 printf("INVALID_VALUE"); break;
            case GL_INVALID_OPERATION:             printf("INVALID_OPERATION"); break;
            case GL_STACK_OVERFLOW:                printf("STACK_OVERFLOW"); break;
            case GL_STACK_UNDERFLOW:               printf("STACK_UNDERFLOW"); break;
            case GL_OUT_OF_MEMORY:                 printf("OUT_OF_MEMORY"); break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: printf("INVALID_FRAMEBUFFER_OPERATION"); break;
            case GL_CONTEXT_LOST:                  printf("GL_CONTEXT_LOST"); break;
            case GL_TABLE_TOO_LARGE:               printf("GL_TABLE_TOO_LARGE"); break;
            default:
                printf("Unknown error code %d", error);
        }
        printf(" encountered at function %s (%s:%d)\n",function,file,line);
    }

#ifdef EXIT_ON_GL_ERROR
    if (is_error) { exit(1); }
#endif
}

// Uncomment next line to enable openGL error checking defined by gl_check_error function
#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS
#define GCE gl_check_error(__FUNCTION__,__FILE__, __LINE__);
#else
#define GCE
#endif
// ------------------------------

#include <FrameWork/OGLFW/imGui/oglfw_imgui_glfw.h>

#include "thirdparty/ImGUI/ImGuizmo_quat/imGuIZMOquat.h"
#define IMGUIZMO_USE_ONLY_ROT

// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/string_cast.hpp>

#include <FrameWork/SCONFW/Objects/sfw_camera.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CRITICAL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!! DEFINE THE GEOMETRY PIPELINE EMULATION CLASS HERE THAT !!!!!!!!!!!!
// !!!!!!!!!!! HAS THE VERTEX AND GEOMETRY EMULATION SHADER WITHIN IT !!!!!!!!!!!!

#include <FrameWork/SCONFW/Templates/sfw_pipeline_template.h>

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

//#include <FrameWork/SCONFW/Panels/buffer_data_panel.h>
//#include <FrameWork/SCONFW/Panels/pipeline_panel.h>
#include <FrameWork/SCONFW/Panels/sfw_vertex_data_panel.h>
#include <FrameWork/SCONFW/Panels/sfw_geometry_data_panel.h>

#include "SCON/Editor/Main_Window/overlay.h"
#include "SCON/Editor/Main_Window/Panels/parameter_panel.h"

#include <FrameWork/SCONFW/Panels/sfw_data_buffer_panel.h>

//#endif
// ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


class shaderCON_class : public afw_app_base_class {
public:
	shaderCON_class() {}
	~shaderCON_class() {}

    glfw_openGL_window_class* create_glfw_window() {
        glfw_openGL_window_class* glfw_window = new glfw_openGL_window_class();
        glfw_window->settings.setGLVersion(4, 6);
        glfw_window->create_window("ShaderCON", { 200.0f,200.0f }, 1200, 800);
        return glfw_window;
    }

    void  define_logger() {
        logger = new afw_Logger_class("logger.txt");
        afw_globalc::set_current_logger(logger);
    }

    void setup() override {
        define_logger();
    
        glfw_window = create_glfw_window();
        if (!glfw_window) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Unable to initialize/create OpenGL GLFW! : Application Aborted");
            exit(0);
        }
    
        //initialise imgui
        oglfw_ImGui.ImGui_init(glfw_window->get_window_ptr());

        log_panel = new log_panel_class();
        if (log_panel == NULL) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "No Applicaton Logger Defined : Cannot perform application");
            return;
        }

        parameter_panel.viewport_properties_widget.display_camera_info = &display_info;
        parameter_panel.viewport_properties_widget.display_gimbal      = &display_gimbal;
        parameter_panel.viewport_properties_widget.display_crosshairs  = &display_crosshairs;

        parameter_panel.viewport_properties_widget.xhair_color_viewport = &vw_cross_hair_overlay.cross_hair_color;
        parameter_panel.viewport_properties_widget.background_color     = &background_color;
        
        // This causes render problems and need to be left commented out until debuged
 // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
 // Set up the fonts file to use to display text 
 // and the awesome icon font file to display icons since ImGui is not designed
 // to have icon images imported and displayed as part of the GUI !!!!!!!
        ImFontConfig cfg;
        cfg.OversampleH = 3;
        cfg.OversampleV = 1;
        cfg.PixelSnapH = true;
        //cfg.GlyphExtraSpacing.x = 0.0f;// Depreciated
        //cfg.GlyphExtraSpacing.y = 0.0f;// Depreciated
        cfg.GlyphExtraAdvanceX = 0.0f; // New to ImGui to replace GlyphExtraSpacing
        cfg.RasterizerMultiply = 1.25f;

        ImGuiIO& io = ImGui::GetIO();

        io.Fonts->Clear();
        ImFont* mainFont = io.Fonts->AddFontFromFileTTF("Fonts/Cousine-Regular.ttf", 13.0f, &cfg);

        static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

        ImFontConfig icons_config;
        icons_config.MergeMode = true;
        icons_config.GlyphOffset = ImVec2(0, 1);

        ImFont* iconFont = io.Fonts->AddFontFromFileTTF("Fonts/fontawesome_6_solid.otf", 13.0f + 1.0, &icons_config, icons_ranges);
        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        
        current_camera = new oglfw_camera_base_class;
        current_camera->initialise();

        sconfw_pipeline.define_sconfw_camera(current_camera);

        current_camera->setPosition({ 0.0f,0.0f,30.0f });
        current_camera->set_movement_rate(0.1f);
        current_camera->set_rotation_rate(1.0f);
        current_camera->setRotationSensitivity(0.15f, 0.15f, 0.15f);
        current_camera->look_at({ 0.0f, 0.0f, 0.0f });
    }


    void run_loop() override {
       // buffer_data_panel.sconfw_pipeline = &sconfw_pipeline_test;
        data_buffer_panel.sconfw_pipeline = &sconfw_pipeline;// ++++

        
        // #### !!!!! Define Shader buffer data to be used !!!!! #####
        // Read vertex data from ply file 
        Model mdl;
        std::string model_pathnem = "data/grid_foundation.ply";
        bool ret = mdl.LoadFromFile(model_pathnem);

        if (!ret) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Could not read initial buffer data. Aborted\n");
            return;
        }
        // Load vetex data into a buffer like structure to be used in a glsl emulator program
        sconfw_pipeline.initialise_buffer_data(mdl);
        sconfw_pipeline.initialise_geometry_shader(); // Only use if using geometry shader and only after initialise_buffer_data
        

        // #### !!!!! Create the default output vertex and fragment shader programs to display the output 
        // #### !!!!! data creted by the emulated glsl program code 
        std::string output_vert_shader_file = "Assets/Shaders/default_output_vert.glsl";
        std::string output_frag_shader_file = "Assets/Shaders/default_output_frag.glsl";
        sconfw_pipeline.create_output_shader_program(output_vert_shader_file, output_frag_shader_file);
        if (sconfw_pipeline.output_shader_program.get_program() <=0) {
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "Could not create output shader program. Aborted\n");
            afw_globalc::get_current_logger()->log(LogLevel::CRITICAL, "create_shader_program ERROR :\n "+ sconfw_pipeline.output_shader_program.error_log);
            return;
        }
       


        printf("SCON :: number vertices %u : pipeline shader program id %i \n", sconfw_pipeline.vertex_shader.get_number_of_vertices(), sconfw_pipeline.output_shader_program.get_program());

        // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

        glfw_window->inititialise_window_frame_buffer(); // Required before excuting the run loop 

        // Initialise mouse and key event callback
        gflw_event.glfw_key_event   = glfw_window->glfw_key_event;
        gflw_event.glfw_mouse_event = glfw_window->glfw_mouse_event;
        //gflw_event.glfw_mouse_event->enable_mouse_button_repeat(glfw_window->get_window_ptr());
        gflw_event.glfw_cursor_event = glfw_window->glfw_cursor_event;

        ///////////////////////////////////////////////////////////////////////////
        glEnable(GL_DEPTH_TEST);

        while (!glfwWindowShouldClose(glfw_window->get_window_ptr()))
        {
            glfw_window->poll_events();

            //Process user input, in this case if the user presses the 'esc' key
            //to close the application
            gflw_event.glfw_key_event    = glfw_window->glfw_key_event;
            gflw_event.glfw_mouse_event  = glfw_window->glfw_mouse_event;
            gflw_event.glfw_cursor_event = glfw_window->glfw_cursor_event;
            current_camera->processInput(glfw_window->get_window_ptr(), gflw_event);
            current_camera->look_at({ 0.0f, 0.0f, 0.0f });

            // Update any changes to the window framebuffer to be used in rendering the scene
            // Code here in its raw form rather than as a function in class glfw_ogl_window so as to compare
            // with any opengl or glfw code that may be referenced as an example or modification
            glfwGetFramebufferSize(glfw_window->get_window_ptr(), &glfw_window->window_w, &glfw_window->window_h);
            glViewport(0, 0, glfw_window->window_w, glfw_window->window_h);

            // render
            // ------
            //glClearColor(0.1f, 0.1f,0.1f, 1.0f);
            glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Critical to have  GL_DEPTH_BUFFER_BIT with GL_COLOR_BUFFER_BIT or nothing will be displayed !!!!

            if (window_is_resized(glfw_window)) {
                glfw_window->perform_window_resize_action();
            }
// printf("MAIN 000 : %f : %f : %f\n", current_camera->position.x, current_camera->position.y, current_camera->position.z);
            oglfw_ImGui.ImGui_pre_render();

            display_crosshair_overlay();
            display_camera_info_overlay(&display_info);
            display_gimbal_overlay();

            draw_UI();// This must be placed before glfwSwapBuffers

            // Perform glsl emulation pipeline procedure
            sconfw_pipeline.perform_pipeline_procedure();

            oglfw_ImGui.ImGui_post_render(glfw_window);

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(glfw_window->get_window_ptr());

        }
    
    }

    void processInput(GLFWwindow* window)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    }

    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

    void draw_UI() {
        // Define interface style parameters each render pass so as
        // to be able to change interface style or style components
        tron_style();

        //// Log panel to display application logging messages and shader code compile error messages
        log_panel->display_application_log();
        log_panel->display_code_log();

        //buffer_data_panel.display();
        data_buffer_panel.display();// ++++

        //graphics_pipline_panel.display();

        // --------------------------------------------------
        // These initialiseations must be placed before scon_vertex_data_panel.display();
        scon_vertex_data_panel.gl_in          = &sconfw_pipeline.vertex_shader.gl_in;
        scon_vertex_data_panel.gl_out         = &sconfw_pipeline.vertex_shader.gl_out;
        scon_vertex_data_panel.input_layouts  = &sconfw_pipeline.vertex_shader.input_layouts;
        scon_vertex_data_panel.output_layouts = &sconfw_pipeline.vertex_shader.output_layouts;

        scon_vertex_data_panel.uniforms  = &sconfw_pipeline.vertex_shader.uniforms; // uniform data
        scon_vertex_data_panel.variables = &sconfw_pipeline.vertex_shader.variables;// variable data
        scon_vertex_data_panel.display();
        // --------------------------------------------------

        // --------------------------------------------------
        // These initialiseations must be placed before scon_geometry_data_panel.display();
        scon_geometry_data_panel.gl_in          = &sconfw_pipeline.geometry_shader.gl_in;
        scon_geometry_data_panel.gl_out         = &sconfw_pipeline.geometry_shader.gl_out;
        scon_geometry_data_panel.input_layouts  = &sconfw_pipeline.geometry_shader.input_layouts;
        scon_geometry_data_panel.output_layouts = &sconfw_pipeline.geometry_shader.output_layouts;

        scon_geometry_data_panel.uniforms  = &sconfw_pipeline.geometry_shader.uniforms; // uniform data
        scon_geometry_data_panel.variables = &sconfw_pipeline.geometry_shader.variables;// variable data
        scon_geometry_data_panel.display();

        parameter_panel.camera_poperties_widget.camera = current_camera;
        parameter_panel.show();
        // --------------------------------------------------

        // Following uncommented when investigating ImGui widget examples
        //ImGui::Begin;
        //bool show_demo_window = true;
        //ImGui::ShowDemoWindow(&show_demo_window);
        //ImGui::End;
        // ----------------------------------

        ImGui::Render();

    }

    void display_camera_info_overlay(bool* display_info) {
        if (*display_info) {
            static int location = 0;
            ImGuiIO& io = ImGui::GetIO();
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
            if (location >= 0)
            {
                const float PAD = 10.0f;
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
                ImVec2 work_size = viewport->WorkSize;
                ImVec2 window_pos, window_pos_pivot;
                window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
                window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
                window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
                window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
                ImGui::SetNextWindowViewport(viewport->ID);
                window_flags |= ImGuiWindowFlags_NoMove;
            }
            else if (location == -2)
            {
                // Center window
                ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
                window_flags |= ImGuiWindowFlags_NoMove;
            }

            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
            if (ImGui::Begin("Example: Simple overlay", display_info, window_flags))
            {
                ImGui::Text("Camera Info");
                ImGui::Separator();

                ImGui::Text("X    : %3.3f", current_camera->getPosition().x);
                ImGui::Text("Y    : %3.3f", current_camera->getPosition().y);
                ImGui::Text("Z    : %3.3f", current_camera->getPosition().z);
                ImGui::Text("Pitch: %3.3f", current_camera->getPitchDeg());
                ImGui::Text("Yaw  : %3.3f", current_camera->getHeadingDeg());
                ImGui::Text("Roll : %3.3f", current_camera->getRollDeg());

                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
                    if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
                    if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
                    if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
                    if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
                    if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
                    if (display_info && ImGui::MenuItem("Close")) *display_info = false;
                    ImGui::EndPopup();
                }
            }
            ImGui::End();
        }
    }

    void display_crosshair_overlay() {
        //printf("display_viewport_crosshairs 0000\n");
        if (display_crosshairs) {// Draw crosshairs at center of viewport

            // Center window
            ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize({ vw_cross_hair_overlay.view_dim.x,vw_cross_hair_overlay.view_dim.y });

            if (ImGui::Begin("###ch", &display_crosshairs, vw_cross_hair_overlay.window_flags)) {
                //printf("display_viewport_crosshairs 3333 %f  : %f \n",p1.x,p2.x);
                ImDrawList* draw_list = ImGui::GetWindowDrawList();

                ImVec2 p = ImGui::GetWindowPos();

                vw_cross_hair_overlay.define_crosshair_line();

                ImVec2 p1 = { p.x + vw_cross_hair_overlay.p1.x,p.y + vw_cross_hair_overlay.p1.y };
                ImVec2 p2 = { p.x + vw_cross_hair_overlay.p2.x,p.y + vw_cross_hair_overlay.p2.y };
                ImVec2 p3 = { p.x + vw_cross_hair_overlay.p3.x,p.y + vw_cross_hair_overlay.p3.y };
                ImVec2 p4 = { p.x + vw_cross_hair_overlay.p4.x,p.y + vw_cross_hair_overlay.p4.y };

                draw_list->AddLine(p1, p2, vw_cross_hair_overlay.cross_hair_color, 1.0f);
                draw_list->AddLine(p3, p4, vw_cross_hair_overlay.cross_hair_color, 1.0f);

                ImGui::End();
            }
        }
    }


    void display_gimbal_overlay() {
        if (display_gimbal) {// Draw crosshairs at center of viewport
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = work_pos.x + work_size.x / 2.0f + 95.0f;
            window_pos.y = work_pos.y + work_size.y - 25.0f;
            window_pos_pivot.x = 1.0f;
            window_pos_pivot.y = 1.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            ImGui::SetNextWindowSize({ 180.0f,195.0f });

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

            // To get the orientation of a directional axis displayed pointing in the direction of the world axis using
            // imGuIZMOquat need to take the negative qanternion x y z values of the camera orientation. Because imGuIZMOquat
            // some strange reason gets the forward direction of the subject object such as a camera from the quaternion of 
            // it using foward qanternion = (w,x,y,z), need to also mimic this in defining the qanternion to pass to ImGui::gizmo3D
            glm::quat cquat = current_camera->getOrientationQuat();
            qRot = quat(cquat.w, -cquat.x, -cquat.y, -cquat.z);

            ImGui::SetNextWindowBgAlpha(0.0f); // Transparent background ImGui::gizmo3D overrides this and edited line 576 of imGuIZMOquat.cpp to get rid of it
            if (ImGui::Begin("###gimbal", &display_gimbal, window_flags)) {
                ImGui::gizmo3D("##gizmo1", qRot, IMGUIZMO_DEF_SIZE * 2.0f, imguiGizmo::mode3Axes | imguiGizmo::cubeAtOrigin | imguiGizmo::modeFullAxes);
                ImGui::End();
            }
        }
    }

// =============== keyboard interactions ==============

    bool key_pressed(glfw_window_base_class* glfw_window) {
        return(glfw_window->key_pressed());
    }

    int active_key(glfw_window_base_class* glfw_window) {
        return(glfw_window->active_key());
    }

    // =========== Mouse interactions ===================
    bool mouse_button_pressed(glfw_window_base_class* glfw_window) {
        return(glfw_window->mouse_button_pressed());
    }

    int active_mouse_button(glfw_window_base_class* glfw_window) {
        return(glfw_window->active_mouse_button());
    }

    bool mouse_scroll_active(glfw_window_base_class* glfw_window) {
        return(glfw_window->mouse_scroll_active());
    }
    // =========== Cursor interactions ===================
    bool entered_window(glfw_window_base_class* glfw_window) {
        return(glfw_window->entered_window());
    }

    glm::vec2 get_cursor_position(glfw_window_base_class* glfw_window) {
        return glfw_window->cursor_position();
    }

    // ============= Window interactions ====================
    bool window_is_resized(glfw_window_base_class* glfw_window) {
        return glfw_window->is_window_resized();
    }

    glm::ivec2 window_resize(glfw_window_base_class* glfw_window) {
        return glfw_window->window_resize();
    }

    bool window_focused(glfw_window_base_class* glfw_window) {
        return glfw_window->window_focused();
    }

    bool window_maximized(glfw_window_base_class* glfw_window) {
        return glfw_window->window_maximised();
    }
    // =================================================

    void close() override {
        delete log_panel;
        delete logger;

        oglfw_ImGui.ImGui_end();

        if (glfw_window) glfw_window->close();
        glfwTerminate();
    }

protected:
    glfw_openGL_window_class* glfw_window = nullptr;
    //glfw_openGL_window_class* active_window = nullptr;
    vw_cross_hair_overlay_struct vw_cross_hair_overlay;

    // For imGuIZMO, declare static or global variable or member class quaternion
    quat qRot = quat(1.f, 0.f, 0.f, 0.f);

    bool display_grid        = true;
    bool display_info        = true;
    bool display_gimbal      = true;
    bool display_crosshairs  = true;

    glm::vec4 background_color = { 0.0f,0.0f,0.0f,1.0f };

private:
    afw_Logger_class* logger = nullptr;

    oglfw_camera_base_class *current_camera = nullptr;

    oglFW_ImGui_GLFW_class oglfw_ImGui;

    gflw_event_struct_type gflw_event;

    log_panel_class       *log_panel = nullptr;
    parameter_panel_class  parameter_panel;

    // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
    //buffer_data_panel_class buffer_data_panel;

    data_buffer_panel_class data_buffer_panel;// ++++

    //graphics_pipline_panel_class graphics_pipline_panel;

    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! CRITICAL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    // !!!!!!!!!!! DEFINE THE GEOMETRY PIPELINE EMULATION CLASS HERE THAT !!!!!!!!!!!!
    // !!!!!!!!!!! HAS THE VERTEX AND GEOMETRY EMULATION SHADER WITHIN IT !!!!!!!!!!!!

    sconfw_pipeline_template_base_class sconfw_pipeline;
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    // Vertex and geometry data panels that display information on emulated glsl layouts
    // uniform and variable data to be used for debugging and monitoring purposes.
    scon_vertex_data_panel_class   scon_vertex_data_panel;
    scon_geometry_data_panel_class scon_geometry_data_panel;

    // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
};

int main() {
    shaderCON_class *app = new shaderCON_class;


    app->run();

    exit(0);
}