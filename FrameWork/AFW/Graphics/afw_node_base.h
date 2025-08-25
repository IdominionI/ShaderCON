#pragma once

#include <array>
#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#define GLM_ENABLE_EXPERIMENTAL // need this to avoid build error for glm/gtx
#include <glm/gtx/quaternion.hpp>
#include "glm/mat4x4.hpp"

#include "FrameWork/AFW/Common/afw_definitions.h"

struct afw_node_properties_struct{
	int         id = INVALID_ID;
	std::string name = "";
	std::string description = "";
	bool        enabled = true;

	glm::vec3 position    = { 0.0f,0.0f,0.0f };
	glm::quat orientation = {0.0f,0.0f,0.0f,0.0f};
	glm::vec3 scale       = { 1.0f,1.0f,1.0f };

	std::array<glm::vec3, 3> axis;

	// ------------------------------------------
	glm::vec3 local_location, global_location;

	glm::vec3 previous_position, current_position;
	glm::quat current_orientation_quat, previous_orientation_quat;
	glm::vec3 current_orientation_euler, previous_orientation_euler;
	glm::vec3 previous_scale, current_scale;

	// #############################################

	virtual void setPosition(const glm::vec3& p) { position = p; }
	void setPosition(float px, float py, float pz) {setPosition({ px, py, pz });}
	//-------------------------
	virtual void setOrientation(const glm::quat& q) {orientation = q;}
	void setOrientation(const glm::vec3& eulerAngles) {
		glm::quat q(glm::radians(eulerAngles));
		setOrientation(q);
	}
	void set_orientation(float pitch, float yaw, float roll) {
		setOrientation({ pitch, yaw, roll });
	}
	// -------------------------------------------
	virtual void setScale(const glm::vec3& s) { scale = s; }
	void setScale(float s) {setScale(s, s, s);}
	void setScale(float sx, float sy, float sz) {setScale({ sx, sy, sz });}

	//===============================================

	glm::vec3 getPosition() const {	return position;}

	float getPitchDeg() const {	return getOrientationEulerDeg().x;}
	float getHeadingDeg() const {return getOrientationEulerDeg().y;}
	float getRollDeg() const {return getOrientationEulerDeg().z;}
	//----------------------------------------
	
	float getPitchRad() const {	return getOrientationEulerRad().x;}
	float getHeadingRad() const {return getOrientationEulerRad().y;	}
	float getRollRad() const {return getOrientationEulerRad().z;}
	//----------------------------------------
	glm::vec3 getScale() const {
		return scale;
	}

	// ------------------------------------------
	glm::vec3 getXAxis() const { return axis[0]; }
	glm::vec3 getYAxis() const { return axis[1]; }
	glm::vec3 getZAxis() const { return axis[2]; }
	//----------------------------------------
	glm::vec3 get_right() const { return getXAxis(); }
	glm::vec3 get_forward_dir() const { return -getZAxis(); }
	glm::vec3 get_up_dir() const { return getYAxis(); }
	// ------------------------------------------

	glm::vec3 getOrientationEulerDeg() const {
		glm::vec3 euler = glm::eulerAngles(orientation);
		return { glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z) };
	}
	glm::vec3 getOrientationEulerRad() const {
		return glm::eulerAngles(orientation);
	}

	// ===========================================
	//----------------------------------------
	virtual void move(const glm::vec3& offset) { position += offset; }
	void move(float x, float y, float z) {move({ x, y, z });}

	void truck(float amount) {move(getXAxis() * amount);}
	void boom(float amount) {move(getYAxis() * amount);}
	void dolly(float amount) {move(getZAxis() * amount);}
	//---------------------------------------------------
	void tiltDeg(float degrees) {rotateDeg(degrees, getXAxis());}
	void panDeg(float degrees) {rotateDeg(degrees, getYAxis());	}
	void rollDeg(float degrees) {rotateDeg(degrees, getZAxis());}
	
	void tiltRad(float radians) {rotateRad(radians, getXAxis());}
	void panRad(float radians) {rotateRad(radians, getYAxis());}
	void rollRad(float radians) {rotateRad(radians, getZAxis());}

	//----------------------------------------
	virtual void rotate(const glm::quat& q) {orientation = q * (const glm::quat&)orientation;}
	void rotateDeg(float degrees, const glm::vec3& v) {	rotate(glm::angleAxis(glm::radians(degrees), v));}
	void rotateRad(float radians, const glm::vec3& v) {	rotate(glm::angleAxis(radians, v));}
	void rotateDeg(float degrees, float vx, float vy, float vz) {
		rotate(glm::angleAxis(glm::radians(degrees), glm::vec3(vx, vy, vz)));
	}
	void rotateRad(float radians, float vx, float vy, float vz) {
		rotate(glm::angleAxis(radians, glm::vec3(vx, vy, vz)));
	}

	// =================================================

};

struct afw_graphics_node_properties_struct : public afw_node_properties_struct {
	glm::vec3 origin_location;

	// #############################################

	void set_origin_location(float px, float py, float pz) { set_origin_location({ px, py, pz }); }
	void set_origin_location(const glm::vec3& p) { origin_location = p; }

	glm::vec3 get_origin_location() const { return origin_location; }


};

class afw_node_base_class : public afw_graphics_node_properties_struct {
public:
	afw_node_base_class() {
		// These initialisations must be present in the constructor or have
		// a situation where there is a random occurance of position and 
		// orientation not being initialised and updated in a derived class
		// even if position and orientation are initialised in the derived class
		// Weired !!!!
		glm::vec3 position    = { 0.0f,0.0f,0.0f };
		set_orientation(0.0f, 0.0f, 0.0f);
		glm::vec3 scale       = { 1.0f,1.0f,1.0f };
	};
	~afw_node_base_class() {};

	afw_node_base_class *parent = nullptr;
	std::vector<afw_node_base_class *> children;

	glm::mat4 localTransformMatrix = {};



	void setParent(afw_node_base_class& parent, bool bMaintainGlobalTransform) {
		if (this->parent)
		{
			// we need to make sure to clear before
			// re-assigning parenthood.
			clearParent(bMaintainGlobalTransform);
		}
		if (bMaintainGlobalTransform) {
			auto postParentPosition = position - parent.getGlobalPosition();
			//auto postParentOrientation = orientation.get() * glm::inverse(parent.getGlobalOrientation());
			auto postParentOrientation = orientation * glm::inverse(parent.getGlobalOrientation());
			auto postParentScale = scale / parent.getGlobalScale();
			//parent.addListener(*this);
			setOrientation(postParentOrientation);
			setPosition(postParentPosition);
			setScale(postParentScale);
		}
		else {
			//parent.addListener(*this);
		}
		this->parent = &parent;
	}

	//----------------------------------------
	void clearParent(bool bMaintainGlobalTransform) {
		if (parent) {
			//parent->removeListener(*this);
		}
		if (bMaintainGlobalTransform && parent) {
			auto orientation = getGlobalOrientation();
			auto position = getGlobalPosition();
			auto scale = getGlobalScale();
			this->parent = nullptr;
			setOrientation(orientation);
			setPosition(position);
			setScale(scale);
		}
		else {
			this->parent = nullptr;
		}

	}

	//----------------------------------------
	afw_node_base_class* getParent() const {return parent;}

	void setPosition(const glm::vec3& p) override {
		position = p; 
		createMatrix();
	}

	void setOrientation(const glm::quat& q) override {
		orientation = q;
		createMatrix();
	}

	void setScale(const glm::vec3& s) override {
		scale = s; 
		createMatrix();
	}

	void move (const glm::vec3& offset)  override { 
		position += offset;
		createMatrix();
//printf("afw_node_base_class:: move :: %f : %f : %f \n", position.x, position.y, position.z);
	}

	void rotate(const glm::quat& q) override {
		orientation = q * (const glm::quat&)orientation;
		createMatrix();
		glm::vec3 ea = getOrientationEulerRad();
//printf("afw_node_base_class:: rotate :: %f : %f : %f \n", glm::degrees(ea.x), glm::degrees(ea.y), glm::degrees(ea.z));
	}

	// ===============================================================
	void look_at_location(const glm::vec3& lookAtPosition) {
		auto relPosition = (getGlobalPosition() - lookAtPosition);
		auto radius = glm::length(relPosition);
		if (radius > 0) {
			float latitude = acos(relPosition.y / radius) - glm::half_pi<float>();
			float longitude = atan2(relPosition.x, relPosition.z);
			glm::quat q = glm::angleAxis(0.f, glm::vec3(0, 0, 1)) * glm::angleAxis(longitude, glm::vec3(0, 1, 0)) * glm::angleAxis(latitude, glm::vec3(1, 0, 0));
			setGlobalOrientation(q);
		}
	}

	//----------------------------------------
	void look_at_location(const glm::vec3& lookAtPosition, glm::vec3 upVector) {
		if (parent) {
			auto upVector4 = glm::inverse(parent->getGlobalTransformMatrix()) * glm::vec4(upVector, 1.0);
			upVector = glm::vec3(upVector4) / upVector4.w;
		}
		auto zaxis = glm::normalize(getGlobalPosition() - lookAtPosition);
		if (glm::length(zaxis) > 0) {
			auto xaxis = glm::normalize(glm::cross(upVector, zaxis));
			auto yaxis = glm::cross(zaxis, xaxis);
			glm::mat3 m;
			m[0] = xaxis;
			m[1] = yaxis;
			m[2] = zaxis;

			setGlobalOrientation(glm::toQuat(m));
		}
	}

	//----------------------------------------
	void look_at_node(const afw_node_base_class& lookAtNode) {
		look_at_location(lookAtNode.getGlobalPosition());
	}

	//----------------------------------------
	void look_at_node(const afw_node_base_class& lookAtNode, const glm::vec3& upVector) {
		look_at_location(lookAtNode.getGlobalPosition(), upVector);
	}

	// ==========================================================
	void setGlobalOrientation(const glm::quat& q) {
		if (parent == nullptr) {
			setOrientation(q);
		}
		else {
			auto invParent = glm::inverse(parent->getGlobalOrientation());
			auto m44 = invParent * q;
			setOrientation(m44);
		}
	}

	//----------------------------------------
	const glm::mat4& getLocalTransformMatrix() const {
		return localTransformMatrix;
	}

	glm::mat4 getGlobalTransformMatrix() const {
		if (parent) {
			return parent->getGlobalTransformMatrix() * getLocalTransformMatrix();
//printf("afw_node_base_class::getGlobalTransformMatrix: parent \n");
		}
		else {
//printf("afw_node_base_class::getGlobalTransformMatrix: !parent \n");
			return getLocalTransformMatrix();
		}
	}

	glm::vec3 getGlobalPosition() const {
		return glm::vec3(getGlobalTransformMatrix()[3]);
	}

	//----------------------------------------
	glm::quat getGlobalOrientation() const {
		if (parent) return parent->getGlobalOrientation() * getOrientationQuat();
		return getOrientationQuat();
	}

	//----------------------------------------
	glm::vec3 getGlobalScale() const {
		if (parent) return getScale() * parent->getGlobalScale();
		else return getScale();
	}

	glm::quat getOrientationQuat() const {
		return orientation;
	}

	//===================================
	void updateAxis() {
		if (scale.x > 0) axis[0] = glm::vec3(getLocalTransformMatrix()[0] / scale.x);
		if (scale.y > 0) axis[1] = glm::vec3(getLocalTransformMatrix()[1] / scale.y);
		if (scale.z > 0) axis[2] = glm::vec3(getLocalTransformMatrix()[2] / scale.z);
	}



	void createMatrix() {
		//localTransformMatrix = glm::translate(glm::mat4(1.0), toGlm(position));
		localTransformMatrix = glm::translate(glm::mat4(1.0), position);
		localTransformMatrix = localTransformMatrix * glm::toMat4((const glm::quat&)orientation);
		//localTransformMatrix = glm::scale(localTransformMatrix, toGlm(scale));
		localTransformMatrix = glm::scale(localTransformMatrix, scale);

		updateAxis();
	}



private:

};
