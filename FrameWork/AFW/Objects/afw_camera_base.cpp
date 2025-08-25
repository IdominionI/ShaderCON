#include "afw_camera_base.h"

// ################################################################################

void afw_camera_base_class::move_forward() {
    dolly(-movement_rate);
//printf("move_forward\n");
    assign_last_mouse_translations();
}

void afw_camera_base_class::move_backward() {
    dolly(movement_rate);
//printf("move_backward\n");
    assign_last_mouse_translations();
}

void afw_camera_base_class::move_left() {
    truck(-movement_rate);
    assign_last_mouse_translations();
}

void afw_camera_base_class::move_right() {
    truck(movement_rate);
    assign_last_mouse_translations();
}

void afw_camera_base_class::move_up() {
    boom(movement_rate);
    assign_last_mouse_translations();
}

void afw_camera_base_class::move_down() {
    boom(-movement_rate);
    assign_last_mouse_translations();
}

void afw_camera_base_class::rotate_up() {
    tiltDeg(rotation_speed);
//printf("rotate_up\n");
    assign_last_mouse_translations();
}
void afw_camera_base_class::rotate_down() {
    tiltDeg(-rotation_speed);
//printf("rotate_down\n");
    assign_last_mouse_translations();
}
void afw_camera_base_class::rotate_left() {
    panDeg(rotation_speed);
    assign_last_mouse_translations();
}
void afw_camera_base_class::rotate_right() {
    panDeg(-rotation_speed);
    assign_last_mouse_translations();
}
void afw_camera_base_class::roll_right() {
    rollDeg(rotation_speed);
    assign_last_mouse_translations();
}
void afw_camera_base_class::roll_left() {
    rollDeg(-rotation_speed);
    assign_last_mouse_translations();
}

///This function handles the camera movement
void afw_camera_base_class::MoveCamera(camera_movement_enum direction)
{
    switch (direction) {
        case camera_movement_enum::FORWARD:   move_forward(); break;
        case camera_movement_enum::BACKWARD:  move_backward(); break;
        case camera_movement_enum::LEFT:      move_left(); break;
        case camera_movement_enum::RIGHT:     move_right(); break;
        case camera_movement_enum::UP:        move_up(); break;
        case camera_movement_enum::DOWN:      move_down(); break;

        case camera_movement_enum::LEFT_SPIN:  rotate_left(); break;
        case camera_movement_enum::RIGHT_SPIN: rotate_right(); break;
        case camera_movement_enum::UP_SPIN:    rotate_up(); break;
        case camera_movement_enum::DOWN_SPIN:  rotate_down(); break;
        case camera_movement_enum::ROLL_RIGHT: roll_right(); break;
        case camera_movement_enum::ROLL_LEFT:  roll_left(); break;
    }

    //printf("position %f : %f : %f\n", position.x, position.y, position.z);
    //printf("orientation %f : %f : %f\n", getPitchDeg(), getHeadingDeg(), getRollDeg());
}


///This function returns the corresponding lookAt
///matrix of this camera for the vertex shader
glm::mat4 afw_camera_base_class::get_view_matrix()
{
    return glm::lookAt(position, position + get_forward_dir(), get_up_dir());
}

///This function returns the corresponding Projection
///matrix of this camera for the vertex shader
glm::mat4 afw_camera_base_class::get_projection_matrix()
{
    if (camera_type == camera_type_enum::PERSPECTIVE)
    {
        return glm::perspective(glm::radians(fov), aspect_ratio_width / aspect_ratio_height, near_clipping_plane, far_clipping_plane);
    }
    else if (camera_type == camera_type_enum::ORTHOGRAPHIC)
    {
        return glm::ortho(0.0f, aspect_ratio_width, 0.0f, aspect_ratio_height, near_clipping_plane, far_clipping_plane);
    }

    return glm::mat4();

}

//----------------------------------------
glm::vec3 afw_camera_base_class::worldToScreen(glm::vec3 WorldXYZ, view_port_struct_type& viewport) {
	auto CameraXYZ = worldToCamera(WorldXYZ);

	glm::vec3 ScreenXYZ;
	ScreenXYZ.x = (CameraXYZ.x + 1.0f) / 2.0f * viewport.width + viewport.x_pos;
	ScreenXYZ.y = (1.0f - CameraXYZ.y) / 2.0f * viewport.height + viewport.y_pos;
	ScreenXYZ.z = CameraXYZ.z;

	return ScreenXYZ;
}

//----------------------------------------
glm::vec3 afw_camera_base_class::screenToWorld(glm::vec3 ScreenXYZ, view_port_struct_type& viewport) {
	//convert from screen to camera
	glm::vec3 CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x_pos) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f * (ScreenXYZ.y - viewport.y_pos) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	return cameraToWorld(CameraXYZ);
}

//----------------------------------------
glm::vec3 afw_camera_base_class::worldToCamera(glm::vec3 WorldXYZ) {
	auto MVPmatrix = getModelViewProjectionMatrix();
	//if (vFlip) {
	//	MVPmatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.f, -1.f, 1.f)) * MVPmatrix;
	//}
	auto camera = MVPmatrix * glm::vec4(WorldXYZ, 1.0);
	return glm::vec3(camera) / camera.w;

}

//----------------------------------------
glm::vec3 afw_camera_base_class::cameraToWorld(glm::vec3 CameraXYZ) {
	auto MVPmatrix = getModelViewProjectionMatrix();
	//if (vFlip) {
	//	MVPmatrix = glm::scale(glm::mat4(1.0), glm::vec3(1.f, -1.f, 1.f)) * MVPmatrix;
	//}
	auto world = glm::inverse(MVPmatrix) * glm::vec4(CameraXYZ, 1.0);
	return glm::vec3(world) / world.w;
}

//----------------------------------------
void afw_camera_base_class::calcClipPlanes(const view_port_struct_type& viewport) {
	// autocalculate near/far clip planes if not set by user
	//if (nearClip == 0 || farClip == 0) {
	//	float dist = getImagePlaneDistance(viewport);
	//	nearClip = (nearClip == 0) ? dist / 100.0f : nearClip;
	//	farClip = (farClip == 0) ? dist * 10.0f : farClip;
	//}

	if (near_clipping_plane == 0 || far_clipping_plane == 0) {
		float dist = getImagePlaneDistance(viewport);
		near_clipping_plane = (near_clipping_plane == 0) ? dist / 100.0f : near_clipping_plane;
		far_clipping_plane = (far_clipping_plane == 0) ? dist * 10.0f : far_clipping_plane;
	}
}

// ###################### Camera alignment ###########################
void afw_camera_base_class::align_to_plane(camera_ailgnment_type_enum camera_ailgnment) { // not tested
	glm::vec3 alignment_vector = { 0.0f,0.0f,0.0f };

	glm::vec3 mPosition = getPosition();
	glm::vec3 mlook_at = glm::normalize(get_forward_dir());

	float look_at_distance = glm::length(mlook_at) * 10.0f;

	if (look_at_distance == 0.0f) {
		printf("Cannot align to requested plane as camera is looking at itself in its own location.\n");
		return;
	}

	//std::cout << "vw_camera_class::align_to_plane 0000 : " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;

	mlook_at += mPosition;

	//std::cout << "vw_camera_class::align_to_plane 1111: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;

	switch (camera_ailgnment) {
	case camera_ailgnment_type_enum::yz_plane: alignment_vector = { mPosition.x,mlook_at.y,mlook_at.z }; break;
	case camera_ailgnment_type_enum::xz_plane: alignment_vector = { mlook_at.x,mPosition.y,mlook_at.z }; break;
	case camera_ailgnment_type_enum::xy_plane: alignment_vector = { mlook_at.x,mlook_at.y,mPosition.z }; break;
	}

	//std::cout << "vw_camera_class::align_to_plane 2222: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<alignment_vector.x <<":"<< alignment_vector.y<<":"<<alignment_vector.z<<":" << std::endl;
		//lookAt(alignment_vector);
	glm::lookAt(getPosition(), alignment_vector, get_up_dir());

	//mlook_at = getLookAtDir();
	mlook_at = get_forward_dir();
	//std::cout << "vw_camera_class::align_to_plane 3333: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;


	assign_last_mouse_translations();
}
