#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cassert>
#include <limits>

#define Assert(x) assert(x)
#define LIKELY [[likely]]
static constexpr float ASPECT_RATIO = 16.f / 9.f;
static constexpr int IMG_WIDTH = 1920;
static constexpr int IMG_HEIGHT = IMG_WIDTH / ASPECT_RATIO;
static constexpr int NUM_PIXELS = IMG_WIDTH * IMG_HEIGHT;
static constexpr int MAX_COLOR_COMP = 255;
static constexpr float EPSILON = std::numeric_limits<float>::epsilon() * 0.5f;
static constexpr float PI = 3.14159265358979323846;
static constexpr float SHADOW_BIAS = 1e-2f;
static constexpr float REFLECTION_BIAS = 1e-3f;
static constexpr float REFRACTION_BIAS = 1e-4f;
static constexpr int MAX_RAY_DEPTH = 4;
static constexpr size_t PIXELS_PER_THREAD = 16;
static constexpr float MAX_FLOAT = std::numeric_limits<float>::max();
static constexpr float MIN_FLOAT = std::numeric_limits<float>::min();

namespace SceneConstants {
    inline const char* STR_SCENE_SETTINGS = "settings";
    inline const char* STR_IMAGE_SETTINGS = "image_settings";
    inline const char* STR_BACKGROUND_COLOR = "background_color";
    inline const char* STR_IMAGE_WIDTH = "width";
    inline const char* STR_IMAGE_HEIGHT = "height";
    inline const char* STR_SCENE_LIGHTS = "lights";
    inline const char* STR_LIGHT_INTENSITY = "intensity";
    inline const char* STR_LIGHT_POSITION = "position";
    inline const char* STR_MATERIAL_INFO = "materials";
    inline const char* STR_MATERIAL_TYPE = "type";
    inline const char* STR_MATERIAL_ALBEDO = "albedo";
    inline const char* STR_MATERIAL_IOR = "ior";
    inline const char* STR_MATERIAL_SMOOTH = "smooth_shading";
    inline const char* STR_CAMERA_SETTINGS = "camera";
    inline const char* STR_CAMERA_POSITION = "position";
    inline const char* STR_ROTATION_MATRIX = "matrix";
    inline const char* STR_SCENE_OBJECT = "objects";
    inline const char* STR_MATERIAL_IDX = "material_index";
    inline const char* STR_VERTICES = "vertices";
    inline const char* STR_TRIANGLE_INDICES = "triangles";
};  

#endif  