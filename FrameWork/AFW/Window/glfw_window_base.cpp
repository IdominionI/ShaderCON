//#include "glfw_window_base.h"
//
//
// bool glfw_window_base_class::initialise_callbacks() {
//    if (!window_ptr) {
//        printf("Unable to create initialise vulcan GLFW window callbacks! :: Window not defined as probably not created or deleted.");
//        ofLogError() << "Unable to create initialise vulcan GLFW window callbacks! :: Window not defined as probably not created or deleted.";
//        return false;
//    }
//
//
//    //glfwSetFramebufferSizeCallback(window_ptr, [](GLFWwindow* w, int width, int height) {
//    //    glfw_window_base_class* instance = static_cast<glfw_window_base_class*>(glfwGetWindowUserPointer(w));
//    //    if (instance->m_windowSizeCallback)
//    //    {
//    //        instance->m_windowSizeCallback(width, height);
//    //    }
//    //    });
//
//    //glfwSetCursorPosCallback(window_ptr, [](GLFWwindow* w, double x, double y) {
//    //    glfw_window_base_class* instance = static_cast<glfw_window_base_class*>(glfwGetWindowUserPointer(w));
//    //    if (instance->m_mouseCallBack)
//    //    {
//    //        instance->m_mouseCallBack(x, y);
//    //    }
//    //    });
//
//    //glfwSetKeyCallback(window_ptr, key_callback);
//    //glfwSetKeyCallback(window_ptr, glfw_key_event_class::key_callback);
//    glfwSetKeyCallback(window_ptr, glfw_key_event->key_callback);
//
//   // glfwSetDropCallback(window_ptr, glfw_window_base_class::WindowDropCallback);
//}