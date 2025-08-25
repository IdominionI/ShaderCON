#pragma once

#include <FrameWork/AFW/Graphics/afw_node_base.h>
#include <FrameWork/AFW/Graphics/afw_color.h>

enum ofLightType {
	OF_LIGHT_POINT = 0,
	OF_LIGHT_DIRECTIONAL = 1,
	OF_LIGHT_SPOT = 2,
	OF_LIGHT_AREA = 3 // Only programmable renderer
};

typedef glm::vec4 float_color;
typedef glm::uvec3 ofColor;

//typedef afw_color_class color;

class ofLight : public afw_node_base_class {
public:
	ofLight();

	afw_color_class afw_color;

	// VWCUSTOM ADDITION BEGIN +++++++++++++++++
	bool light_is_on = true;
	//bool display_overlay = false;
	//bool display_shadow_frustrum = false;

	// ADDITION END +++++++++++++++++

	void setup();
	void enable();
	void disable();
	bool getIsEnabled() const;

	void setDirectional();
	bool getIsDirectional() const;

	void setSpotlight(float spotCutOff = 45.f, float exponent = 0.f);
	bool getIsSpotlight() const;
	void setSpotlightCutOff(float spotCutOff);
	float getSpotlightCutOff() const;
	void setSpotConcentration(float exponent);
	float getSpotConcentration() const;

	void setPointLight();
	bool getIsPointLight() const;
	void setAttenuation(float constant = 1.f, float linear = 0.f, float quadratic = 0.f);
	float getAttenuationConstant() const;
	float getAttenuationLinear() const;
	float getAttenuationQuadratic() const;

	void setAreaLight(float width, float height);
	bool getIsAreaLight() const;

	int getType() const;

	void setAmbientColor(const float_color& c);
	void setDiffuseColor(const float_color& c);
	void setSpecularColor(const float_color& c);

	float_color getAmbientColor() const;
	float_color getDiffuseColor() const;
	float_color getSpecularColor() const;

	int getLightID() const;

	//bool shouldRenderShadowDepthPass();
	//int getNumShadowDepthPasses();
	//bool beginShadowDepthPass();
	//bool endShadowDepthPass();
	//bool beginShadowDepthPass(GLenum aPassIndex);
	//bool endShadowDepthPass(GLenum aPassIndex);

	//ofShadow& getShadow() { return shadow; }

	class Data {
	public:
		Data();
		~Data();

		float_color ambientColor;
		float_color diffuseColor;
		float_color specularColor;

		float attenuation_constant;
		float attenuation_linear;
		float attenuation_quadratic;

		ofLightType lightType;

		int glIndex;
		int isEnabled;
		float spotCutOff;
		float exponent;
		glm::vec4 position;
		glm::vec3 direction;

		float width;
		float height;
		glm::vec3 up;
		glm::vec3 right;
		/// weak link back to renderer for which this light was created/setup
		//std::weak_ptr<ofBaseGLRenderer> rendererP;
	};

	// VWCUSTOM  BEGIN SUBSTITUTED ######################
	void define_light_settings(Data light_settings) {
		*data = light_settings;
	}

	//std::shared_ptr<Data> get_light_settings() {
	//	return data;
	//}

	// SUBSTITUTED END ###########################################
	// VWCUSTOM ADDED BEGIN +++++++++++++++++++++++++++

	float intensity; // Brightness of light source Why is this missing in ofLight

	// to do or to have performed in vw_scene_lights_class
	// dependent upon type of light
	//virtual void render_overlay() {
	//void render_overlay() {
	//	if (display_overlay) {
	//		draw();
	//	}

	//	if (shadow.hasActiveShadows() && display_shadow_frustrum) {
	//		shadow.drawFrustum();
	//	}
	//}
	// ADDED END +++++++++++++++++++++++++++++++++++++++++++


protected:
	//ofShadow shadow;

private:
	//void customDraw(const ofBaseRenderer* renderer) const;
	Data *data;
	//std::shared_ptr<Data> data;
	// update opengl light 
	// this method overrides ofNode to catch the changes and update glLightv(GL_POSITION)
	virtual void onPositionChanged();
	virtual void onOrientationChanged();
};


//std::vector<ofLight::Data *> ofLightsData();

