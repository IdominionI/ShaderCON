#include "afw_rectangle.h"

#include <algorithm>
#include <iostream>

//#include "ofMath.h"
//#include "ofLog.h"
//#include "ofVectorMath.h"

//using std::ostream;
//using std::istream;
//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class() : x(position.x), y(position.y) {
    set(0, 0, 0, 0);
}

//----------------------------------------------------------
afw_rectangle_class::~afw_rectangle_class() {}

//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class(float px, float py, float w, float h) : x(position.x), y(position.y) {
    set(px, py, w, h);
}

//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class(const glm::vec3& p, float w, float h) : x(position.x), y(position.y) {
    set(p, w, h);
}

//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class(const glm::vec2& p, float w, float h) : x(position.x), y(position.y) {
    set(p, w, h);
}

//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class(const afw_rectangle_class& rect) : x(position.x), y(position.y) {
    set(rect);
}

//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class(const glm::vec3& p0, const glm::vec3& p1) : x(position.x), y(position.y) {
    set(p0, p1);
}

//----------------------------------------------------------
afw_rectangle_class::afw_rectangle_class(const glm::vec2& p0, const glm::vec2& p1) : x(position.x), y(position.y) {
    set(p0, p1);
}

//----------------------------------------------------------
void afw_rectangle_class::set(float px, float py, float w, float h) {
    x = px;
    y = py;
    width = w;
    height = h;
}

//----------------------------------------------------------
void afw_rectangle_class::set(const glm::vec3& p, float w, float h) {
    set(glm::vec2(p), w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::set(const glm::vec2& p, float w, float h) {
    set(p.x, p.y, w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::set(const afw_rectangle_class& rect) {
    set(rect.x, rect.y, rect.width, rect.height);
}

//----------------------------------------------------------
void afw_rectangle_class::set(const glm::vec3& p0, const glm::vec3& p1) {
    set(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
void afw_rectangle_class::set(const glm::vec2& p0, const glm::vec2& p1) {
    float x0, y0, x1, y1;

    x0 = std::min(p0.x, p1.x);
    x1 = std::max(p0.x, p1.x);
    y0 = std::min(p0.y, p1.y);
    y1 = std::max(p0.y, p1.y);

    float w = x1 - x0;
    float h = y1 - y0;

    set(x0, y0, w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::setX(float px) {
    x = px;
}

//----------------------------------------------------------
void afw_rectangle_class::setY(float py) {
    y = py;
}

//----------------------------------------------------------
void afw_rectangle_class::setWidth(float w) {
    width = w;
}

//----------------------------------------------------------
void afw_rectangle_class::setHeight(float h) {
    height = h;
}

//----------------------------------------------------------
void afw_rectangle_class::setPosition(float px, float py) {
    position.x = px;
    position.y = py;
}

//----------------------------------------------------------
void afw_rectangle_class::setPosition(const glm::vec3& p) {
    position = p;
}

//----------------------------------------------------------
void afw_rectangle_class::setSize(float w, float h) {
    width = w;
    height = h;
}


//----------------------------------------------------------
void afw_rectangle_class::setFromCenter(float px, float py, float w, float h) {
    set(px - w * 0.5f, py - h * 0.5f, w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::setFromCenter(const glm::vec3& p, float w, float h) {
    setFromCenter(glm::vec2(p), w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::setFromCenter(const glm::vec2& p, float w, float h) {
    setFromCenter(p.x, p.y, w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::translate(float dx, float dy) {
    translateX(dx);
    translateY(dy);
}

//----------------------------------------------------------
void afw_rectangle_class::translate(const glm::vec3& dp) {
    translate(glm::vec2(dp));
}

//----------------------------------------------------------
void afw_rectangle_class::translate(const glm::vec2& dp) {
    translateX(dp.x);
    translateY(dp.y);
}

//----------------------------------------------------------
void afw_rectangle_class::translateX(float dx) {
    x += dx;
}

//----------------------------------------------------------
void afw_rectangle_class::translateY(float dy) {
    y += dy;
}

//----------------------------------------------------------
void afw_rectangle_class::scale(float s) {
    scaleWidth(s);
    scaleHeight(s);
}

//----------------------------------------------------------
void afw_rectangle_class::scale(float sX, float sY) {
    scaleWidth(sX);
    scaleHeight(sY);
}

//----------------------------------------------------------
void afw_rectangle_class::scale(const glm::vec3& s) {
    scale(glm::vec2(s));
}

//----------------------------------------------------------
void afw_rectangle_class::scale(const glm::vec2& s) {
    scaleWidth(s.x);
    scaleHeight(s.y);
}

//----------------------------------------------------------
void afw_rectangle_class::scaleWidth(float  sX) {
    width *= sX;
}
//----------------------------------------------------------
void afw_rectangle_class::scaleHeight(float sY) {
    height *= sY;
}

//----------------------------------------------------------
void afw_rectangle_class::scaleFromCenter(float s) {
    scaleFromCenter(s, s);
}

//----------------------------------------------------------
void afw_rectangle_class::scaleFromCenter(float sX, float sY) {
    scaleFromCenter({ sX,sY,0.f });
}

//----------------------------------------------------------
void afw_rectangle_class::scaleFromCenter(const glm::vec3& s) {
    scaleFromCenter(glm::vec2(s));
}

//----------------------------------------------------------
void afw_rectangle_class::scaleFromCenter(const glm::vec2& s) {
    //if (ofIsFloatEqual(s.x, 1.0f) && ofIsFloatEqual(s.y, 1.0f)) return; // nothing to do

    if (std::abs(s.x - 1.0f) <= std::numeric_limits<float>::epsilon() * std::abs(s.x)
        && std::abs(s.y - 1.0f) <= std::numeric_limits<float>::epsilon() * std::abs(s.y))  
    {
        return; // nothing to do
    }

    float newWidth = width * s.x;
    float newHeight = height * s.y;

    auto center = getCenter();

    x = center.x - newWidth / 2.0f;
    y = center.y - newHeight / 2.0f;

    width = newWidth;
    height = newHeight;
}

//----------------------------------------------------------
void afw_rectangle_class::scaleTo(const afw_rectangle_class& targetRect,
    ofScaleMode scaleMode) {

    if (scaleMode == OF_SCALEMODE_FIT) {
        scaleTo(targetRect,
            OF_ASPECT_RATIO_KEEP,
            OF_ALIGN_HORZ_CENTER,
            OF_ALIGN_VERT_CENTER);
    }
    else if (scaleMode == OF_SCALEMODE_FILL) {
        scaleTo(targetRect,
            OF_ASPECT_RATIO_KEEP_BY_EXPANDING,
            OF_ALIGN_HORZ_CENTER,
            OF_ALIGN_VERT_CENTER);
    }
    else if (scaleMode == OF_SCALEMODE_CENTER) {
        alignTo(targetRect,
            OF_ALIGN_HORZ_CENTER,
            OF_ALIGN_VERT_CENTER);
    }
    else if (scaleMode == OF_SCALEMODE_STRETCH_TO_FILL) {
        scaleTo(targetRect,
            OF_ASPECT_RATIO_IGNORE,
            OF_ALIGN_HORZ_CENTER,
            OF_ALIGN_VERT_CENTER);
    }
    else {
        scaleTo(targetRect,
            OF_ASPECT_RATIO_KEEP);
    }
}

//----------------------------------------------------------
void afw_rectangle_class::scaleTo(const afw_rectangle_class& targetRect,
    ofAspectRatioMode subjectAspectRatioMode,
    ofAlignHorz sharedHorzAnchor,
    ofAlignVert sharedVertAnchor) {
    scaleTo(targetRect,
        subjectAspectRatioMode,
        sharedHorzAnchor,
        sharedVertAnchor,
        sharedHorzAnchor,
        sharedVertAnchor);
}

//----------------------------------------------------------
void afw_rectangle_class::scaleTo(const afw_rectangle_class& targetRect,
    ofAspectRatioMode aspectRatioMode,
    ofAlignHorz modelHorzAnchor,
    ofAlignVert modelVertAnchor,
    ofAlignHorz thisHorzAnchor,
    ofAlignVert thisVertAnchor) {

    float tw = targetRect.getWidth();    // target width
    float th = targetRect.getHeight();   // target height
    float sw = getWidth();   // subject width
    float sh = getHeight();  // subject height

    if (aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING ||
        aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
        if (fabs(sw) >= std::numeric_limits<float>::epsilon() || fabs(sh) >= FLT_EPSILON) {
            float wRatio = fabs(tw) / fabs(sw);
            float hRatio = fabs(th) / fabs(sh);
            if (aspectRatioMode == OF_ASPECT_RATIO_KEEP_BY_EXPANDING) {
                scale(std::max(wRatio, hRatio));
            }
            else if (aspectRatioMode == OF_ASPECT_RATIO_KEEP) {
                scale(std::min(wRatio, hRatio));
            }
        }
        else {
            std::cout << "afw_rectangle_class" << "scaleTo(): no scaling applied to avoid divide by zero, rectangle has 0 width and/or height: " << sw << "x" << sh << std::endl;
        }
    }
    else if (aspectRatioMode == OF_ASPECT_RATIO_IGNORE) {
        width = tw;
        height = th;
    }
    else {
        std::cout << "afw_rectangle_class" << "scaleTo(): unknown ofAspectRatioMode = " << aspectRatioMode << ", using OF_ASPECT_RATIO_IGNORE" << std::endl;
        width = tw;
        height = th;
    }

    // now align if anchors are not ignored.
    alignTo(targetRect,
        modelHorzAnchor,
        modelVertAnchor,
        thisHorzAnchor,
        thisVertAnchor);

}

//----------------------------------------------------------
void afw_rectangle_class::alignToHorz(const float& targetX,
    ofAlignHorz thisHorzAnchor) {

    if (thisHorzAnchor != OF_ALIGN_HORZ_IGNORE) {
        translateX(targetX - getHorzAnchor(thisHorzAnchor));
    }
    else {
        std::cout << "afw_rectangle_class" << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied" << std::endl;
    }
}

//----------------------------------------------------------
void afw_rectangle_class::alignToHorz(const afw_rectangle_class& targetRect,
    ofAlignHorz sharedAnchor) {

    alignToHorz(targetRect, sharedAnchor, sharedAnchor);
}

//----------------------------------------------------------
void afw_rectangle_class::alignToHorz(const afw_rectangle_class& targetRect,
    ofAlignHorz targetHorzAnchor,
    ofAlignHorz thisHorzAnchor) {

    if (targetHorzAnchor != OF_ALIGN_HORZ_IGNORE &&
        thisHorzAnchor != OF_ALIGN_HORZ_IGNORE) {
        alignToHorz(targetRect.getHorzAnchor(targetHorzAnchor), thisHorzAnchor);
    }
    else {
        if (targetHorzAnchor == OF_ALIGN_HORZ_IGNORE) {
            std::cout << "afw_rectangle_class" << "alignToHorz(): targetHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied" << std::endl;
        }
        else {
            std::cout << "afw_rectangle_class" << "alignToHorz(): thisHorzAnchor == OF_ALIGN_HORZ_IGNORE, no alignment applied" << std::endl;
        }
    }

}

//----------------------------------------------------------
void afw_rectangle_class::alignToVert(const float& targetY,
    ofAlignVert thisVertAnchor) {

    if (thisVertAnchor != OF_ALIGN_VERT_IGNORE) {
        translateY(targetY - getVertAnchor(thisVertAnchor));
    }
    else {
        std::cout << "afw_rectangle_class" << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied" << std::endl;
    }
}

//----------------------------------------------------------
void afw_rectangle_class::alignToVert(const afw_rectangle_class& targetRect,
    ofAlignVert sharedAnchor) {

    alignToVert(targetRect, sharedAnchor, sharedAnchor);
}

//----------------------------------------------------------
void afw_rectangle_class::alignToVert(const afw_rectangle_class& targetRect,
    ofAlignVert targetVertAnchor,
    ofAlignVert thisVertAnchor) {

    if (targetVertAnchor != OF_ALIGN_VERT_IGNORE &&
        thisVertAnchor != OF_ALIGN_VERT_IGNORE) {
        alignToVert(targetRect.getVertAnchor(targetVertAnchor), thisVertAnchor);
    }
    else {
        if (targetVertAnchor == OF_ALIGN_VERT_IGNORE) {
            std::cout << "afw_rectangle_class" << "alignToVert(): targetVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied" << std::endl;
        }
        else {
            std::cout << "afw_rectangle_class" << "alignToVert(): thisVertAnchor == OF_ALIGN_VERT_IGNORE, no alignment applied" << std::endl;
        }

    }
}

//----------------------------------------------------------
void afw_rectangle_class::alignTo(const glm::vec3& targetPoint,
    ofAlignHorz thisHorzAnchor,
    ofAlignVert thisVertAnchor) {

    alignTo(glm::vec2(targetPoint), thisHorzAnchor, thisVertAnchor);
}

//----------------------------------------------------------
void afw_rectangle_class::alignTo(const glm::vec2& targetPoint,
    ofAlignHorz thisHorzAnchor,
    ofAlignVert thisVertAnchor) {

    alignToHorz(targetPoint.x, thisHorzAnchor);
    alignToVert(targetPoint.y, thisVertAnchor);
}


//----------------------------------------------------------
void afw_rectangle_class::alignTo(const afw_rectangle_class& targetRect,
    ofAlignHorz sharedHorzAnchor,
    ofAlignVert sharedVertAnchor) {
    alignTo(targetRect,
        sharedHorzAnchor,
        sharedVertAnchor,
        sharedHorzAnchor,
        sharedVertAnchor);
}

//----------------------------------------------------------
void afw_rectangle_class::alignTo(const afw_rectangle_class& targetRect,
    ofAlignHorz targetHorzAnchor,
    ofAlignVert targetVertAnchor,
    ofAlignHorz thisHorzAnchor,
    ofAlignVert thisVertAnchor) {

    alignToHorz(targetRect, targetHorzAnchor, thisHorzAnchor);
    alignToVert(targetRect, targetVertAnchor, thisVertAnchor);
}

//----------------------------------------------------------
bool afw_rectangle_class::inside(float px, float py) const {
    return inside({ px,py });
}

//----------------------------------------------------------
bool afw_rectangle_class::inside(const glm::vec3& p) const {
    return inside(glm::vec2(p));
}

//----------------------------------------------------------
bool afw_rectangle_class::inside(const glm::vec2& p) const {
    return p.x > getMinX() && p.y > getMinY() &&
        p.x < getMaxX() && p.y < getMaxY();
}

//----------------------------------------------------------
bool afw_rectangle_class::inside(const afw_rectangle_class& rect) const {
    return inside(rect.getMinX(), rect.getMinY()) &&
        inside(rect.getMaxX(), rect.getMaxY());
}

//----------------------------------------------------------
bool afw_rectangle_class::inside(const glm::vec3& p0, const glm::vec3& p1) const {
    // check to see if a line segment is inside the rectangle
    return inside(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
bool afw_rectangle_class::inside(const glm::vec2& p0, const glm::vec2& p1) const {
    // check to see if a line segment is inside the rectangle
    return inside(p0) && inside(p1);
}

//----------------------------------------------------------
bool afw_rectangle_class::intersects(const afw_rectangle_class& rect) const {
    return (getMinX() < rect.getMaxX() && getMaxX() > rect.getMinX() &&
        getMinY() < rect.getMaxY() && getMaxY() > rect.getMinY());
}

//----------------------------------------------------------
//bool afw_rectangle_class::intersects(const glm::vec3& p0, const glm::vec3& p1) const {
//    return intersects(glm::vec2(p0), glm::vec2(p1));
//}

//----------------------------------------------------------
//bool afw_rectangle_class::intersects(const glm::vec2& p0, const glm::vec2& p1) const {
//    // check for a line intersection
//    glm::vec2 p;
//
//    glm::vec2 topLeft(getTopLeft());
//    glm::vec2 topRight(getTopRight());
//    glm::vec2 bottomRight(getBottomRight());
//    glm::vec2 bottomLeft(getBottomLeft());
//
//    return inside(p0) || // check end inside
//        inside(p1) || // check end inside
//        ofLineSegmentIntersection(p0, p1, topLeft, topRight, p) || // cross top
//        ofLineSegmentIntersection(p0, p1, topRight, bottomRight, p) || // cross right
//        ofLineSegmentIntersection(p0, p1, bottomRight, bottomLeft, p) || // cross bottom
//        ofLineSegmentIntersection(p0, p1, bottomLeft, topLeft, p);   // cross left
//}

//----------------------------------------------------------
void afw_rectangle_class::growToInclude(float px, float py) {
    growToInclude({ px,py });
}

//----------------------------------------------------------
void afw_rectangle_class::growToInclude(const glm::vec3& p) {
    growToInclude(glm::vec2(p));
}

//----------------------------------------------------------
void afw_rectangle_class::growToInclude(const glm::vec2& p) {
    float x0 = std::min(getMinX(), p.x);
    float x1 = std::max(getMaxX(), p.x);
    float y0 = std::min(getMinY(), p.y);
    float y1 = std::max(getMaxY(), p.y);
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0, y0, w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::growToInclude(const afw_rectangle_class& rect) {
    float x0 = std::min(getMinX(), rect.getMinX());
    float x1 = std::max(getMaxX(), rect.getMaxX());
    float y0 = std::min(getMinY(), rect.getMinY());
    float y1 = std::max(getMaxY(), rect.getMaxY());
    float w = x1 - x0;
    float h = y1 - y0;
    set(x0, y0, w, h);
}

//----------------------------------------------------------
void afw_rectangle_class::growToInclude(const glm::vec3& p0, const glm::vec3& p1) {
    growToInclude(glm::vec2(p0), glm::vec2(p1));
}

//----------------------------------------------------------
void afw_rectangle_class::growToInclude(const glm::vec2& p0, const glm::vec2& p1) {
    growToInclude(p0);
    growToInclude(p1);
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::getIntersection(const afw_rectangle_class& rect) const {

    float x0 = std::max(getMinX(), rect.getMinX());
    float x1 = std::min(getMaxX(), rect.getMaxX());

    float w = x1 - x0;
    if (w < 0.0f) return afw_rectangle_class(0, 0, 0, 0); // short circuit if needed

    float y0 = std::max(getMinY(), rect.getMinY());
    float y1 = std::min(getMaxY(), rect.getMaxY());

    float h = y1 - y0;
    if (h < 0.0f) return afw_rectangle_class(0, 0, 0, 0);  // short circuit if needed

    return afw_rectangle_class(x0, y0, w, h);
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::getUnion(const afw_rectangle_class& rect) const {
    afw_rectangle_class united = *this;
    united.growToInclude(rect);
    return united;
}

//----------------------------------------------------------
void afw_rectangle_class::standardize() {
    if (width < 0.0f) {
        x += width;
        width = -1.0f * width;
    }

    if (height < 0.0f) {
        y += height;
        height = -1.0f * height;
    }
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::getStandardized() const {
    if (isStandardized()) {
        return *this;
    }
    else {
        afw_rectangle_class canRect(*this); // copy it
        canRect.standardize();
        return canRect;
    }
}

//----------------------------------------------------------
bool afw_rectangle_class::isStandardized() const {
    return width >= 0.0f && height >= 0.0f;
}

//----------------------------------------------------------
float afw_rectangle_class::getArea() const {
    return fabs(width) * fabs(height);
}

//----------------------------------------------------------
float afw_rectangle_class::getPerimeter() const {
    return 2.0f * fabs(width) + 2.0f * fabs(height);
}

//----------------------------------------------------------
float afw_rectangle_class::getAspectRatio() const {
    return fabs(width) / fabs(height);
}

//----------------------------------------------------------
bool afw_rectangle_class::isEmpty() const {
   // return ofIsFloatEqual(width, 0.0f) && ofIsFloatEqual(height, 0.0f);
    if(std::abs(width - 0.0f) <= std::numeric_limits<float>::epsilon() * std::abs(width) &&
        std::abs(height - 0.0f) <= std::numeric_limits<float>::epsilon() * std::abs(height))
    {
        return true;
    }

    return false;
}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getMin() const {
    return { getMinX(),getMinY(),0.f };
}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getMax() const {
    return { getMaxX(),getMaxY(),0.f };
}

//----------------------------------------------------------
float afw_rectangle_class::getMinX() const {
    return std::min(x, x + width);  // - width
}

//----------------------------------------------------------
float afw_rectangle_class::getMaxX() const {
    return std::max(x, x + width);  // - width
}

//----------------------------------------------------------
float afw_rectangle_class::getMinY() const {
    return std::min(y, y + height);  // - height
}

//----------------------------------------------------------
float afw_rectangle_class::getMaxY() const {
    return std::max(y, y + height);  // - height
}

//----------------------------------------------------------
float afw_rectangle_class::getLeft() const {
    return getMinX();
}

//----------------------------------------------------------
float afw_rectangle_class::getRight() const {
    return getMaxX();
}

//----------------------------------------------------------
float afw_rectangle_class::getTop() const {
    return getMinY();
}

//----------------------------------------------------------
float afw_rectangle_class::getBottom() const {
    return getMaxY();
}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getTopLeft() const {
    return getMin();
}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getTopRight() const {
    return { getRight(), getTop(), 0.f };
}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getBottomLeft() const {
    return glm::vec3(getLeft(), getBottom(), 0.f);
}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getBottomRight() const {
    return getMax();
}

//----------------------------------------------------------
float afw_rectangle_class::getHorzAnchor(ofAlignHorz anchor) const {
    switch (anchor) {
    case OF_ALIGN_HORZ_IGNORE:
        //ofLogError("afw_rectangle_class" << "getHorzAnchor(): unable to get anchor for OF_ALIGN_HORZ_IGNORE, returning 0";
        std::cout << "afw_rectangle_class" << "getHorzAnchor(): unable to get anchor for OF_ALIGN_HORZ_IGNORE, returning 0" << std::endl;
        return 0.0f;
    case OF_ALIGN_HORZ_LEFT:
        return getLeft();
    case OF_ALIGN_HORZ_RIGHT:
        return getRight();
    case OF_ALIGN_HORZ_CENTER:
        return getCenter().x;
    default:
        //ofLogError("afw_rectangle_class" << "getHorzAnchor(): unknown ofAlignHorz = " << anchor << ", returning 0.0";
        std::cout << "afw_rectangle_class" << "getHorzAnchor(): unknown ofAlignHorz = " << anchor << ", returning 0.0" << std::endl;
        return 0.0f;
    }
}

//----------------------------------------------------------
float afw_rectangle_class::getVertAnchor(ofAlignVert anchor) const {
    switch (anchor) {
    case OF_ALIGN_VERT_IGNORE:
        //ofLogError("afw_rectangle_class" << "getVertAnchor(): unable to get anchor for OF_ALIGN_VERT_IGNORE, returning 0.0";
        std::cout << "afw_rectangle_class" << "getVertAnchor(): unable to get anchor for OF_ALIGN_VERT_IGNORE, returning 0.0" << std::endl;
        return 0.0f;
    case OF_ALIGN_VERT_TOP:
        return getTop();
    case OF_ALIGN_VERT_BOTTOM:
        return getBottom();
    case OF_ALIGN_VERT_CENTER:
        return getCenter().y;
    default:
        // ofLogError("afw_rectangle_class" << "getVertAnchor(): unknown ofAlignVert = " << anchor << ", returning 0.0";
        std::cout << "afw_rectangle_class" << "getVertAnchor(): unknown ofAlignVert = " << anchor << ", returning 0.0" << std::endl;
        return 0.0f;
    }
}

//----------------------------------------------------------
bool afw_rectangle_class::operator != (const afw_rectangle_class& rect) const {
    return (x != rect.x) || (y != rect.y) || (width != rect.width) || (height != rect.height);
}

//----------------------------------------------------------
const glm::vec3& afw_rectangle_class::getPosition() const {
    return position;
}

//----------------------------------------------------------
//glm::vec3& afw_rectangle_class::getPositionRef() {
//    return position;
//}

//----------------------------------------------------------
glm::vec3 afw_rectangle_class::getCenter() const {
    return { x + width * 0.5f, y + height * 0.5f, 0.f };
}

//----------------------------------------------------------
float afw_rectangle_class::getX() const {
    return x;
}

//----------------------------------------------------------
float afw_rectangle_class::getY() const {
    return y;
}

//----------------------------------------------------------
float afw_rectangle_class::getWidth() const {
    return width;
}

//----------------------------------------------------------
float afw_rectangle_class::getHeight() const {
    return height;
}

//----------------------------------------------------------
//glm::vec2 afw_rectangle_class::map(const glm::vec2& coeff) const {
//    return glm::vec2(
//        ofMap(coeff.x, 0.0f, 1.0f, getMinX(), getMaxX(), false),
//        ofMap(coeff.y, 0.0f, 1.0f, getMinY(), getMaxY(), false)
//    );
//}

//----------------------------------------------------------

//afw_rectangle_class afw_rectangle_class::map(const afw_rectangle_class& coeff) const {
//    return afw_rectangle_class(
//        map(glm::vec2(coeff.getMinX(), coeff.getMinY())),
//        map(glm::vec2(coeff.getMaxX(), coeff.getMaxY()))
//    );
//}
//
//glm::vec2 afw_rectangle_class::mapClamp(const glm::vec2& coeff) const {
//    return glm::vec2(
//        ofMap(coeff.x, 0.0f, 1.0f, getMinX(), getMaxX(), true),
//        ofMap(coeff.y, 0.0f, 1.0f, getMinY(), getMaxY(), true)
//    );
//}
//
//afw_rectangle_class afw_rectangle_class::mapClamp(const afw_rectangle_class& coeff) const {
//    return afw_rectangle_class(
//        mapClamp(glm::vec2(coeff.getMinX(), coeff.getMinY())),
//        mapClamp(glm::vec2(coeff.getMaxX(), coeff.getMaxY()))
//    );
//}


//----------------------------------------------------------
afw_rectangle_class& afw_rectangle_class::operator = (const afw_rectangle_class& rect) {
    set(rect);
    return *this;
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::operator + (const glm::vec3& point) {
    return operator+(glm::vec2(point));
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::operator + (const glm::vec2& point) {
    afw_rectangle_class rect = *this;
    rect.x += point.x;
    rect.y += point.y;
    return rect;
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::operator - (const glm::vec3& point) {
    return operator-(glm::vec2(point));
}

//----------------------------------------------------------
afw_rectangle_class afw_rectangle_class::operator - (const glm::vec2& point) {
    afw_rectangle_class rect = *this;
    rect.x -= point.x;
    rect.y -= point.y;
    return rect;
}

//----------------------------------------------------------
//bool afw_rectangle_class::operator == (const afw_rectangle_class& rect) const {
//    return ofIsFloatEqual(x, rect.x) && ofIsFloatEqual(y, rect.y) && ofIsFloatEqual(width, rect.width) && ofIsFloatEqual(height, rect.height);
//}
//
////----------------------------------------------------------
bool afw_rectangle_class::isZero() const {
    //return ofIsFloatEqual(x, 0.0f) && ofIsFloatEqual(y, 0.0f) && ofIsFloatEqual(width, 0.0f) && ofIsFloatEqual(height, 0.0f);

    bool fex = std::abs(x - 0.0f) <= std::numeric_limits<float>::epsilon() * std::abs(x);
    bool fey = std::abs(y - 0.0f) <= std::numeric_limits<float>::epsilon() * std::abs(y);
    bool few = std::abs(width - 0.0f) <= std::numeric_limits<float>::epsilon() * std::abs(width);
    bool feh = std::abs(height - 0.0f) <= std::numeric_limits<float>::epsilon() * std::abs(height);

    return fex && fey && few && feh;
}
//
////----------------------------------------------------------
//ostream& operator<<(ostream& os, const afw_rectangle_class& rect) {
//    os << rect.position << ", " << rect.width << ", " << rect.height;
//    return os;
//}
//
////----------------------------------------------------------
//istream& operator>>(istream& is, afw_rectangle_class& rect) {
//    is >> rect.position;
//    is.ignore(2);
//    is >> rect.width;
//    is.ignore(2);
//    is >> rect.height;
//    return is;
//}
