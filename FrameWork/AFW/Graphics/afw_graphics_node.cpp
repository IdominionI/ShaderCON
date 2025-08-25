
#include "afw_graphics_node.h"
//#include "of3dGraphics.h"

//----------------------------------------
afw_graphics_node_base_class::afw_graphics_node_base_class()
	:parent(nullptr)
	, localTransformMatrix(1)
	, legacyCustomDrawOverrided(true) {
	setPosition({ 0.f, 0.f, 0.f });
	setOrientation({ 0.f, 0.f, 0.f });
	setScale(1.f);
	//position.disableEvents();
	//scale.disableEvents();
	//orientation.disableEvents();

	// VWCUSTOM ADDITION BEGIN  +++++++++++++++++++++++++++
	//previous_position = getGlobalPosition();
	//current_position = previous_position;

	//previous_scale = getScale();
	//current_scale = previous_scale;

	//previous_orientation_quat = getOrientationQuat();
	//current_orientation_quat = previous_orientation_quat;
	//previous_orientation_euler = getOrientationEuler();
	//current_orientation_euler = previous_orientation_euler;
	// ADDITION END +++++++++++++++++++++++++++++
}

//----------------------------------------

//afw_graphics_node_base_class::~afw_graphics_node_base_class() {
	//if (parent) {
	//	parent->removeListener(*this);
	//}

	// clearParent() will remove children of this element as a side-effect.
	// This changes the "children", and so we can't use a normal foreach
	// loop, but must use the following construction to deal with newly
	// invalidated iterators:
	//while (!children.empty()) {
	//	(*children.begin())->clearParent();
	//}
//}

//----------------------------------------
//afw_graphics_node_base_class::afw_graphics_node_base_class(const afw_graphics_node_base_class& node)
//	:parent(node.parent)
//	, axis(node.axis)
//	, localTransformMatrix(node.localTransformMatrix)
//	, legacyCustomDrawOverrided(true) {
//	if (parent) {
//		parent->addListener(*this);
//	}
//	position = node.position;
//	orientation = node.orientation;
//	scale = node.scale;
//	position.disableEvents();
//	scale.disableEvents();
//	orientation.disableEvents();
//}
//
////----------------------------------------
//afw_graphics_node_base_class::afw_graphics_node_base_class(afw_graphics_node_base_class&& node)
//	:parent(node.parent)
//	, position(std::move(node.position))
//	, orientation(std::move(node.orientation))
//	, scale(std::move(node.scale))
//	, axis(std::move(node.axis))
//	, localTransformMatrix(std::move(node.localTransformMatrix))
//	, legacyCustomDrawOverrided(std::move(node.legacyCustomDrawOverrided))
//	, children(std::move(node.children)) {
//	if (parent) {
//		parent->addListener(*this);
//	}
//}

//----------------------------------------
//afw_graphics_node_base_class& afw_graphics_node_base_class::operator=(const afw_graphics_node_base_class& node) {
//	if (this == &node) return *this;
//	parent = node.parent;
//	position = node.position;
//	orientation = node.orientation;
//	scale = node.scale;
//	axis = node.axis;
//	position.disableEvents();
//	scale.disableEvents();
//	orientation.disableEvents();
//	localTransformMatrix = node.localTransformMatrix;
//	legacyCustomDrawOverrided = true;
//	if (parent) {
//		parent->addListener(*this);
//	}
//	return *this;
//}
//
////----------------------------------------
//afw_graphics_node_base_class& afw_graphics_node_base_class::operator=(afw_graphics_node_base_class&& node) {
//	if (this == &node) return *this;
//	parent = node.parent;
//	position = std::move(node.position);
//	orientation = std::move(node.orientation);
//	scale = std::move(node.scale);
//	axis = std::move(node.axis);
//	localTransformMatrix = std::move(node.localTransformMatrix);
//	legacyCustomDrawOverrided = std::move(node.legacyCustomDrawOverrided);
//	children = std::move(node.children);
//	if (parent) {
//		parent->addListener(*this);
//	}
//	return *this;
//}

//----------------------------------------
//void afw_graphics_node_base_class::addListener(afw_graphics_node_base_class& node) {
//	position.addListener(&node, &afw_graphics_node_base_class::onParentPositionChanged);
//	orientation.addListener(&node, &afw_graphics_node_base_class::onParentOrientationChanged);
//	scale.addListener(&node, &afw_graphics_node_base_class::onParentScaleChanged);
//	position.enableEvents();
//	orientation.enableEvents();
//	scale.enableEvents();
//	children.insert(&node);
//}

//----------------------------------------
//void afw_graphics_node_base_class::removeListener(afw_graphics_node_base_class& node) {
//	position.removeListener(&node, &afw_graphics_node_base_class::onParentPositionChanged);
//	orientation.removeListener(&node, &afw_graphics_node_base_class::onParentOrientationChanged);
//	scale.removeListener(&node, &afw_graphics_node_base_class::onParentScaleChanged);
//	if (position.getNumListeners() == 0) {
//		position.disableEvents();
//		scale.disableEvents();
//		orientation.disableEvents();
//	}
//	children.erase(&node);
//}

//----------------------------------------
void afw_graphics_node_base_class::setParent(afw_graphics_node_base_class& parent, bool bMaintainGlobalTransform) {
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
void afw_graphics_node_base_class::clearParent(bool bMaintainGlobalTransform) {
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
afw_graphics_node_base_class* afw_graphics_node_base_class::getParent() const {
	return parent;
}

//----------------------------------------
void afw_graphics_node_base_class::setPosition(float px, float py, float pz) {
	setPosition({ px, py, pz });
}

//----------------------------------------
void afw_graphics_node_base_class::setPosition(const glm::vec3& p) {
	position = p;
	createMatrix();
	onPositionChanged();
}

//----------------------------------------
void afw_graphics_node_base_class::setGlobalPosition(float px, float py, float pz) {
	setGlobalPosition({ px, py, pz });
}

//----------------------------------------
void afw_graphics_node_base_class::setGlobalPosition(const glm::vec3& p) {
	if (parent == nullptr) {
		setPosition(p);
	}
	else {
		auto newP = glm::inverse(parent->getGlobalTransformMatrix()) * glm::vec4(p, 1.0);
		setPosition(glm::vec3(newP) / newP.w);
	}
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getPosition() const {
	return position;
}

//----------------------------------------
float afw_graphics_node_base_class::getX() const {
	return position.x;
}

//----------------------------------------
float afw_graphics_node_base_class::getY() const {
	return position.y;
}

//----------------------------------------
float afw_graphics_node_base_class::getZ() const {
	return position.z;
}

//----------------------------------------
void afw_graphics_node_base_class::setOrientation(const glm::quat& q) {
	orientation = q;
	createMatrix();
	onOrientationChanged();
}

//----------------------------------------
void afw_graphics_node_base_class::setOrientation(const glm::vec3& eulerAngles) {
	glm::quat q(glm::radians(eulerAngles));
	setOrientation(q);
}

//----------------------------------------
void afw_graphics_node_base_class::setGlobalOrientation(const glm::quat& q) {
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
glm::quat afw_graphics_node_base_class::getOrientationQuat() const {
	return orientation;
}

//----------------------------------------
//glm::vec3 afw_graphics_node_base_class::getOrientationEuler() const {
//	return getOrientationEulerDeg();
//}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getOrientationEulerDeg() const {
	glm::vec3 euler = glm::eulerAngles(orientation);
	return { glm::degrees(euler.x), glm::degrees(euler.y), glm::degrees(euler.z) };
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getOrientationEulerRad() const {
	return glm::eulerAngles(orientation);
}

//----------------------------------------
void afw_graphics_node_base_class::setScale(float s) {
	setScale(s, s, s);
}

//----------------------------------------
void afw_graphics_node_base_class::setScale(float sx, float sy, float sz) {
	setScale({ sx, sy, sz });
}

//----------------------------------------
void afw_graphics_node_base_class::setScale(const glm::vec3& s) {
	this->scale = s;
	createMatrix();
	onScaleChanged();
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getScale() const {
	return scale;
}

//----------------------------------------
void afw_graphics_node_base_class::move(float x, float y, float z) {
	move({ x, y, z });
}

//----------------------------------------
void afw_graphics_node_base_class::move(const glm::vec3& offset) {
	position += offset;
	createMatrix();
	onPositionChanged();
}

//----------------------------------------
void afw_graphics_node_base_class::truck(float amount) {
	move(getXAxis() * amount);
}

//----------------------------------------
void afw_graphics_node_base_class::boom(float amount) {
	move(getYAxis() * amount);
}

//----------------------------------------
void afw_graphics_node_base_class::dolly(float amount) {
	move(getZAxis() * amount);
}

//----------------------------------------
//void afw_graphics_node_base_class::tilt(float degrees) {
//	rotateDeg(degrees, getXAxis());
//}
//
////----------------------------------------
//void afw_graphics_node_base_class::pan(float degrees) {
//	rotateDeg(degrees, getYAxis());
//}
//
////----------------------------------------
//void afw_graphics_node_base_class::roll(float degrees) {
//	rotateDeg(degrees, getZAxis());
//}

//----------------------------------------
void afw_graphics_node_base_class::tiltDeg(float degrees) {
	rotateDeg(degrees, getXAxis());
}

//----------------------------------------
void afw_graphics_node_base_class::panDeg(float degrees) {
	rotateDeg(degrees, getYAxis());
}

//----------------------------------------
void afw_graphics_node_base_class::rollDeg(float degrees) {
	rotateDeg(degrees, getZAxis());
}

//----------------------------------------
void afw_graphics_node_base_class::tiltRad(float radians) {
	rotateRad(radians, getXAxis());
}

//----------------------------------------
void afw_graphics_node_base_class::panRad(float radians) {
	rotateRad(radians, getYAxis());
}

//----------------------------------------
void afw_graphics_node_base_class::rollRad(float radians) {
	rotateRad(radians, getZAxis());
}

//----------------------------------------
void afw_graphics_node_base_class::rotate(const glm::quat& q) {
	orientation = q * (const glm::quat&)orientation;
	createMatrix();
	onOrientationChanged();
}

//----------------------------------------
//void afw_graphics_node_base_class::rotate(float degrees, const glm::vec3& v) {
//	rotateDeg(degrees, v);
//}

//----------------------------------------
void afw_graphics_node_base_class::rotateDeg(float degrees, const glm::vec3& v) {
	rotate(glm::angleAxis(glm::radians(degrees), v));
}

//----------------------------------------
void afw_graphics_node_base_class::rotateRad(float radians, const glm::vec3& v) {
	rotate(glm::angleAxis(radians, v));
}

//----------------------------------------
//void afw_graphics_node_base_class::rotate(float degrees, float vx, float vy, float vz) {
//	rotateDeg(degrees, vx, vy, vz);
//}

//----------------------------------------
void afw_graphics_node_base_class::rotateDeg(float degrees, float vx, float vy, float vz) {
	rotate(glm::angleAxis(glm::radians(degrees), glm::vec3(vx, vy, vz)));
}

//----------------------------------------
void afw_graphics_node_base_class::rotateRad(float radians, float vx, float vy, float vz) {
	rotate(glm::angleAxis(radians, glm::vec3(vx, vy, vz)));
}

//----------------------------------------
void afw_graphics_node_base_class::rotateAround(const glm::quat& q, const glm::vec3& point) {
	//	ofLogVerbose("afw_graphics_node_base_class") << "rotateAround(const glm::quat& q, const glm::vec3& point) not implemented yet";
	//	glm::mat4 m = getLocalTransformMatrix();
	//	m.setTranslation(point);
	//	m.rotate(q);

	setGlobalPosition(q * (getGlobalPosition() - point) + point);

	onOrientationChanged();
	onPositionChanged();
}

//----------------------------------------
//void afw_graphics_node_base_class::rotateAround(float degrees, const glm::vec3& axis, const glm::vec3& point) {
//	rotateAroundDeg(degrees, axis, point);
//}

//----------------------------------------
void afw_graphics_node_base_class::rotateAroundDeg(float degrees, const glm::vec3& axis, const glm::vec3& point) {
	rotateAround(glm::angleAxis(glm::radians(degrees), axis), point);
}

//----------------------------------------
void afw_graphics_node_base_class::rotateAroundRad(float radians, const glm::vec3& axis, const glm::vec3& point) {
	rotateAround(glm::angleAxis(radians, axis), point);
}

//----------------------------------------
void afw_graphics_node_base_class::lookAt(const glm::vec3& lookAtPosition) {
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
void afw_graphics_node_base_class::lookAt(const glm::vec3& lookAtPosition, glm::vec3 upVector) {
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
void afw_graphics_node_base_class::lookAt(const afw_graphics_node_base_class& lookAtNode) {
	lookAt(lookAtNode.getGlobalPosition());
}

//----------------------------------------
void afw_graphics_node_base_class::lookAt(const afw_graphics_node_base_class& lookAtNode, const glm::vec3& upVector) {
	lookAt(lookAtNode.getGlobalPosition(), upVector);
}

//----------------------------------------
void afw_graphics_node_base_class::updateAxis() {
	if (scale.x > 0) axis[0] = glm::vec3(getLocalTransformMatrix()[0] / scale.x);
	if (scale.y > 0) axis[1] = glm::vec3(getLocalTransformMatrix()[1] / scale.y);
	if (scale.z > 0) axis[2] = glm::vec3(getLocalTransformMatrix()[2] / scale.z);
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getXAxis() const {
	return axis[0];
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getYAxis() const {
	return axis[1];
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getZAxis() const {
	return axis[2];
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getSideDir() const {
	return getXAxis();
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getLookAtDir() const {
	return -getZAxis();
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getUpDir() const {
	return getYAxis();
}

//----------------------------------------
//float afw_graphics_node_base_class::getPitch() const {
//	return getPitchDeg();
//}
//
////----------------------------------------
//float afw_graphics_node_base_class::getHeading() const {
//	return getHeadingDeg();
//}
//
////----------------------------------------
//float afw_graphics_node_base_class::getRoll() const {
//	return getRollDeg();
//}

//----------------------------------------
float afw_graphics_node_base_class::getPitchDeg() const {
	return getOrientationEulerDeg().x;
}

//----------------------------------------
float afw_graphics_node_base_class::getHeadingDeg() const {
	return getOrientationEulerDeg().y;
}

//----------------------------------------
float afw_graphics_node_base_class::getRollDeg() const {
	return getOrientationEulerDeg().z;
}

//----------------------------------------
float afw_graphics_node_base_class::getPitchRad() const {
	return getOrientationEulerRad().x;
}

//----------------------------------------
float afw_graphics_node_base_class::getHeadingRad() const {
	return getOrientationEulerRad().y;
}

//----------------------------------------
float afw_graphics_node_base_class::getRollRad() const {
	return getOrientationEulerRad().z;
}

//----------------------------------------
const glm::mat4& afw_graphics_node_base_class::getLocalTransformMatrix() const {
	return localTransformMatrix;
}

//----------------------------------------
glm::mat4 afw_graphics_node_base_class::getGlobalTransformMatrix() const {
	if (parent) return parent->getGlobalTransformMatrix() * getLocalTransformMatrix();
	else return getLocalTransformMatrix();
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getGlobalPosition() const {
	return glm::vec3(getGlobalTransformMatrix()[3]);
}

//----------------------------------------
glm::quat afw_graphics_node_base_class::getGlobalOrientation() const {
	if (parent) return parent->getGlobalOrientation() * getOrientationQuat();
	return getOrientationQuat();
}

//----------------------------------------
glm::vec3 afw_graphics_node_base_class::getGlobalScale() const {
	if (parent) return getScale() * parent->getGlobalScale();
	else return getScale();
}

//----------------------------------------
//void afw_graphics_node_base_class::orbit(float longitude, float latitude, float radius, const glm::vec3& centerPoint) {
//	orbitDeg(longitude, latitude, radius, centerPoint);
//}
//
////----------------------------------------
//void afw_graphics_node_base_class::orbit(float longitude, float latitude, float radius, afw_graphics_node_base_class& centerNode) {
//	orbitDeg(longitude, latitude, radius, centerNode);
//}

//----------------------------------------
void afw_graphics_node_base_class::orbitDeg(float longitude, float latitude, float radius, afw_graphics_node_base_class& centerNode) {
	orbitDeg(longitude, latitude, radius, centerNode.getGlobalPosition());
}

//----------------------------------------
void afw_graphics_node_base_class::orbitDeg(float longitude, float latitude, float radius, const glm::vec3& centerPoint) {
	glm::quat q =
		glm::angleAxis(glm::radians(longitude), glm::vec3(0, 1, 0))
		* glm::angleAxis(glm::radians(latitude), glm::vec3(1, 0, 0));

	//glm::vec4 p{ 0.f, 0.f, 1.f, 0.f };	   // p is a direction, not a position, so .w == 0
	glm::vec3 p{ 0.f, 0.f, 1.f};	   // p is a direction, not a position, so .w == 0

	p = q * p;							   // rotate p on unit sphere based on quaternion
	p = p * radius;						   // scale p by radius from its position on unit sphere

	setGlobalPosition(centerPoint + p);
	setOrientation(q);

	onOrientationChanged();
	onPositionChanged();
}

//----------------------------------------
void afw_graphics_node_base_class::orbitRad(float longitude, float latitude, float radius, afw_graphics_node_base_class& centerNode) {
	orbitRad(longitude, latitude, radius, centerNode.getGlobalPosition());
}

//----------------------------------------
void afw_graphics_node_base_class::orbitRad(float longitude, float latitude, float radius, const glm::vec3& centerPoint) {
	glm::quat q =
		glm::angleAxis(longitude, glm::vec3(0, 1, 0))
		* glm::angleAxis(latitude, glm::vec3(1, 0, 0));

	//glm::vec4 p{ 0.f, 0.f, 1.f, 0.f };	   // p is a direction, not a position, so .w == 0
	glm::vec3 p{ 0.f, 0.f, 1.f };	   // p is a direction, not a position, so .w == 0

	p = q * p;							   // rotate p on unit sphere based on quaternion
	p = p * radius;						   // scale p by radius from its position on unit sphere

	setGlobalPosition(centerPoint + p);
	setOrientation(q);

	onOrientationChanged();
	onPositionChanged();
}

//----------------------------------------
void afw_graphics_node_base_class::resetTransform() {
	setPosition({ 0.f,0.f,0.f });
	setOrientation({ 0.f,0.f,0.f });
	setScale({ 1.f,1.f,1.f });
}

////----------------------------------------
//void afw_graphics_node_base_class::draw()  const {
//	ofGetCurrentRenderer()->draw(*this);
//}
//
//void afw_graphics_node_base_class::draw_geometry()  const {
//	// VWCUSTOM ---------------------
//	//ofGetCurrentRenderer()->draw(*this);
//	//----------------------------------
//
//	// VWCUSTOM ++++++++++++++++++
//	if (geometry != NULL)
//		geometry->render();
//	// ++++++++++++++++++++++++++
//}
//
////----------------------------------------
//void afw_graphics_node_base_class::customDraw(const ofBaseRenderer* renderer) const {
//	const_cast<afw_graphics_node_base_class*>(this)->customDraw();
//	if (!legacyCustomDrawOverrided) {
//		renderer->drawBox(10);
//		renderer->draw(ofMesh::axis(20), OF_MESH_FILL);
//	}
//}
//
////----------------------------------------
//void afw_graphics_node_base_class::customDraw() {
//	legacyCustomDrawOverrided = false;
//}

//----------------------------------------
//void afw_graphics_node_base_class::transformGL(ofBaseRenderer* renderer) const {
//	if (renderer == nullptr) {
//		renderer = ofGetCurrentRenderer().get();
//	}
//	renderer->pushMatrix();
//	renderer->multMatrix(getGlobalTransformMatrix());
//}

//----------------------------------------
//void afw_graphics_node_base_class::restoreTransformGL(ofBaseRenderer* renderer) const {
//	if (renderer == nullptr) {
//		renderer = ofGetCurrentRenderer().get();
//	}
//	renderer->popMatrix();
//}

//----------------------------------------
void afw_graphics_node_base_class::createMatrix() {
	//localTransformMatrix = glm::translate(glm::mat4(1.0), toGlm(position));
	localTransformMatrix = glm::translate(glm::mat4(1.0), position);
	localTransformMatrix = localTransformMatrix * glm::toMat4((const glm::quat&)orientation);
	//localTransformMatrix = glm::scale(localTransformMatrix, toGlm(scale));
	localTransformMatrix = glm::scale(localTransformMatrix,scale);

	updateAxis();
}


