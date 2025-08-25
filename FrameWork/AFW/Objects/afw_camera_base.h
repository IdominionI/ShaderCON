#pragma once

///GLM
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

#include <FrameWork/AFW/Graphics/afw_node_base.h>

struct view_port_struct_type {
    int x_pos = 0;
    int y_pos = 0;
    size_t width = 0;
    size_t height = 0;

    void setup(int x, int y, size_t w, size_t h) {
        x_pos = x;
        y_pos = y;
        width = w;
        height = h;
    }
    void set_size(size_t w, size_t h) {
        width = w;
        height = h;
    }
    void set_position(int x, int y) {
        x_pos = x;
        y_pos = y;
    }

    //===============================

    bool is_zero() const {
        if (width == 0 || height == 0)
            return true;
        else
            return false;
    }

    bool inside(size_t x, size_t y) {
        if (x > x_pos && x < (x_pos + width)) {
            if (y > y_pos && x < (y_pos + height))
                return true;
        }
        return false;
    }

    glm::vec2 getCenter() {
        return { (x_pos + width) / 2.0f, (y_pos + height) / 2.0f };
    }
};

enum class camera_ailgnment_type_enum { none, xy_plane, xz_plane, yz_plane };
enum class camera_movement_mode_enum { free, xy_plane, xz_plane, yz_plane, orbital, none };

enum class camera_movement_enum {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    LEFT_SPIN,
    RIGHT_SPIN,
    UP_SPIN,
    DOWN_SPIN,
    ROLL_RIGHT,
    ROLL_LEFT
};

enum class camera_type_enum { ORTHOGRAPHIC, PERSPECTIVE };

enum class camera_movement_type_enum { FREE_ROAM, ANCHORED };



struct camera_attributes_struct_type {
    float     aspect_ratio = 4.0f / 3.0f; // only used when forceAspect=true, = w / h

    ///Field of vision
    float fov = 45.0f;

    ///Camera Settings
    camera_type_enum camera_type = camera_type_enum::PERSPECTIVE;
    camera_movement_type_enum camera_movement_type = camera_movement_type_enum::FREE_ROAM;
    float near_clipping_plane = 0.1f;
    float far_clipping_plane = 10000.0f;
    float aspect_ratio_width = 800.0f;
    float aspect_ratio_height = 600.0f;

    ///Angles
    float pitch = 0.0f;
    float yaw = 0.0f;
    float roll = 0.0f;

    ///Movement and Rotation Speed
    float movement_rate = .01f;
    float rotation_speed = 1.0f;
    float mouse_scroll_sensitivity = 1.0f;

    void set_camera_type(camera_type_enum mtype) { camera_type = mtype; }
    void set_movment_type(camera_movement_type_enum mtype) { camera_movement_type = mtype; }
    void set_field_of_view(float mfov) { fov = mfov; }
    void set_near_clipping_plane(float ncp) { near_clipping_plane = ncp; }
    void set_far_clipping_plane(float fcp) { far_clipping_plane = fcp; }
    void set_pitch(float mpitch) { pitch = mpitch; }
    void set_yaw(float myaw) { yaw = myaw; }
    void set_roll(float mroll) { roll = mroll; }
    void set_movement_rate(float mr) { movement_rate = mr; }
    void set_rotation_rate(float rr) { rotation_speed = rr; }
    void set_mouse_scroll_sensitivity(float ss) { mouse_scroll_sensitivity = ss; }

    void set_aspect_ratio(float ar) { aspect_ratio = ar; }
    void set_aspect_ratio(float width, float height) {
        aspect_ratio = width / height;
        aspect_ratio_width = width;
        aspect_ratio_height = height;
    }

    camera_type_enum get_camera_type() { return camera_type; }
    camera_movement_type_enum get_movment_type() { return  camera_movement_type; }
    float get_field_of_view() { return  fov; }
    float get_near_clipping_plane() { return  near_clipping_plane; }
    float get_far_clipping_plane() { return  far_clipping_plane; }
    float get_pitch() { return  pitch; }
    float get_yaw() { return  yaw; }
    float get_roll() { return  roll; }
    float get_movement_rate() { return  movement_rate; }
    float get_mouse_scroll_sensitivity() { return mouse_scroll_sensitivity; }
    float get_rotation_rate() { return  rotation_speed; }
    float get_aspect_ratio() { return  aspect_ratio; }
};


class afw_camera_base_class : public camera_attributes_struct_type, public afw_node_base_class
{
public:
    ///Constructor
    afw_camera_base_class() {
        initialise();
    }

    ~afw_camera_base_class() {}

    bool display_active = false;
    bool display_overlay = false;
    bool view_port_active = false; // If false the whole window is the viewport otherwise use a defined view port

    view_port_struct_type view_port;

    virtual void initialise(camera_attributes_struct_type settings, glm::vec3 camPos) {}
    void initialise() {
        camera_type = camera_type_enum::PERSPECTIVE;
        camera_movement_type = camera_movement_type_enum::FREE_ROAM;

        near_clipping_plane = 0.1f;
        far_clipping_plane = 1000.0f;
        aspect_ratio_width = 800.0f;
        aspect_ratio_height = 600.0f;

        fov = 45.0f;

        ///Angles
        pitch = 0.0f;
        yaw = 0.0f;
        roll = 0.0f;

        ///Movement and Rotation Speed
        movement_rate = .1f;
        rotation_speed = 1.0f;

        glm::vec3 camPos = glm::vec3(0.01f, 1.0f, 5.0f);
        setPosition(camPos);
        set_orientation(0.01f, 0.01f, 0.01f);
    }

    ///Public Functions
    glm::mat4 get_view_matrix();
    glm::mat4 get_projection_matrix();


    void define_camera_settings(camera_attributes_struct_type _camera_settings) {
        set_field_of_view(_camera_settings.fov);
        set_near_clipping_plane(_camera_settings.near_clipping_plane);
        set_far_clipping_plane(_camera_settings.far_clipping_plane);
        set_aspect_ratio(_camera_settings.aspect_ratio);
    }

    // Add more camera functions to be performed using the keyboard here : TO DO
    // rotation
    // first person/orbital rotation mode change
    // move in a direction using keyboard while while able to look around with mouse

    // Need this so does not revert any camera movements or orientations 
    // not using the mouse to last location using mouse interaction.
    void assign_last_mouse_translations() {
        lastPressAxisX = getXAxis();
        lastPressAxisY = getYAxis();
        lastPressAxisZ = getZAxis();
        lastPressPosition = getGlobalPosition();
        lastPressOrientation = getGlobalOrientation();
    }


    glm::mat4 getModelViewProjectionMatrix() {
        return get_projection_matrix() * get_view_matrix();
    }

    float getImagePlaneDistance(const view_port_struct_type& viewport) const {
        return viewport.height / (2.0f * tanf(glm::pi<float>() * fov / 360.0f));
    }

    // !!!!!!!!!!!!!!!!!!! FOLLOWING FUNCTIONS NEED TESTING !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    /// \brief Obtain the screen coordinates of a point in the 3D world.
///
/// Takes an (X,Y,Z) point in your 3D world, encoded as an glm::vec3,
/// and returns the location (also as an glm::vec3) where this point would
/// appear on your (two-dimensional) display. The screen position's "Z
/// coordinate" is set to be the same as your camera's.
///
/// \param WorldXYZ A 3D point in the world, whose screen coordinates you wish to know. 
/// \param viewport (Optional) A viewport. The default is ofGetCurrentViewport(). 
/// \returns An glm::vec3 containing the screen coordinates of your 3D point of interest.
    glm::vec3 worldToScreen(glm::vec3 WorldXYZ, view_port_struct_type& viewport);
    glm::vec3 worldToScreen(glm::vec3 WorldXYZ) {
        return worldToScreen(WorldXYZ, view_port);
    }

    /// \brief Obtain the coordinates, in the 3D world, of a 2D point presumed to be on your screen.
    ///
    /// Takes a pixel location on your screen, encoded in an glm::vec3,
    /// and returns (also as an glm::vec3) the 3D world coordinates of that point.
    /// You'll also need to specify a Z value when providing your screen point. 
    /// This Z value is interpreted as a distance into or away from the screen. 
    ///
    /// \param ScreenXYZ A point on your screen, whose 3D world coordinates you wish to know.
    glm::vec3 screenToWorld(glm::vec3 ScreenXYZ, view_port_struct_type& viewport);
    glm::vec3 screenToWorld(glm::vec3 ScreenXYZ) {
        return screenToWorld(ScreenXYZ, view_port);
    }

    /// \todo worldToCamera()
    glm::vec3 worldToCamera(glm::vec3 WorldXYZ);

    /// \todo cameraToWorld()
    glm::vec3 cameraToWorld(glm::vec3 CameraXYZ);

    void calcClipPlanes(const view_port_struct_type& viewport);

    // ###################### Camera alignment ###########################
    void align_to_plane(camera_ailgnment_type_enum camera_ailgnment); // not tested

    view_port_struct_type getViewport() { return view_port; }

    ///Function to get keyboard input and move the camera
    void MoveCamera(camera_movement_enum direction);

    void move_forward();
    void move_backward();
    void move_left();
    void move_right();
    void move_up();
    void move_down();

    void rotate_up();
    void rotate_down();
    void rotate_left();
    void rotate_right();
    void roll_right();
    void roll_left();

    void look_at(glm::vec3 location) {
        look_at_location(location);
        //glm::lookAt(position, location, get_forward_dir());
    }
    void look_at(float x, float y, float z) {
        look_at({ x,y,z });
    }

    bool is_orthographic() {
        if (camera_type == camera_type_enum::ORTHOGRAPHIC)
            return true;
        else
            return false;
    }

    glm::vec3 lastPressAxisX = { 0.0f,0.0f,0.0f };
    glm::vec3 lastPressAxisY = { 0.0f,0.0f,0.0f };
    glm::vec3 lastPressAxisZ = { 0.0f,0.0f,0.0f };
    glm::vec3 lastPressPosition = { 0.0f,0.0f,0.0f };
    glm::quat lastPressOrientation{ 0.0,0.0,0.0,0.0 };
    glm::vec2 lastPressMouse = { 0.0f,0.0f };
};


