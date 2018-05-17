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

  ShapeBody(const LoaderParams* pParams);

  virtual void draw();
  virtual void update();
  virtual void clean();

  int getWidth() { return width; }
  int getHeight() { return height; }


  Vector2D& getPosition() {return position; } 
  int getPositionX() {return position.getX(); }
  int getPositionY() {return position.getY(); }

  float& getRotation() {return rotation; }

protected:

  //virtual ~GameObject() {}

  Vector2D position;
  Vector2D velocity;
  Vector2D acceleration;
  float rotation;

  
  int width;
  int height;

  int currentRow;
  int currentFrame;

  std::string textureID;
  
};

#endif
