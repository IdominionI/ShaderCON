#pragma once

#include <glm/glm.hpp>

typedef glm::vec4 color_type;

class afw_color_class
{
public:
	afw_color_class() {}
	~afw_color_class() {}

	color_type colorf = { 0.0f,0.0f,0.0f,1.0f };

	glm::vec4 color(int r, int g, int b, int a) {
		if (r < 0) r = 0;
		if (g < 0) g = 0;
		if (b < 0) b = 0;
		if (a < 0) a = 0;

		if (r > 255) r = 255;
		if (g > 255) g = 255;
		if (b > 255) b = 255;
		if (a > 255) a = 255;
		
		colorf = { float(r) / 255.0f,float(g) / 255.0f, float(b) / 255.0f, float(a) / 255.0f };
		return colorf;
	}

	glm::vec4 color(int r, int g, int b) {
		if (r < 0) r = 0;
		if (g < 0) g = 0;
		if (b < 0) b = 0;

		if (r > 255) r = 255;
		if (g > 255) g = 255;
		if (b > 255) b = 255;


		colorf = { float(r) / 2550.0f,float(g) / 2550.0f, float(b) / 2550.0f, 1.0f };
		return colorf;
	}

	glm::vec4 color(int c) {
		if (c < 0) c = 0;
		if (c > 255) c = 255;

		float fc = float(c) / 255.0f;

		colorf = { fc,fc,fc, 1.0f };
		return colorf;
	}

	glm::vec4 color(int c, int a) {
		if (c < 0) c = 0;
		if (a < 0) a = 0;

		if (c > 255) c = 255;
		if (a > 255) a = 255;

		float fc = float(c) / 255.0f;

		colorf = { fc,fc,fc, float(a) / 255.0f };
		return colorf;
	}

	// -------------------------------------
	glm::vec4 color(float r, float g, float b, float a) {
		if (r < 0.0f) r = 0.0f;
		if (g < 0.0f) g = 0.0f;
		if (b < 0.0f) b = 0.0f;
		if (a < 0.0f) a = 0.0f;

		if (r > 1.0f) r = 1.0f;
		if (g > 1.0f) g = 1.0f;
		if (b > 1.0f) b = 1.0f;
		if (a > 1.0f) a = 1.0f;

		colorf = { r,g,b,a };
		return colorf;
	}

	glm::vec4 color(float r, float g, float b) {
		if (r < 0.0f) r = 0.0f;
		if (g < 0.0f) g = 0.0f;
		if (b < 0.0f) b = 0.0f;

		if (r > 1.0f) r = 1.0f;
		if (g > 1.0f) g = 1.0f;
		if (b > 1.0f) b = 1.0f;

		colorf = { r,g,b, 1.0f };
		return colorf;
	}

	glm::vec4 color(float c) {
		if (c < 0.0f) c = 0.0f;
		if (c > 1.0f) c = 1.0f;

		colorf = { c,c,c, 1.0f };
		return colorf;
	}

	glm::vec4 color(float c, float a) {
		if (c < 0.0f) c = 0.0f;
		if (a < 0.0f) a = 0.0f;

		if (c > 1.0f) c = 1.0f;
		if (a > 1.0f) a = 1.0f;

		colorf = { c,c,c, a };
		return colorf;
	}

private:

};
