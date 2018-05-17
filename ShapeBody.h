#ifndef _ShapeBody_
#define _ShapeBody_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include <iostream>
#include <string>

class ShapeBody : public GameObject {

public:

  ShapeBody(int p_x, int p_y,/* int p_rotation,*/ int p_density, Uint32 p_colour);

  virtual void draw();
  virtual void update();
  virtual void clean();
  
  Vector2D& getPosition() {return position; } 
  int getPositionX() {return position.getX(); }
  int getPositionY() {return position.getY(); }

  float& getRotation() {return rotation; }
  float& getRotationalVelocity() { return rotationalVelocity; }

  float getArea() {}
  float getMass() { area * density; }
  
protected:

  Vector2D position;
  float rotation;

  //x,y diff from first coordinate to the center of mass
  //Vector2D centerOfMass;
  void calculateCenterOfMass();

  float area; //calulated in child constructor
  float density; //mass
  
  Vector2D velocity;
  float rotationalVelocity;

  
  Vector2D acceleration; //

  Uint32 colour;
  
};

#endif
