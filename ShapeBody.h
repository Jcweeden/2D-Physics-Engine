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

  ShapeBody(int p_x, int p_y,/* int p_rotation,*/ int p_mass, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA);

  virtual void draw();
  virtual void update();
  virtual void clean();
  
  Vector2D& getPosition() {return position; } 
  int getPositionX() {return position.getX(); }
  int getPositionY() {return position.getY(); }

  float& getRotation() {return rotation; }
  float& getRotationalVelocity() { return rotationalVelocity; }

  float getArea() {}

  virtual void setVelocity(float p_x, float p_y) { velocity.setX(p_x); velocity.setY(p_y); }
  virtual void setAcceleration(float p_x, float p_y) { acceleration.setX(p_x); acceleration.setY(p_y); }
  
  virtual void setDamping(float val) { damping = val; }
  virtual float getDamping() { return damping; }

  virtual void setMass(const float mass); //sets inverse mass
  virtual float getMass() { return inverseMass; }
  
protected:

  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;

  Vector2D forceAccumulated; //holds force to be applied at the next iteration. cleared after each iteration
  void clearAccumForces();
  
  float damping;


  float inverseMass;
 


  
  float rotation;

  //x,y diff from first coordinate to the center of mass
  //Vector2D centerOfMass;
  void calculateCenterOfMass();

  float area; //calulated in child constructor
  //float density; //mass


  
  float rotationalVelocity;

  

  Uint32 colourR, colourG, colourB, colourA;




  
};

#endif
