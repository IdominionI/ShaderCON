#include "afw_camera_ui_base.h"

#include <FrameWork/AFW/Application/afw_global.h>
//#include "afw_camera_ui_base_class.h"
//#include "vw_camera.h"
//#include "ofMath.h"
//#include "ofUtils.h"
//#include "ofGraphicsBaseTypes.h"

#define GLM_FORCE_CTOR_INIT
#include "glm/gtx/vector_angle.hpp"
#include <limits>

#include <FrameWork/AFW/Events/afw_user_bindings.h>

//#include <glm/geometric.hpp>

// when an afw_camera_ui_base_class is moving due to momentum, this keeps it
// from moving forever by assuming small values are zero.
static const float minDifference = 0.1e-5f;

// this is the default on windows os
static const unsigned long doubleclickTime = 200;

//----------------------------------------
afw_camera_ui_base_class::afw_camera_ui_base_class() {
	initialise_ui();
}

void afw_camera_ui_base_class::initialise_ui() {
	reset();
	sensitivityTranslate = { 1,1,1 };
	sensitivityRot = { 1,1,1 };

	addInteraction(TransformType::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_LEFT, doTranslationKey);
	addInteraction(TransformType::TRANSFORM_ROTATE, OF_MOUSE_BUTTON_LEFT);
	addInteraction(TransformType::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);
	addInteraction(TransformType::TRANSFORM_TRANSLATE_XY, OF_MOUSE_BUTTON_MIDDLE);

	//keys for apllicaton 
	add_key_interaction(ACTION_TYPE_CLOSE_APPLICATON, GLFW_PRESS, KEY_INTERACTION_ENABLED, KEY_CLOSE_APPLICATON, NO_KEY_MODIFIER);

	//Keys for camera movement
	add_key_interaction(ACTION_TYPE_MOVEMENT_FORWARD, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_FORWARD, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_BACKWARD, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_BACKWARD, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_RIGHT, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_RIGHT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_LEFT, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_LEFT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_UP, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_UP, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_MOVEMENT_DOWN, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_MOVEMENT_DOWN, NO_KEY_MODIFIER);

	//Keys for camera rotation
	add_key_interaction(ACTION_TYPE_PITCH_UP, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_PITCH_UP, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_PITCH_DOWN, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_PITCH_DOWN, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_YAW_LEFT, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_YAW_LEFT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_YAW_RIGHT, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_YAW_RIGHT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_ROLL_LEFT, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_ROLL_LEFT, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_ROLL_RIGHT, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_ROLL_RIGHT, NO_KEY_MODIFIER);


	add_key_interaction(ACTION_TYPE_CAMERA_ALIGN_XY_PLANE, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_CAMERA_ALIGN_PLANE, NO_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_CAMERA_ALIGN_PLANE, ALT_KEY_MODIFIER);
	add_key_interaction(ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE, AFW_PRESS_REPEAT, KEY_INTERACTION_ENABLED, KEY_CAMERA_ALIGN_PLANE, CONTROL_KEY_MODIFIER);

	add_mouse_interaction(ACTION_TYPE_CAMERA_ROTATE, GLFW_PRESS, MOUSE_INTERACTION_ENABLED, MOUSE_PITCH_YAW_ROTATION, NO_KEY_MODIFIER);
	add_mouse_interaction(ACTION_TYPE_CAMERA_ROLL, GLFW_PRESS, MOUSE_INTERACTION_ENABLED, MOUSE_ROLL_ROTATION, NO_KEY_MODIFIER);
	add_mouse_interaction(ACTION_TYPE_CAMERA_TRANSLATE, NULL, MOUSE_INTERACTION_ENABLED, MOUSE_FB_MOVEMENT, NO_KEY_MODIFIER);


	//	for (size_t i = 0; i < key_interactions.size();i++) {
	//printf("add_key_interaction ZZ, %i  : %i : %i : % i\n", key_interactions[i].action, key_interactions[i].key_code, key_interactions[i].modifiers, key_interactions[i].interaction_id);
	//	}
}

//----------------------------------------
// 
// VWCUSTOM ADD +++++++++++++++++++++++++++++++++

void afw_camera_ui_base_class::move_in_global_direction(glm::vec3 direction) { // not tested
	glm::vec3 new_position = getPosition() + direction * movement_multiplier;
	setPosition(new_position);
	assign_last_mouse_translations();
}

void afw_camera_ui_base_class::move_in_local_direction(glm::vec3 direction) { // not tested
	truck(direction.x * movement_multiplier);
	boom(direction.y * movement_multiplier);
	dolly(direction.z * movement_multiplier);
	assign_last_mouse_translations();
}

//void afw_camera_ui_base_class::move_forward() {
//	dolly(-movement_factor);
//	assign_last_mouse_translations();
//}
//
//void afw_camera_ui_base_class::move_backward() {
//	dolly(movement_factor);
//	assign_last_mouse_translations();
//}
//
//void afw_camera_ui_base_class::move_left() {
//	truck(-movement_factor);
//	assign_last_mouse_translations();
//}
//
//void afw_camera_ui_base_class::move_right() {
//	truck(movement_factor);
//	assign_last_mouse_translations();
//}
//
//void afw_camera_ui_base_class::move_up() {
//	boom(-movement_factor);
//	assign_last_mouse_translations();
//}
//
//void afw_camera_ui_base_class::move_down() {
//	boom(movement_factor);
//	assign_last_mouse_translations();
//}

// ###################### Camera alignment ###########################
void afw_camera_ui_base_class::align_to_plane(camera_ailgnment_type_enum camera_ailgnment) { // not tested
	glm::vec3 alignment_vector = { 0.0f,0.0f,0.0f };

	glm::vec3 mPosition = getPosition();
	//glm::vec3 mlook_at = glm::normalize(getLookAtDir());
	glm::vec3 mlook_at = glm::normalize(get_forward_dir());

	float look_at_distance = glm::length(mlook_at) * 10.0f;

	if (look_at_distance == 0.0f) {
		std::cout << "Cannot align to requested plane as camera is looking at itself in its own location.\n";
		return;
	}

	//std::cout << "afw_camera_ui_base_class::align_to_plane 0000 : " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;

	mlook_at += mPosition;

	//std::cout << "afw_camera_ui_base_class::align_to_plane 1111: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;

	switch (camera_ailgnment) {
	case camera_ailgnment_type_enum::yz_plane: alignment_vector = { mPosition.x,mlook_at.y,mlook_at.z }; break;
	case camera_ailgnment_type_enum::xz_plane: alignment_vector = { mlook_at.x,mPosition.y,mlook_at.z }; break;
	case camera_ailgnment_type_enum::xy_plane: alignment_vector = { mlook_at.x,mlook_at.y,mPosition.z }; break;
	}

	//std::cout << "afw_camera_ui_base_class::align_to_plane 2222: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<alignment_vector.x <<":"<< alignment_vector.y<<":"<<alignment_vector.z<<":" << std::endl;

	look_at(alignment_vector);

	mlook_at = get_forward_dir();
	//std::cout << "afw_camera_ui_base_class::align_to_plane 3333: " << mPosition.x <<":"<< mPosition.y<<":"<<mPosition.z<<"::::"<<mlook_at.x <<":"<< mlook_at.y<<":"<<mlook_at.z<<":" << std::endl;


	assign_last_mouse_translations();
}






// END VWCUSTOM ADD +++++++++++++++++++++++++++++++++

//----------------------------------------
/*
//void afw_camera_ui_base_class::update(ofEventArgs& args) {
void afw_camera_ui_base_class::update() {
	if (this->view_port.is_zero()) {
		viewport = getViewport();
	}
	if (!bDistanceSet && bAutoDistance) {
		setDistance(getImagePlaneDistance(viewport), true);
	}
	if (bMouseInputEnabled && events) {
		if (events->getMousePressed()) {
			updateMouse(glm::vec2(events->getMouseX(), events->getMouseY()));
		}
		if (currentTransformType == TransformType::TRANSFORM_ROTATE) {
			updateRotation();
		}
		else if (currentTransformType == TransformType::TRANSFORM_TRANSLATE_XY ||
			currentTransformType == TransformType::TRANSFORM_TRANSLATE_Z ||
			currentTransformType == TransformType::TRANSFORM_SCALE) {
			updateTranslation();
		}
	}
}
*/
//----------------------------------------
//void afw_camera_ui_base_class::begin(const view_port_struct_type& _viewport) {
//	if (!bEventsSet) {
//		setEvents(ofEvents());
//	}
//	viewport = _viewport;
//	vw_camera_base_class::begin(viewport);
//}

//----------------------------------------
void afw_camera_ui_base_class::reset() {
	target.resetTransform();

	target.setPosition(0, 0, 0);
	look_at(target.getGlobalPosition());

	//resetTransform();
	//setPosition({ 0.f,0.f,0.f });
	set_orientation(0.0f, 0.0f, 0.0f);
	setScale({ 1.f,1.f,1.f });

	setPosition({ 0.0f, 0.0f, lastDistance });

	rot = { 0,0,0 };
	translate = { 0,0,0 };

	if (bAutoDistance) {
		bDistanceSet = false;
	}
	bApplyInertia = false;
	currentTransformType = TransformType::TRANSFORM_NONE;
}

//----------------------------------------
void afw_camera_ui_base_class::setTarget(const glm::vec3& targetPoint) {
	target.setPosition(targetPoint);
	look_at(target.getGlobalPosition());
}

//----------------------------------------
void afw_camera_ui_base_class::setTarget(afw_graphics_node_base_class& targetNode) {
	target = targetNode;
	look_at(target.getGlobalPosition());
}

//----------------------------------------
const afw_graphics_node_base_class& afw_camera_ui_base_class::getTarget() const {
	return target;
}

//----------------------------------------
void afw_camera_ui_base_class::setDistance(float distance) {
	setDistance(distance, true);
}

//----------------------------------------
void afw_camera_ui_base_class::setDistance(float distance, bool save) {//should this be the distance from the camera to the target?
	if (distance > 0.0f) {
		if (save) {
			this->lastDistance = distance;
		}
		setPosition(target.getPosition() + (distance * getZAxis()));
		bDistanceSet = true;
	}
}

//----------------------------------------
float afw_camera_ui_base_class::getDistance() const {
	return glm::distance(target.getPosition(), getPosition());
}

//----------------------------------------
void afw_camera_ui_base_class::setAutoDistance(bool bAutoDistance) {
	this->bAutoDistance = bAutoDistance;
	if (bAutoDistance) {
		bDistanceSet = false;
	}
}

//----------------------------------------
void afw_camera_ui_base_class::setDrag(float drag) {
	this->drag = drag;
}

//----------------------------------------
float afw_camera_ui_base_class::getDrag() const {
	return drag;
}

//----------------------------------------
void afw_camera_ui_base_class::setTranslationKey(char key) {
	doTranslationKey = key;
}

//----------------------------------------
char afw_camera_ui_base_class::getTranslationKey() const {
	return doTranslationKey;
}

//----------------------------------------
void afw_camera_ui_base_class::enableMouseInput() {
	//if (!bMouseInputEnabled && events) {
	//	listeners.push(events->update.newListener(this, &afw_camera_ui_base_class::update));
	//	listeners.push(events->mousePressed.newListener(this, &afw_camera_ui_base_class::mousePressed));
	//	listeners.push(events->mouseReleased.newListener(this, &afw_camera_ui_base_class::mouseReleased));
	//	listeners.push(events->mouseScrolled.newListener(this, &afw_camera_ui_base_class::mouseScrolled));
	//}
	//// if enableMouseInput was called within ofApp::setup()
	//// `events` will still carry a null pointer, and bad things
	//// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = true;
	//// setEvents() is called upon first load, and will make sure 
	//// to enable the mouse input once the camera is fully loaded.
}

//----------------------------------------
void afw_camera_ui_base_class::disableMouseInput() {
	//if (bMouseInputEnabled && events) {
	//	listeners.unsubscribeAll();
	//}
	// if disableMouseInput was called within ofApp::setup()
	// `events` will still carry a null pointer, and bad things
	// will happen. Therefore we only update the flag. 
	bMouseInputEnabled = false;
	// setEvents() is called upon first load, and will make sure 
	// to enable the mouse input once the camera is fully loaded.
}
//----------------------------------------
bool afw_camera_ui_base_class::getMouseInputEnabled() const {
	return bMouseInputEnabled;
}

// VWCUSTOM ADD ++++++++++++
void afw_camera_ui_base_class::enable_keyboard_input() {
	//if (!keyboard_input_enabled && events) {
	//	key_listeners.push(events->keyPressed.newListener(this, &afw_camera_ui_base_class::key_pressed));
	//	key_listeners.push(events->keyReleased.newListener(this, &afw_camera_ui_base_class::key_released));
	//}

	keyboard_input_enabled = true;
}

void afw_camera_ui_base_class::disable_keyboard_input() {
	//if (keyboard_input_enabled && events) {
	//	key_listeners.unsubscribeAll();
	//}

	keyboard_input_enabled = false;
}

void afw_camera_ui_base_class::key_pressed(glfw_key_event_class& keyEvent) {
	//std::cout <<" afw_camera_ui_base_class::keyPressed : 000 : " << id << "  :  " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;
	if (!is_key_interaction_enabled(keyEvent.type, keyEvent.key_code, keyEvent.modifiers)) return;
	int interaction_id = get_key_interaction_id(keyEvent.type, keyEvent.key_code, keyEvent.modifiers);
	perform_key_pressed_action(interaction_id);
}

void afw_camera_ui_base_class::key_released(glfw_key_event_class& keyEvent) {
	//std::cout <<" afw_camera_ui_base_class::keyReleased : 000 : " << id << "  :  " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;
	if (!is_key_interaction_enabled(keyEvent.type, keyEvent.key_code, keyEvent.modifiers)) return;
	int interaction_id = get_key_interaction_id(keyEvent.type, keyEvent.key_code, keyEvent.modifiers);
	perform_key_released_action(interaction_id);
}

void afw_camera_ui_base_class::perform_key_pressed_action(int interaction_id) {
	//char c = keyEvent.key;
	//std::cout <<" afw_camera_ui_base_class::perform_key_action  : " << c << " ::: " << keyEvent.key << "  :  " << keyEvent.key_code << " : " << keyEvent.scancode << " : " << keyEvent.codepoint << std::endl;

	switch (interaction_id) {
		// movement this needs to be decided what are the plane space
	case ACTION_TYPE_MOVEMENT_FORWARD:  if (!(movement == camera_movement_mode_enum::xz_plane)) move_forward();  break;
	case ACTION_TYPE_MOVEMENT_BACKWARD: if (!(movement == camera_movement_mode_enum::xz_plane)) move_backward(); break;
	case ACTION_TYPE_MOVEMENT_RIGHT:    if (!(movement == camera_movement_mode_enum::yz_plane)) move_right();    break;
	case ACTION_TYPE_MOVEMENT_LEFT:     if (!(movement == camera_movement_mode_enum::yz_plane)) move_left();     break;
	case ACTION_TYPE_MOVEMENT_UP:       if (!(movement == camera_movement_mode_enum::xy_plane)) move_up();       break;
	case ACTION_TYPE_MOVEMENT_DOWN:     if (!(movement == camera_movement_mode_enum::xy_plane)) move_down();     break;

		//case ACTION_TYPE_PITCH_UP:          if (pitch_rotation) tiltDeg(0.5);  break;
		//case ACTION_TYPE_PITCH_DOWN:	    if (pitch_rotation) tiltDeg(-0.5); break;
		//case ACTION_TYPE_YAW_LEFT:          if (yaw_rotation)   panDeg(0.5);     break;
		//case ACTION_TYPE_YAW_RIGHT:		    if (yaw_rotation)   panDeg(-0.5);    break;
		//case ACTION_TYPE_ROLL_LEFT:         if (roll_rotation)  rollDeg(-0.5);      break;
		//case ACTION_TYPE_ROLL_RIGHT:		if (roll_rotation)  rollDeg(0.5);     break;

	case ACTION_TYPE_PITCH_UP:          if (pitch_rotation) key_rotate_camera_pitch(1.0f);  break;
	case ACTION_TYPE_PITCH_DOWN:	    if (pitch_rotation) key_rotate_camera_pitch(-1.0f); break;
	case ACTION_TYPE_YAW_LEFT:          if (yaw_rotation)   key_rotate_camera_yaw(1.0f);     break;
	case ACTION_TYPE_YAW_RIGHT:		    if (yaw_rotation)   key_rotate_camera_yaw(-1.0f);    break;
	case ACTION_TYPE_ROLL_LEFT:         if (roll_rotation)  key_rotate_camera_roll(-1.0f);      break;
	case ACTION_TYPE_ROLL_RIGHT:		if (roll_rotation)  key_rotate_camera_roll(1.0f);     break;

		// interface tools ::: TO DO ???? 
		////case ACTION_TYPE_DRAW_AXIS:           toggle_display_axis();        break;
		////case ACTION_TYPE_DRAW_GRID:           toggle_display_grid();	    break;
		////case ACTION_TYPE_DRAW_XY_GRID:        toggle_display_xy_grid();     break;
		////case ACTION_TYPE_DRAW_XZ_GRID:        toggle_display_xz_grid();     break;
		////case ACTION_TYPE_DRAW_YZ_GRID:        toggle_display_yz_grid();     break;
		////case ACTION_TYPE_DISPLAY_FPS:         toggle_FPS_display();	        break;
		////case ACTION_TYPE_DISPLAY_CAMERA_INFO: toggle_camera_info_display(); break;
		////case ACTION_TYPE_DISPLAY_CROSS_HAIRS: toggle_cross_hairs_display(); break;
		////case ACTION_TYPE_ENABLE_TEXT:         toggle_enable_all_text();     break;
		////case ACTION_TYPE_SAVE_SCREENSHOT:     save_snapShot();              break;

			// Camera alignment
	case ACTION_TYPE_CAMERA_ALIGN_XY_PLANE: align_to_plane(camera_ailgnment_type_enum::xy_plane); break;
	case ACTION_TYPE_CAMERA_ALIGN_XZ_PLANE: align_to_plane(camera_ailgnment_type_enum::xz_plane); break;
	case ACTION_TYPE_CAMERA_ALIGN_YZ_PLANE: align_to_plane(camera_ailgnment_type_enum::yz_plane); break;

		//Add more keyboard-mouse interactions here
	}

}

void afw_camera_ui_base_class::perform_key_released_action(int interaction_id) {

}
//END VWCUSTOM ADD+++++++++++


//----------------------------------------
/*
void afw_camera_ui_base_class::setEvents(ofCoreEvents& _events) {
	// If en/disableMouseInput were called within ofApp::setup(),
	// bMouseInputEnabled will tell us about whether the camera
	// mouse input needs to be initialised as enabled or disabled.
	// we will still set `events`, so that subsequent enabling
	// and disabling can work.

	// we need a temporary copy of bMouseInputEnabled, since it will
	// get changed by disableMouseInput as a side-effect.
	bool wasMouseInputEnabled = bMouseInputEnabled;// || !events;

	disableMouseInput();
	events = &_events;
	if (wasMouseInputEnabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enableMouseInput();
	}

	// VWCUSTOM ADD ++++++++++++
	bool was_key_board_input_enabled = keyboard_input_enabled;// || !events;

	disable_keyboard_input();
	if (was_key_board_input_enabled) {
		// note: this will set bMouseInputEnabled to true as a side-effect.
		enable_keyboard_input();
	}
	//END VWCUSTOM ADD+++++++++++

	bEventsSet = true;
}
*/
//----------------------------------------
void afw_camera_ui_base_class::setRotationSensitivity(const glm::vec3& sensitivity) {
	sensitivityRot = sensitivity;
	//sensitivityRot = sensitivity * rotation_speed;// VWCUSTOM Modify MMMMM
}
//----------------------------------------
void afw_camera_ui_base_class::setRotationSensitivity(float x, float y, float z) {
	setRotationSensitivity({ x,y,z });
}
//----------------------------------------
void afw_camera_ui_base_class::setTranslationSensitivity(const glm::vec3& sensitivity) {
	sensitivityTranslate = sensitivity;
}
//----------------------------------------
void afw_camera_ui_base_class::setTranslationSensitivity(float x, float y, float z) {
	sensitivityTranslate = { x,y,z };
}

//----------------------------------------
void afw_camera_ui_base_class::enableMouseMiddleButton() {
	bEnableMouseMiddleButton = true;
}

//----------------------------------------
void afw_camera_ui_base_class::disableMouseMiddleButton() {
	bEnableMouseMiddleButton = false;
}

//----------------------------------------
bool afw_camera_ui_base_class::getMouseMiddleButtonEnabled() const {
	return bEnableMouseMiddleButton;
}

//----------------------------------------
glm::vec3 afw_camera_ui_base_class::up() const {
	if (bRelativeYAxis) {
		if (bApplyInertia) {
			return getYAxis();
		}
		else {
			return lastPressAxisY;
		}
	}
	else {
		return upAxis;
	}
}

//----------------------------------------
void afw_camera_ui_base_class::setRelativeYAxis(bool relative) {
	bRelativeYAxis = relative;
}

//----------------------------------------
bool afw_camera_ui_base_class::getRelativeYAxis() const {
	return bRelativeYAxis;
}

//----------------------------------------
void afw_camera_ui_base_class::setUpAxis(const glm::vec3& _up) {
	upAxis = _up;
}

//----------------------------------------
const glm::vec3& afw_camera_ui_base_class::getUpAxis() const {
	return upAxis;
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
void afw_camera_ui_base_class::enableInertia() {
	doInertia = true;
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
void afw_camera_ui_base_class::disableInertia() {
	doInertia = false;
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
bool afw_camera_ui_base_class::getInertiaEnabled() const {
	return doInertia;
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
void afw_camera_ui_base_class::updateTranslation() {
	if (bApplyInertia) {
		translate *= drag;
		if (std::abs(translate.x) <= minDifference && std::abs(translate.y) <= minDifference && std::abs(translate.z) <= minDifference) {
			translate = { 0,0,0 };
			bApplyInertia = false;
			currentTransformType = TransformType::TRANSFORM_NONE;

			bIsScrolling = false;
			return;
		}
		move((getXAxis() * translate.x) + (getYAxis() * translate.y) + (getZAxis() * translate.z));
	}
	if (currentTransformType == TransformType::TRANSFORM_TRANSLATE_XY ||
		currentTransformType == TransformType::TRANSFORM_TRANSLATE_Z ||
		currentTransformType == TransformType::TRANSFORM_SCALE) {
		//if (getOrtho()) {
		if (is_orthographic()) {
			//In ortho mode moving along the z axis has no effect besides clipping.
			// Instead, scale is applied to achieve the effect of getting near or far from the target.
			glm::vec3 mousePre;
			bool bDoScale = (currentTransformType == TransformType::TRANSFORM_SCALE || currentTransformType == TransformType::TRANSFORM_TRANSLATE_Z);
			if (bDoScale) {
				mousePre = screenToWorld(glm::vec3((bIsScrolling ? mouseAtScroll : lastPressMouse), 0));
			}
			move(glm::vec3(lastPressAxisX * translate.x) + (lastPressAxisY * translate.y));
			if (bDoScale) {
				setScale(getScale() + translate.z);
				// this move call is to keep the scaling centered below the mouse.
				move(mousePre - screenToWorld(glm::vec3((bIsScrolling ? mouseAtScroll : lastPressMouse), 0)));
			}
		}
		else {
			move(glm::vec3(lastPressAxisX * translate.x) + (lastPressAxisY * translate.y) + (lastPressAxisZ * translate.z));
		}
	}
	if (bIsScrolling) {
		//this it to avoid the transformation to keep on after scrolling ended.
		currentTransformType = TransformType::TRANSFORM_NONE;
		bIsScrolling = false;
	}
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
void afw_camera_ui_base_class::updateRotation() {
	if (bApplyInertia) {
		rot *= drag;
		if (std::abs(rot.x) <= minDifference && std::abs(rot.y) <= minDifference && std::abs(rot.z) <= minDifference) {
			rot = { 0,0,0 };
			bApplyInertia = false;
			currentTransformType = TransformType::TRANSFORM_NONE;
			return;
		}

	}
	if (bApplyInertia) {
		curRot = glm::angleAxis(rot.z, getZAxis()) * glm::angleAxis(rot.y, up()) * glm::angleAxis(rot.x, getXAxis());
		//rotateAround(curRot, target.getGlobalPosition());
		//rotateAround(const glm::quat & q, const glm::vec3 & point)
		setPosition(curRot * (getGlobalPosition() - target.getGlobalPosition()) + target.getGlobalPosition());
		rotate(curRot);
	}
	else {
		curRot = glm::angleAxis(rot.z, lastPressAxisZ) * glm::angleAxis(rot.y, up()) * glm::angleAxis(rot.x, lastPressAxisX);
		setOrientation(curRot * lastPressOrientation);
		setPosition(curRot * (lastPressPosition - target.getGlobalPosition()) + target.getGlobalPosition());
	}
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
void afw_camera_ui_base_class::setControlArea(const view_port_struct_type& _controlArea) {
	controlArea = _controlArea;
}

//----------------------------------------
// !!!!!!!!!! This needs testing !!!!!!!!!!!!!!! is this needed ???????????????
void afw_camera_ui_base_class::clearControlArea() {
	controlArea = view_port_struct_type();
}

//----------------------------------------
//view_port_struct_type afw_camera_ui_base_class::getControlArea() const {
//	if (controlArea.is_zero()) {
//		if (viewport.is_zero()) {
//			//return getRenderer()->getCurrentViewport();
//			return view_port; // THis is a bug that needs to get the window dimensions and return
//		}
//		return viewport;
//	}
//	return controlArea;
//}

//----------------------------------------
void afw_camera_ui_base_class::mouse_button_pressed(GLFWwindow* glfw_window, glfw_mouse_event_class& mouse_event, glfw_cursor_event_class& cursor_event) {

	// For some unknown reason, if perform either of the following tests, glfwGetMouseButton function does not work !!!!
	// These test work in returning the correct results to perform a camera rotation for the selected buttons and button action
	// but it seems performing a test via a for loop renders the glfwGetMouseButton function will only operate on a single button press
	// not when the mouse button is pressed down in a repeat action !!!!! Thus cannot use a test to see if a mouse button is enabled, nor
	// to return a value to indicate which mouse button is pressed and thus have to go about mouse interaction in a more inefficient manner
	// of if else statemenmts. It seems if anything in terms of CPU processing comes between a mouse button poll event and glfwGetMouseButton
	// renders the glfwGetMouseButton function inoperable and non functioning !!!!!!!!!!!!!!

	if (!is_mouse_interaction_enabled(mouse_event.button_action, mouse_event.active_button, mouse_event.button_mods)) return;
	int interaction_id = get_mouse_interaction_id(mouse_event.button_action, mouse_event.active_button, mouse_event.button_mods);

	if (mouse_event.button_action == GLFW_PRESS) {
		lastPressMouse = cursor_event.cursor_window_pos;
		prevMouse = cursor_event.cursor_window_pos;
		printf("afw_camera_ui_base_class::mousePressed :mouse_event.button_pressed AAAA: %f : %f\n", lastPressMouse.x, prevMouse.x);
	}
	perform_mouse_pressed_action(glfw_window, mouse_event, cursor_event, interaction_id);
}

void afw_camera_ui_base_class::mouse_button_released(GLFWwindow* glfw_window, glfw_mouse_event_class& mouse_event, glfw_cursor_event_class& cursor_event) {
	//std::cout <<" afw_camera_ui_base_class::keyReleased : 000 : " << id << "  :  " << getPosition().x << " : " << getPosition().y << " : " << getPosition().z << std::endl;
	if (!is_mouse_interaction_enabled(mouse_event.button_action, mouse_event.active_button, mouse_event.button_mods)) return;
	int interaction_id = get_mouse_interaction_id(mouse_event.button_action, mouse_event.active_button, mouse_event.button_mods);
	perform_mouse_released_action(glfw_window, interaction_id);
}

void afw_camera_ui_base_class::perform_mouse_pressed_action(GLFWwindow* glfw_window, glfw_mouse_event_class& mouse_event, glfw_cursor_event_class& cursor_event, int interaction_id) {

	// Due to reasons given for mouse_button_pressed function, cannot perform the desired switch statemnet for mouse button pressed action
	//switch (interaction_id) {
	//	case ACTION_TYPE_CAMERA_ROTATE    : mouse_rotate_camera_pitch_yaw(cursor_event); break;
	//	case ACTION_TYPE_CAMERA_ROLL      : mouse_rotate_camera_roll(cursor_event);  break;
	//}

	//Add more mouse interactions here


	// Get initilal mouse cursor locations when a mouse button is presssed.
	if (mouse_event.button_action == GLFW_PRESS) {
		lastPressMouse = cursor_event.cursor_window_pos;
		prevMouse = cursor_event.cursor_window_pos;
		//printf("afw_camera_ui_base_class::mousePressed :mouse_event.button_pressed AAAA: %f : %f\n", lastPressMouse.x, prevMouse.x);
	}

	mouseVel = cursor_event.cursor_window_pos - prevMouse;
	if (glfwGetMouseButton(glfw_window, MOUSE_PITCH_YAW_ROTATION) == GLFW_PRESS) {
		// Cannot perform the desired test function even here for mouse button pressed action enebled or not
		//if (!is_mouse_interaction_enabled(mouse_event.button_action, mouse_event.active_button, mouse_event.button_mods)) return
		if (!orbital_rotation) {
			//printf("!orbital_rotation\n");
			mouse_rotate_camera_pitch_yaw(cursor_event);
		}
		else {
			//printf("orbital_rotation\n");
			orbital_mouse_rotate_camera_pitch_yaw(cursor_event);
		}
	}
	else {
		if (glfwGetMouseButton(glfw_window, MOUSE_ROLL_ROTATION) == GLFW_PRESS) {
			mouse_rotate_camera_roll(cursor_event);
		}
	}
	//Add more mouse interactions here as else if statements 
}

void afw_camera_ui_base_class::perform_mouse_released_action(GLFWwindow* glfw_window, int interaction_id) {
	// Nothing yet implemeted
}

void afw_camera_ui_base_class::key_rotate_camera_pitch(float rot_dir) {
	float pitch_rot = rotation_sensitivity * glm::pi<float>() * rotation_speed * rot_dir * key_rotation_factor;

	float distance = glm::length(getPosition() - look_at_location);// get distance to location camera is looking at

	tiltDeg(pitch_rot);

	if (orbital_rotation) {
		glm::vec3 forward_dir_n = glm::normalize(get_forward_dir());// Get new normalised forward directional vector after camera has rotated 
		glm::vec3 look_at_n = forward_dir_n * distance;				// Get new location of where camera is looking at that is same distance of previous look at location 

		setPosition(look_at_location - look_at_n);					// set a new location for the camera sch that it retains the initial look at location before being rotated.
	}
}

void afw_camera_ui_base_class::key_rotate_camera_yaw(float rot_dir) {
	float yaw_rot = rotation_sensitivity * glm::pi<float>() * rotation_speed * rot_dir * key_rotation_factor;

	float distance = glm::length(getPosition() - look_at_location);// get distance to location camera is looking at

	panDeg(yaw_rot);

	if (orbital_rotation) {
		glm::vec3 forward_dir_n = glm::normalize(get_forward_dir());// Get new normalised forward directional vector after camera has rotated 
		glm::vec3 look_at_n = forward_dir_n * distance;				// Get new location of where camera is looking at that is same distance of previous look at location 

		setPosition(look_at_location - look_at_n);					// set a new location for the camera sch that it retains the initial look at location before being rotated.
	}
}

void afw_camera_ui_base_class::key_rotate_camera_roll(float rot_dir) {
	float roll_rot = rotation_sensitivity * glm::pi<float>() * rotation_speed * rot_dir * key_rotation_factor;

	rollDeg(roll_rot);
}

void afw_camera_ui_base_class::mouse_rotate_camera_pitch_yaw(glfw_cursor_event_class& cursor_event) {
	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 000: %f : %f\n", cursor_event.cursor_window_pos.x, cursor_event.cursor_window_pos.y);
	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 111 : %f : %f : %f\n", mouseVel.x, sensitivityRot.y, rotation_speed);

		// Code for if wish to have different rotation for pitch - yaw
		//float pitch_rot = mouseVel.y * sensitivityRot.y * glm::pi<float>() * rotation_speed;
		//float yaw_rot   = mouseVel.x * sensitivityRot.x * glm::pi<float>() * rotation_speed;

	float pitch_rot = mouseVel.y * rotation_sensitivity * glm::pi<float>() * rotation_speed;
	float yaw_rot = mouseVel.x * rotation_sensitivity * glm::pi<float>() * rotation_speed;

	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 222 : %f : %f \n", pitch_rot, yaw_rot);

	tiltDeg(pitch_rot);
	panDeg(yaw_rot);

	//printf("afw_camera_ui_base_class::mousePressed orbit  333 : %f : %f \n", pitch_rot, yaw_rot);

	prevMouse = cursor_event.cursor_window_pos;
}

void afw_camera_ui_base_class::orbital_mouse_rotate_camera_pitch_yaw(glfw_cursor_event_class& cursor_event) {
	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 000: %f : %f\n", cursor_event.cursor_window_pos.x, cursor_event.cursor_window_pos.y);
	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 111 : %f : %f : %f\n", mouseVel.x, sensitivityRot.y, rotation_speed);

		// Code for if wish to have different rotation for pitch - yaw
		//float pitch_rot = mouseVel.y * sensitivityRot.y * glm::pi<float>() * rotation_speed;
		//float yaw_rot   = mouseVel.x * sensitivityRot.x * glm::pi<float>() * rotation_speed;

	float pitch_rot = mouseVel.y * rotation_sensitivity * glm::pi<float>() * rotation_speed;
	float yaw_rot = mouseVel.x * rotation_sensitivity * glm::pi<float>() * rotation_speed;

	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 222 : %f : %f \n", pitch_rot, yaw_rot);

	float distance = glm::length(getPosition() - look_at_location);// get distance to location camera is looking at

	tiltDeg(pitch_rot);
	panDeg(yaw_rot);

	glm::vec3 forward_dir_n = glm::normalize(get_forward_dir());// Get new normalised forward directional vector after camera has rotated 
	glm::vec3 look_at_n = forward_dir_n * distance;				// Get new location of where camera is looking at that is same distance of previous look at location 

	setPosition(look_at_location - look_at_n);					// set a new location for the camera sch that it retains the initial look at location before being rotated.

	//printf("afw_camera_ui_base_class::mousePressed orbit  333 : %f : %f \n", pitch_rot, yaw_rot);

	prevMouse = cursor_event.cursor_window_pos;
}

void afw_camera_ui_base_class::mouse_rotate_camera_roll(glfw_cursor_event_class& cursor_event) {
	//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_RIGHT\n");

		// Code for if wish to have different rotation for roll to that of pitch - yaw
		//float roll_rot = (mouseVel.x) * sensitivityRot.z * glm::pi<float>() * rotation_speed;

	float roll_rot = (mouseVel.x) * rotation_sensitivity * glm::pi<float>() * rotation_speed;

	rollDeg(roll_rot);

	prevMouse = cursor_event.cursor_window_pos;
}

void afw_camera_ui_base_class::mouse_scrolled(glfw_mouse_event_class& mouse_event, glfw_cursor_event_class& cursor_event) {

	//printf("afw_camera_ui_base_class::mouseScrolled X : %f Y : %f \n", mouse_event.x_scroll, mouse_event.y_scroll);

	if (mouse_event.y_scroll) {

		float mov_rate = movement_rate;									 // save default movement rate of camera

		movement_rate = float(mouse_event.y_scroll) * mouse_scroll_sensitivity; // assign a new movement rate of camera to a mouse scroll movement rate
		move_forward();													 // move foward/ backward camera depending upon the mouse scroll direction and movement rate

		movement_rate = mov_rate;										 // restore default camera movement rate
	}

	// Add x mouse scroll function here
}

/*
void afw_camera_ui_base_class::mousePressed(GLFWwindow *glfw_window, glfw_mouse_event_class &mouse_event,glfw_cursor_event_class &cursor_event) {
//printf("afw_camera_ui_base_class::mousePressed 0000  %f : %f\n", cursor_event.cursor_window_pos.x, cursor_event.cursor_window_pos.y);

		//lastPressAxisX = getXAxis();
		//lastPressAxisY = getYAxis();
		//lastPressAxisZ = getZAxis();
		//lastPressPosition = getGlobalPosition();
		//lastPressOrientation = getGlobalOrientation();

		//currentTransformType = TransformType::TRANSFORM_NONE;

		// Get initilal mouse cursor locations when a mouse button is presssed.
		if (mouse_event.button_action == GLFW_PRESS) {
			lastPressMouse = cursor_event.cursor_window_pos;
			prevMouse      = cursor_event.cursor_window_pos;
//printf("afw_camera_ui_base_class::mousePressed :mouse_event.button_pressed AAAA: %f : %f\n", lastPressMouse.x, prevMouse.x);
		}

		mouseVel = cursor_event.cursor_window_pos - prevMouse;
		if (glfwGetMouseButton(glfw_window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 000: %f : %f\n", cursor_event.cursor_window_pos.x, cursor_event.cursor_window_pos.y);
//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 111 : %f : %f : %f : %i\n", mouseVel.x, sensitivityRot.y, rotation_speed, std::min(area.x, area.y));

			float pitch_rot = mouseVel.y * sensitivityRot.y * glm::pi<float>() * rotation_speed;
			float yaw_rot   = mouseVel.x * sensitivityRot.x * glm::pi<float>() * rotation_speed;

//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_LEFT 222 : %f : %f \n", pitch_rot, yaw_rot);

			float distance = glm::length(getPosition() - look_at_location);// +++++

			tiltDeg(pitch_rot);
			panDeg(yaw_rot);

			glm::vec3 forward_dir_n = glm::normalize(get_forward_dir());// +++++
			glm::vec3 look_at_n = forward_dir_n * distance;// +++++

			setPosition(look_at_location - look_at_n) ;// +++++

//printf("afw_camera_ui_base_class::mousePressed orbit  : %f : %f : %f\n", old_pos.x, yaw_rot);

			//rotateAroundDeg(pitch_rot, const glm::vec3 & axis, {0.0f,0.0f,0.0f});

			prevMouse = cursor_event.cursor_window_pos;
		}
		else {
			if (glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
//printf("afw_camera_ui_base_class::mousePressed : mouse_event.active_button == GLFW_MOUSE_BUTTON_RIGHT\n");
				float roll_rot = (mouseVel.x) * sensitivityRot.z * glm::pi<float>() * rotation_speed;

				rollDeg(roll_rot);

				prevMouse = cursor_event.cursor_window_pos;
			}
			//else {
			//	if (glfwGetMouseButton(glfw_window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) { Not yet implemented
			//		float zoom_factor = mouseVel.y * movement_factor * glm::pi<float>() * movement_rate;

			//		dolly(-movement_rate);
			//		//printf("move_forward\n");
			//		assign_last_mouse_translations();
			//	}
			//}
		}
}
*/
//----------------------------------------
//void afw_camera_ui_base_class::mouseReleased(glfw_mouse_event_class& mouse_event, glfw_cursor_event_class &cursor_event) {
/*
	view_port_struct_type area = getControlArea();

	if (area.inside(mouse)) {
		// Check if it's double click
		unsigned long curTap = ofGetElapsedTimeMillis();
		if (lastTap != 0 && curTap - lastTap < doubleclickTime) {
			reset();
			return;
		}
		lastTap = curTap;
	}

	if (doInertia) {
		bApplyInertia = true;
	}
	else {
		currentTransformType = TransformType::TRANSFORM_NONE;
		rot = { 0,0,0 };
		translate = { 0,0,0 };
	}
*/
//}
//----------------------------------------


//----------------------------------------
//void afw_camera_ui_base_class::updateMouse(GLFWwindow *glfw_window,const glm::vec2& mouse) {
/*
	//view_port_struct_type area = getControlArea();
	//int vFlip = (isVFlipped() ? -1 : 1);
	int vFlip = 1;
	glm::ivec2 area;
	glfwGetWindowSize(glfw_window, &area.x, &area.y);
printf("afw_camera_ui_base_class::updateMouse : 0000\n");

	rot = { 0,0,0 };
	translate = { 0,0,0 };
	switch (currentTransformType) {
		case TransformType::TRANSFORM_ROTATE:
printf("afw_camera_ui_base_class::updateMouse :TransformType::TRANSFORM_ROTATE:\n");

			mouseVel = mouse - lastPressMouse;
			if (bInsideArcball) {
printf("afw_camera_ui_base_class::updateMouse :TransformType::TRANSFORM_ROTATE: bInsideArcball\n");
				//rot.x = vFlip * -mouseVel.y * sensitivityRot.x * glm::pi<float>() / std::min(area.width, area.height);
				rot.x = vFlip * -mouseVel.y * sensitivityRot.x * glm::pi<float>() * rotation_speed / std::min(area.x, area.y);// VWCUSTOM Modify MMMMM
				//rot.y = -mouseVel.x * sensitivityRot.y * glm::pi<float>() / std::min(area.width, area.height);
				rot.y = -mouseVel.x * sensitivityRot.y * glm::pi<float>() * rotation_speed / std::min(area.x, area.y);// VWCUSTOM Modify MMMMM
			}
			else {
printf("afw_camera_ui_base_class::updateMouse :TransformType::TRANSFORM_ROTATE: ! bInsideArcball\n");
				//glm::vec2 center(area.getCenter());
				glm::vec2 center({ area.x/2, area.y/2 });
				rot.z = sensitivityRot.z * rotation_speed * -vFlip * glm::orientedAngle(glm::normalize(mouse - center), glm::normalize(lastPressMouse - center));// VWCUSTOM Modify MMMMM
				//rot.z = sensitivityRot.z * -vFlip * glm::orientedAngle(glm::normalize(mouse - center),glm::normalize(lastPressMouse - center));
			}
			break;
		case TransformType::TRANSFORM_TRANSLATE_XY:
			mouseVel = mouse - prevMouse;
			//if (getOrtho()) {
			if (get_camera_type() == camera_type_enum::ORTHOGRAPHIC) {
				translate.x = -mouseVel.x * getScale().z;
				translate.y = vFlip * mouseVel.y * getScale().z;
			}
			else {
				//translate.x = -mouseVel.x * sensitivityTranslate.x * 0.5f * (getDistance() + std::numeric_limits<float>::epsilon()) / area.width;
				//translate.y = vFlip * mouseVel.y * sensitivityTranslate.y * 0.5f * (getDistance() + std::numeric_limits<float>::epsilon()) / area.height;
				translate.x = -mouseVel.x * sensitivityTranslate.x * 0.5f * (getDistance() + std::numeric_limits<float>::epsilon()) / area.x;
				translate.y = vFlip * mouseVel.y * sensitivityTranslate.y * 0.5f * (getDistance() + std::numeric_limits<float>::epsilon()) / area.y;
			}
			break;
		case TransformType::TRANSFORM_TRANSLATE_Z:
			mouseVel = mouse - prevMouse;
			if (get_camera_type() == camera_type_enum::ORTHOGRAPHIC) {
				//translate.z = mouseVel.y * sensitivityScroll / area.height;
				translate.z = mouseVel.y * sensitivityScroll / area.y;
			}
			else {
				//translate.z = mouseVel.y * (sensitivityTranslate.z * 0.7f) * (getDistance() + std::numeric_limits<float>::epsilon()) / area.height;
				translate.z = mouseVel.y * (sensitivityTranslate.z * 0.7f) * (getDistance() + std::numeric_limits<float>::epsilon()) / area.y;
			}
			break;

		default:
			break;
	}
	prevMouse = mouse;
*/
//}

//----------------------------------------
void afw_camera_ui_base_class::addInteraction(TransformType type, int mouseButton, int key) {
	if (!hasInteraction(mouseButton, key)) {
		interactions.push_back(interaction(type, mouseButton, key));
	}
	else {
		//ofLogNotice("afw_camera_ui_base_class") << "Can not add interaction. It already exists";
		afw_globalc::get_current_logger()->log(LogLevel::INFO, "Can not add interaction. It already exists", log_output_enum::consul_only);
	}
}
//----------------------------------------
void afw_camera_ui_base_class::removeInteraction(TransformType type, int mouseButton, int key) {
	//ofRemove(interactions, [&](interaction& i) { return i.transformType == type && i.mouseButton == mouseButton && i.key == key; });
	for (int i = 0; i < interactions.size(); i++) {
		if (interactions[i].transformType == type && interactions[i].mouseButton == mouseButton && interactions[i].key == key) {
			interactions.erase(interactions.begin() + i);
		}
	}
}
//----------------------------------------
bool afw_camera_ui_base_class::hasInteraction(int mouseButton, int key) {
	for (const auto& i : interactions) {
		if (i.mouseButton == mouseButton && i.key == key) {
			return true;
		}
	}
	return false;
}
//----------------------------------------
bool afw_camera_ui_base_class::hasInteraction(TransformType type, int mouseButton, int key) {
	for (const auto& i : interactions) {
		if (i.transformType == type && i.mouseButton == mouseButton && i.key == key) {
			return true;
		}
	}
	return false;
}
//----------------------------------------
void afw_camera_ui_base_class::removeAllInteractions() {
	interactions.clear();
}

// VWCUSTOM ADD ++++++++++++++++++++++++++++
	//----------------------------------------
void afw_camera_ui_base_class::add_key_interaction(int interaction_id, int key_event_type, bool enabled, int key_code, int modifier) {
	if (!has_key_interaction(key_event_type, key_code, modifier)) {
		key_interaction_struct_type key_interaction;
		key_interaction.action = key_event_type;
		key_interaction.enabled = enabled;
		key_interaction.key_code = key_code;
		key_interaction.modifiers = modifier;
		key_interaction.interaction_id = interaction_id;

		//printf("add_key_interaction : %i : %i : %i : %i : %i :\n", key_interaction.action, key_interaction.enabled, key_interaction.key_code, key_interaction.modifiers, key_interaction.interaction_id);
		key_interactions.push_back(key_interaction);
		//printf("add_key_interaction 01 : %i\n", key_interactions.size());
	}
	else {
		//ofLogNotice("afw_camera_ui_base_class") << "Can not add interaction. It already exists";
		afw_globalc::get_current_logger()->log(LogLevel::INFO, "Can not add interaction. It already exists", log_output_enum::consul_only);
	}
}
//----------------------------------------
void afw_camera_ui_base_class::remove_key_interaction(int key_event_type, int key_code, int modifier) {
	for (int i = 0; i < key_interactions.size(); i++) {
		if (key_interactions[i].action == key_event_type && key_interactions[i].key_code == key_code && key_interactions[i].modifiers == modifier) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}

//----------------------------------------
bool afw_camera_ui_base_class::has_key_interaction(int key_event_type, int key_code, int modifier) {
	for (key_interaction_struct_type& key_interaction : key_interactions) {
		if (key_interaction.action == key_event_type && key_interaction.key_code == key_code && key_interaction.modifiers == modifier) {
			return true;
		}
	}
	return false;
}

bool afw_camera_ui_base_class::is_key_interaction_enabled(int key_event_type, int key_code, int modifier) {
	for (key_interaction_struct_type& key_interaction : key_interactions) {
		if (key_interaction.action == AFW_PRESS_REPEAT) {
			if ((key_event_type == GLFW_PRESS || key_event_type == GLFW_REPEAT)
				&& key_interaction.key_code == key_code && key_interaction.modifiers == modifier && key_interaction.enabled) {
				//printf("(key_interaction.action == AFW_PRESS_REPEAT\n");
				return true;
			}
		}
		else {
			if (key_interaction.action == key_event_type && key_interaction.key_code == key_code && key_interaction.modifiers == modifier && key_interaction.enabled) {
				return true;
			}
		}
	}
	return false;
}

void afw_camera_ui_base_class::enable_key_interaction(int key_event_type, int key_code, int modifier) {
	for (key_interaction_struct_type& key_interaction : key_interactions) {
		if (key_interaction.action == key_event_type && key_interaction.key_code == key_code && key_interaction.modifiers == modifier) {
			key_interaction.enabled = true;
		}
	}
}

void afw_camera_ui_base_class::disable_key_interaction(int key_event_type, int key_code, int modifier) {
	for (key_interaction_struct_type& key_interaction : key_interactions) {
		if (key_interaction.action == key_event_type && key_interaction.key_code == key_code && key_interaction.modifiers == modifier) {
			key_interaction.enabled = false;
		}
	}
}

int afw_camera_ui_base_class::get_key_interaction_id(int key_event_type, int key_code, int modifier) {
	//printf("get_key_interaction_id AA : %i\n", key_interactions.size());
	for (key_interaction_struct_type key_interaction : key_interactions) {
		//printf("get_key_interaction_id 00, %i  : %i : %i : % i\n", key_interaction.action, key_interaction.key_code, key_interaction.modifiers, key_interaction.interaction_id);
		//printf("get_key_interaction_id 11, %i  : %i : %i \n", key_event_type, key_code, modifier);
				// NOTE : Critical to have the OR || logical test in brackets as it seems C++20 or visual studio does not evaluate or boolean expression in a left to right 
				//        order but ceases the boolean test if an or || statement is true.
				//        ie if key_event_type == GLFW_PRESS then the if statement is evaluated as true and ignores that all other conditions which are needed to be evaluated
				//        as true before the if statement is evaluated as true.
		if (key_interaction.action == AFW_PRESS_REPEAT) {
			if ((key_event_type == GLFW_PRESS || key_event_type == GLFW_REPEAT) && key_interaction.key_code == key_code && key_interaction.modifiers == modifier) {
				//printf("get_key_interaction_id PR AA :: key_interaction.action == AFW_PRESS_REPEAT %i  : %i : %i : % i\n", key_interaction.action, key_interaction.key_code, key_interaction.modifiers, key_interaction.interaction_id);
				return key_interaction.interaction_id;
			}
		}
		else {
			if (key_interaction.action == key_event_type && key_interaction.key_code == key_code && key_interaction.modifiers == modifier) {
				//printf("get_key_interaction_id P 22, %i  : %i : %i : %i  \n", key_interaction.action, key_interaction.key_code, key_interaction.modifiers,key_interaction.interaction_id);
				//printf("get_key_interaction_id P 33, %i  : %i : %i \n", key_event_type, key_code, modifier);

				return key_interaction.interaction_id;
			}
		}
	}

	return -1;
}

//----------------------------------------
void afw_camera_ui_base_class::remove_all_key_interactions() {
	key_interactions.clear();
}

void afw_camera_ui_base_class::remove_all_press_key_interactions() {
	for (int i = int(key_interactions.size()) - 1; i > -1; i--) {
		//if (key_interactions[i].action == int::key_pressed) {
		if (key_interactions[i].action == GLFW_PRESS) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}

void afw_camera_ui_base_class::remove_all_press_repeat_key_interactions() {
	for (int i = int(key_interactions.size()) - 1; i > -1; i--) {
		//if (key_interactions[i].action == int::key_pressed) {
		if (key_interactions[i].action == AFW_PRESS_REPEAT) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}

void afw_camera_ui_base_class::remove_all_release_key_interactions() {
	for (int i = int(key_interactions.size()) - 1; i > -1; i--) {
		//if (key_interactions[i].action == int::Released) {
		if (key_interactions[i].action == GLFW_RELEASE) {
			key_interactions.erase(key_interactions.begin() + i);
		}
	}
}
//----------------------------------------

// --------- Mouse interaction -------------
void afw_camera_ui_base_class::add_mouse_interaction(int interaction_id, int mouse_event_type, bool enabled, int button_code, int modifier) {
	if (!has_mouse_interaction(mouse_event_type, button_code, modifier)) {
		mouse_interaction_struct_type mouse_interaction;
		mouse_interaction.action = mouse_event_type;
		mouse_interaction.enabled = enabled;
		mouse_interaction.button_code = button_code;
		mouse_interaction.modifiers = modifier;
		mouse_interaction.interaction_id = interaction_id;

		//printf("add_mouse_interaction : %i : %i : %i : %i : %i :\n", mouse_interaction.action, mouse_interaction.enabled, mouse_interaction.button_code, mouse_interaction.modifiers, mouse_interaction.interaction_id);
		mouse_interactions.push_back(mouse_interaction);
		//printf("add_mouse_interaction 01 : %i\n", mouse_interactions.size());
	}
	else {
		//ofLogNotice("afw_camera_ui_base_class") << "Can not add interaction. It already exists";
		afw_globalc::get_current_logger()->log(LogLevel::INFO, "Can not add mouse interaction. It already exists", log_output_enum::consul_only);
	}
}
//----------------------------------------
void afw_camera_ui_base_class::remove_mouse_interaction(int mouse_event_type, int button_code, int modifier) {
	for (int i = 0; i < mouse_interactions.size(); i++) {
		if (mouse_interactions[i].action == mouse_event_type && mouse_interactions[i].button_code == button_code && mouse_interactions[i].modifiers == modifier) {
			mouse_interactions.erase(mouse_interactions.begin() + i);
		}
	}
}

//----------------------------------------
bool afw_camera_ui_base_class::has_mouse_interaction(int mouse_event_type, int button_code, int modifier) {
	for (mouse_interaction_struct_type& mouse_interaction : mouse_interactions) {
		if (mouse_interaction.action == mouse_event_type && mouse_interaction.button_code == button_code && mouse_interaction.modifiers == modifier) {
			return true;
		}
	}
	return false;
}

bool afw_camera_ui_base_class::is_mouse_interaction_enabled(int mouse_event_type, int button_code, int modifier) {
	for (mouse_interaction_struct_type& mouse_interaction : mouse_interactions) {
		//if (mouse_interaction.action == AFW_PRESS_REPEAT) {
		//	if ((mouse_event_type == GLFW_PRESS || mouse_event_type == GLFW_REPEAT)
		//		&& mouse_interaction.button_code == button_code && mouse_interaction.modifiers == modifier && mouse_interaction.enabled) {
//printf("(mouse_interaction.action == AFW_PRESS_REPEAT\n");
		//		return true;
		//	}
		//}
		//else {
		if (mouse_interaction.action == mouse_event_type && mouse_interaction.button_code == button_code && mouse_interaction.modifiers == modifier && mouse_interaction.enabled) {
			return true;
		}
		//}
	}
	return false;
}

void afw_camera_ui_base_class::enable_mouse_interaction(int mouse_event_type, int button_code, int modifier) {
	for (mouse_interaction_struct_type& mouse_interaction : mouse_interactions) {
		if (mouse_interaction.action == mouse_event_type && mouse_interaction.button_code == button_code && mouse_interaction.modifiers == modifier) {
			mouse_interaction.enabled = true;
		}
	}
}

void afw_camera_ui_base_class::disable_mouse_interaction(int mouse_event_type, int button_code, int modifier) {
	for (mouse_interaction_struct_type& mouse_interaction : mouse_interactions) {
		if (mouse_interaction.action == mouse_event_type && mouse_interaction.button_code == button_code && mouse_interaction.modifiers == modifier) {
			mouse_interaction.enabled = false;
		}
	}
}

int afw_camera_ui_base_class::get_mouse_interaction_id(int mouse_event_type, int button_code, int modifier) {
	//printf("get_mouse_interaction_id AA : %i\n", mouse_interactions.size());
	for (mouse_interaction_struct_type mouse_interaction : mouse_interactions) {
		//printf("get_mouse_interaction_id 00, %i  : %i : %i : % i\n", mouse_interaction.action, mouse_interaction.button_code, mouse_interaction.modifiers, mouse_interaction.interaction_id);
		//printf("get_mouse_interaction_id 11, %i  : %i : %i \n", mouse_event_type, button_code, modifier);

		if (mouse_interaction.action == mouse_event_type && mouse_interaction.button_code == button_code && mouse_interaction.modifiers == modifier) {
			//printf("get_mouse_interaction_id P 22, %i  : %i : %i : %i  \n", mouse_interaction.action, mouse_interaction.button_code, mouse_interaction.modifiers,mouse_interaction.interaction_id);
			//printf("get_mouse_interaction_id P 33, %i  : %i : %i \n", mouse_event_type, button_code, modifier);

			return mouse_interaction.interaction_id;
		}
	}

	return -1;
}

//----------------------------------------
void afw_camera_ui_base_class::remove_all_mouse_interactions() {
	mouse_interactions.clear();
}

void afw_camera_ui_base_class::remove_all_press_mouse_button_interactions() {
	for (int i = int(mouse_interactions.size()) - 1; i > -1; i--) {
		//if (mouse_interactions[i].action == int::mouse_pressed) {
		if (mouse_interactions[i].action == GLFW_PRESS) {
			mouse_interactions.erase(mouse_interactions.begin() + i);
		}
	}
}

//void afw_camera_ui_base_class::remove_all_press_repeat_mouse_interactions() {
//	for (int i = mouse_interactions.size() - 1; i > -1; i--) {
//		//if (mouse_interactions[i].action == int::mouse_pressed) {
//		if (mouse_interactions[i].action == AFW_PRESS_REPEAT) {
//			mouse_interactions.erase(mouse_interactions.begin() + i);
//		}
//	}
//}

void afw_camera_ui_base_class::remove_all_release_mouse_button_interactions() {
	for (int i = int(mouse_interactions.size()) - 1; i > -1; i--) {
		//if (mouse_interactions[i].action == int::Released) {
		if (mouse_interactions[i].action == GLFW_RELEASE) {
			mouse_interactions.erase(mouse_interactions.begin() + i);
		}
	}
}
//----------------------------------------


// END VWCUSTOM ADD ++++++++++++++++++++++++++++


//----------------------------------------
void afw_camera_ui_base_class::onPositionChanged() {
	if (!bDistanceSet && bAutoDistance) {
		bDistanceSet = true;
	}
}
