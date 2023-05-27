#include "Settings.h"

namespace ChaosCourse {

	const ColorRGB& Settings::getBgColor() const {
		return bgColor;
	}

	int Settings::getWidth() const {
		return imgWidth;
	}

	int Settings::getHeight() const {
		return imgHeight;
	}

	float Settings::getAspectRatio() const {
		return (float)imgWidth / (float)imgHeight;
	}

	void Settings::setBgColor(const ColorRGB& newBgColor) {
		bgColor = newBgColor;
	}

	void Settings::setWidth(int newWidth) {
		imgWidth = newWidth;
	}

	void Settings::setHeight(int newHeight) {
		imgHeight = newHeight;
	}

}