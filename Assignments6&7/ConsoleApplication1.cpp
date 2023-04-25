#include <fstream>
#include <iostream>
#include <assert.h>
#include <vector>
#include <utility>
#include <string>

#include "ColorRGB.h"
#include "CRTVector.h"
#include "CRTMatrix.h"
#include "MathUtil.h"
#include "CRTTriangle.h"
#include "CRTRay.h"
#include "CRTCamera.h"
#include "Scene.h"
#include "Settings.h"
#include "Render.h"

using namespace ChaosCourse;

int main() {
  Render renderer;


  /*Scene scene1("scene0.crtscene");
  renderer.render(scene1, "scene0.ppm");*/


  //Scene scene2("scene1.crtscene");
  //renderer.render(scene2, "scene1.ppm");

  //Scene scene3("scene2.crtscene");

  //Scene scene4("scene3.crtscene");
  //renderer.render(scene4, "scene3.ppm");

  Scene scene5("scene4.crtscene");
  renderer.render(scene5, "scene4.ppm");

  
  return 0;
}