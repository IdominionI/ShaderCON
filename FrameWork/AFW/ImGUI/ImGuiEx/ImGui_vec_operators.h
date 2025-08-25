#pragma once

//#include "imgui.h"

#include <thirdparty/ImGUI/imgui_docking/imgui.h>

//Commented out operators as ImGui compilation without IMGUI_DEFINE_MATH_OPERATORS still causes
//compile errors. Need to modify ImGui source code to exclude test for IMGUI_DEFINE_MATH_OPERATORS


//static inline ImVec2  operator*(const ImVec2& lhs, const float rhs)     { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
//static inline ImVec2  operator/(const ImVec2& lhs, const float rhs)     { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
//static inline ImVec2  operator+(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
//static inline ImVec2  operator-(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
//static inline ImVec2  operator*(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
//static inline ImVec2  operator/(const ImVec2& lhs, const ImVec2& rhs)   { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
//static inline ImVec2  operator-(const ImVec2& lhs)                      { return ImVec2(-lhs.x, -lhs.y); }
//static inline ImVec2& operator*=(ImVec2& lhs, const float rhs)          { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
//static inline ImVec2& operator/=(ImVec2& lhs, const float rhs)          { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
//static inline ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
//static inline ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
//static inline ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
//static inline ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs)        { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }


// for some stupid reason C++ 20 complior calls the following as a C2084 "funcion already has a body" error 
// which it does not in pre c++ 20 cannot figure it out except that where it is used has so many auto type definitions
// may in turn effect this in imgui_gradient Mark.hpp. 
//static inline bool operator==( const ImVec2 lhs, const ImVec2 rhs){ 
//	return lhs.x == rhs.x && lhs.y == rhs.y; 
//} // not present in ImGui 


//static inline ImVec4  operator+(const ImVec4& lhs, const ImVec4& rhs)   { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
//static inline ImVec4  operator-(const ImVec4& lhs, const ImVec4& rhs)   { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
//static inline ImVec4  operator*(const ImVec4& lhs, const ImVec4& rhs)   { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }

//static inline bool operator==(const ImVec4 &lhs, const ImVec4 &rhs)   { 
//	return lhs.x == rhs.x && lhs.y == rhs.y &&  lhs.z == rhs.z && lhs.w == rhs.w;
//} // not present in ImGui 



