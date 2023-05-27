#pragma once
#include "ColorRGB.h"

namespace ChaosCourse {

	/*
	* A class to store and manage scene settings.
	*  - Image resolution
	*  - Background colour
	*/
	class Settings {
	public:
		//Default settings - 1920 x 1080, black screen, red triangles
		Settings() : imgWidth(1920), imgHeight(1080), bgColor() {}
		Settings(int width, int height, const ColorRGB& bgColor) :
			imgWidth(width), imgHeight(height), bgColor(bgColor) {}

		//Getters

		//Get the background colour
		const ColorRGB& getBgColor() const;
		//Image width in pixels.
		int getWidth() const;
		//Image height in pixels.
		int getHeight() const;
		//Here aspect ratio is defined as width/height
		float getAspectRatio() const;

		//Setters
		void setBgColor(const ColorRGB& newBgColor);
		void setWidth(int newWidth);
		void setHeight(int newHeight);

	private:
		ColorRGB bgColor;
		int imgWidth;
		int imgHeight;

	};
}