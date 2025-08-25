#pragma once
#include <string>

#include <FrameWork/AFW/Graphics/afw_color.h>

struct ofMaterialSettings {
	color_type diffuse{ 0.8f, 0.8f, 0.8f, 1.0f }; ///< diffuse reflectance
	color_type ambient{ 0.2f, 0.2f, 0.2f, 1.0f }; ///< ambient reflectance
	color_type specular{ 0.0f, 0.0f, 0.0f, 1.0f }; ///< specular reflectance
	color_type emissive{ 0.0f, 0.0f, 0.0f, 1.0f }; ///< emitted light intensity
	float shininess{ 0.2f }; ///< specular exponent

	bool isPbr = false;
	// PBR //
	// the diffuse color is used for the albedo color, metallic color
	float metallic{ 0.0 }; ///< metallic properties, can be fraction between 0 -> 1
	float roughness{ 0.5 }; ///< roughness
	float reflectance{ 0.5 }; ///< reflectance only applies to dielectric materials (non-metallic)
	bool clearCoatEnabled = false; ///<this must be enabled for clear coat to have effect, will be enabled if clear coat texture set
	float clearCoatStrength = 1.0;
	float clearCoatRoughness = 0.1;
	float displacementStrength = 0.2; ///< strength of vertex displacement from displacement map in world space
	float displacementNormalsStrength = 50.0; ///< strength of normal calculation based on displacement map
	float normalGeomToNormalMapMix = 0.95; ///< if a normal map is set, the mix between the geometry normal and normal from the normal / displacement map

	glm::vec2 texCoordScale = { 1.0,1.0 };

	// these are not to be set manually
	std::string postFragment = "";
	std::string customUniforms = ""; ///set by ofMaterial::setCustomUniform*  not to be set manually
	std::string uniqueIdString = ""; /// set by ofMaterial to cache shader
	std::string mainFragment = ""; /// override the default main function in the frag shader
	std::string mainFragmentKey = ""; /// access fragment main function with this key make unique for new instances

	std::string mainVertex = ""; /// override the default main function in the vertex shader
	std::string mainVertexKey = ""; /// access vertex main function with this key make unique for new instances

};