#pragma once

//#define GLEW_STATIC
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>// Must be first include statement 

#include <FrameWork/AFW/Objects/afw_camera_ui_base.h>
#include <FrameWork/OGLFW/Shader/oglfw_shader.h>

#include <FrameWork/AFW/Window/glfw_window_base.h>

class oglfw_camera_base_class : public afw_camera_ui_base_class {
public:
	oglfw_camera_base_class() {}
	~oglfw_camera_base_class() {}

    //glm::vec3 look_at_position = { 0.0,0.0,0.0 };

    virtual void update_shader_uniforms(GLint shader_id) {
        // NOTE :: this is for testing only:
        // Will need to change for production version

        if (shader_id < 0) {
            std::cout << "oglfw_camera_base_class::update_shader_uniforms : shader_id < 0 " << shader_id << std::endl;
            return;
        }

        oglfw_shader_class shader;

        //shader.set_vec3(shader_id, getPosition(), "camera_position");
        //shader.set_vec3(shader_id, get_forward_dir(), "camera_front_vector");
        //shader.set_vec3(shader_id, get_up_dir(), "camera_up_vector");
        //shader.set_vec3(shader_id, get_right(), "camera_right_vector");

        shader.set_vec3(shader_id, getPosition(), "uCamera_position");
        shader.set_vec3(shader_id, get_forward_dir(), "uCamera_forward");
        shader.set_vec3(shader_id, get_up_dir(), "uCamera_up");
        shader.set_vec3(shader_id, get_right(), "uCamera_right");


        shader.set_mat4(shader_id, get_view_matrix(), "u_view");
        shader.set_mat4(shader_id, get_projection_matrix(), "u_projection");
        shader.set_mat4(shader_id, get_projection_matrix()* get_view_matrix(), "modelViewProjectionMatrix");

    }

    // have processInput be a virtual function to be defined in a derived class
    //This is the callback function for input data, keyboard, mouse etc
    void processInput(GLFWwindow* window,gflw_event_struct_type &glfw_event)
    {
        if (glfw_event.glfw_key_event->key_pressed || glfw_event.glfw_key_event->key_repeat) {
//printf("processInput key pressed\n");
            key_pressed(*glfw_event.glfw_key_event);
            glfw_event.glfw_key_event->reset_key_event();
        }

        if (glfw_event.glfw_key_event->key_released) {
// printf("processInput key pressed\n");
            key_released(*glfw_event.glfw_key_event);
            glfw_event.glfw_key_event->reset_key_event();
        }

        if (glfw_event.glfw_mouse_event->button_pressed) {
            perform_mouse_pressed_action(window, *glfw_event.glfw_mouse_event, *glfw_event.glfw_cursor_event,0);
            glfw_event.glfw_mouse_event->reset_button_event();
        }

        if (glfw_event.glfw_mouse_event->button_released) {
            mouse_button_released(window ,*glfw_event.glfw_mouse_event, *glfw_event.glfw_cursor_event);
            glfw_event.glfw_mouse_event->reset_button_event();
        }

        if (glfw_event.glfw_mouse_event->mouse_scroll_active) {
            mouse_scrolled(*glfw_event.glfw_mouse_event, *glfw_event.glfw_cursor_event);
            glfw_event.glfw_mouse_event->reset_scroll_event();
        }
        
/*
        ///If the 'esc' key was pressed
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            ///Close the window
            glfwSetWindowShouldClose(window, true);
        }

        ///\/////////////////////////CAMERA CONTROLS////////////////////////////////////

            ///\//////////////////
            ///CAMERA POSITION////
            ///\//////////////////

            ///Move front and back
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::FORWARD);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::BACKWARD);
        }

        ///Move left and right
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::LEFT);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::RIGHT);
        }

        ///Move Up and Down
        if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::UP);
        }
        if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::DOWN);
        }

        ///\////////////////////////////////////////////////////////////////////////////

            ///\/////////////////
            ///CAMERA ROTATION///
            ///\/////////////////

            ///Update new Yaw angle
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::RIGHT_SPIN);
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::LEFT_SPIN);
        }

        ///Update new Pitch angle
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::UP_SPIN);
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::DOWN_SPIN);
        }

        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::ROLL_RIGHT);
        }

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        {
            MoveCamera(camera_movement_enum::ROLL_LEFT);
        }
*/
    }

protected:


private:

};

