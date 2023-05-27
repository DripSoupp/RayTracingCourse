#pragma once
namespace ChaosCourse {
	

  //json file string constants - used for parsing
	static const char* STR_SETTINGS = "settings";
	static const char* STR_BG_COLOR = "background_color";
	static const char* STR_IMG_SETTINGS = "image_settings";
	static const char* STR_WIDTH = "width";
	static const char* STR_HEIGHT = "height";
	static const char* STR_CAMERA = "camera";
	static const char* STR_POS = "position";
	static const char* STR_MATRIX = "matrix";
	static const char* STR_OBJECTS = "objects";
	static const char* STR_VERTICES = "vertices";
	static const char* STR_TRIANGLES = "triangles";
	static const char* STR_LIGHTS = "lights";
	static const char* STR_LIGHT_INTENSITY = "intensity";

	//Lighting
	static const CRTVector ALBEDO = CRTVector(0.6f, 0.6f, 0.6f);
	static const float SHADOW_BIAS = 0.1f;
}