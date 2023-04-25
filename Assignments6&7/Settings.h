#pragma once
#include "ColorRGB.h"

namespace ChaosCourse {
	class Settings {
	public:
		Settings() : imageWidth(1920), imageHeight(1080), bgColor() {}
		Settings(int width, int height, const ColorRGB& bgColor) :
			imageWidth(width), imageHeight(height), bgColor(bgColor) {}

		const ColorRGB& getBgColor() const;
		int getWidth() const;
		int getHeight() const;
		float getAspectRatio() const;

		void setBgColor(const ColorRGB& newBgColor);
		void setWidth(int newWidth);
		void setHeight(int newHeight);

	private:
		ColorRGB bgColor;
		int imageWidth;
		int imageHeight;


	};

}