#ifndef _GameObject_
#define _GameObject_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "Game.h"
#include "LoaderParams.h"
#include <iostream>
#include <string>

class GameObject {

public:
  virtual void draw() = 0;
  virtual void update() = 0;
  virtual void clean() = 0;


  
  
protected:

  GameObject(const LoaderParams* pParams) {}
  virtual ~GameObject() {}  
};

#endif
