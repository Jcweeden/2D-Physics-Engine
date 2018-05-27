#ifndef _ShapeBody_
#define _ShapeBody_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "GameObject.h"
#include "Vector2D.h"
#include "InputHandler.h"
#include <iostream>
#include <string>

class ShapeBody : public GameObject {

public:

  ShapeBody(int p_x, int p_y,/* int p_rotation,*/ int p_mass, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA);

  ShapeBody();

  virtual void draw();
  virtual void update();
  virtual void clean();

  void setPhysicsEnabled(bool val) { arePhysicsEnabled = val; }

  void setPosition(Vector2D p_position) {position = p_position; }
  void setPositionX(int val) {position.setX(val); }
  void setPositionY(int val) {position.setY(val); }
  Vector2D& getPosition() {return position; } 
  int getPositionX() {return position.getX(); }
  int getPositionY() {return position.getY(); }

  float& getRotation() {return rotation; }
  float& getRotationalVelocity() { return rotationalVelocity; }

  virtual float getArea() {return 0; } //REWORK

  virtual Vector2D getVelocity() { return velocity; }
  virtual void setVelocity(float p_x, float p_y) { velocity.setX(p_x); velocity.setY(p_y); }
  virtual void setVelocity(Vector2D p_velocity) { velocity = p_velocity; }

  Vector2D getAcceleration() { return acceleration; }
  virtual void setAcceleration(float p_x, float p_y) { acceleration.setX(p_x); acceleration.setY(p_y); }
  
  virtual void setDamping(float val) { damping = val; }
  virtual float getDamping() { return damping; }

  virtual void setMass(const float mass); //sets inverse mass
  virtual float getMass() { return ((float)1.0f)/inverseMass; }
  virtual bool hasFiniteMass() { return inverseMass >= 0.0f; }

  
  void addForce(const Vector2D &force);

  void physicsIntegration();


  void clearAccumForces();

protected:

  
  bool arePhysicsEnabled;
  bool isHeldByMouse;
  //bool mouseReleased; //set to true upon initial click
  int offsetFromMouseX, offsetFromMouseY;
  
  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;

  Vector2D forceAccumulated; //sum of all forces to be applied in a frame. cleared after each update

  
  float damping;
  float inverseMass;
  
  float rotation;

  //x,y diff from first coordinate to the center of mass
  //Vector2D centerOfMass;
  //void calculateCenterOfMass();

  float area; //calulated in child constructor

  float rotationalVelocity;
  

  Uint32 colourR, colourG, colourB, colourA;
};

#endif
