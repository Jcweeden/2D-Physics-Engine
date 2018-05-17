#ifndef _Circle_
#define _Circle_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "ShapeBody.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include <iostream>
#include <string>
#include <vector>

class Circle : public ShapeBody {
public:

  Circle(int p_x, int p_y, float p_radius, /* int p_rotation,*/ int p_density, Uint32 p_colour);
  
  virtual void draw();
  virtual void update();
  virtual void clean();
  
  
  float getArea(); //calculate and return polygon area
  float getRadius() {return radius; }; //calculate and return polygon area

  
  void setRadius(float val) {radius = val; }

  const float pi = 3.14159265359;
  
protected:
  
  float radius;
  
};

#endif
