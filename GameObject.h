#ifndef _GameObject_
#define _GameObject_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Game.h"
#include "Vector2D.h"
#include <iostream>
#include <string>

class GameObject {

public:


  GameObject() {}
  GameObject(GameObject const &) = delete; //delete copy constr
  virtual ~GameObject() {}  
  
  virtual void draw() = 0;
  virtual void update() = 0;
  virtual void clean() = 0;


  
  virtual Vector2D& getPosition() = 0;
  virtual int getPositionX() = 0;
  virtual int getPositionY() = 0;
  
  virtual float& getRotation() = 0;
  /*
  virtual void setVelocity(float p_x, float p_y) = 0;
  virtual void setAcceleration(float p_x, float p_y) = 0;
  */
protected:

};

#endif
