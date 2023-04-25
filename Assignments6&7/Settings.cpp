#include "Settings.h"

namespace ChaosCourse {

	const ColorRGB& Settings::getBgColor() const {
		return bgColor;
	}

	int Settings::getWidth() const {
		return imageWidth;
	}

	int Settings::getHeight() const {
		return imageHeight;
	}

	float Settings::getAspectRatio() const {
		return (float)imageWidth / (float)imageHeight;
	}

	void Settings::setBgColor(const ColorRGB& newBgColor) {
		bgColor = newBgColor;
	}

	void Settings::setWidth(int newWidth) {
		imageWidth = newWidth;
	}

	void Settings::setHeight(int newHeight) {
		imageHeight = newHeight;
	}

}