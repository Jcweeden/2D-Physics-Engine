#ifndef _BuoyancyDemo_
#define _BuoyancyDemo_


#include "Demo.h"
#include <math.h>
#include <vector>
#include "ShapeBody.h"
#include "Vector2D.h"
#include "Contacts.h"
#include "InputHandler.h"
#include "World.h"
#include "Platform.h"
#include "ForceGenerator.h"
#include "SDL2/SDL2_gfxPrimitives.h" 


class BuoyancyDemo : public Demo
{
  //vector holding all shapes
  std::vector<ShapeBody*> m_shapes;
  //radius of a shape
  float shapeRadius;

  //array holding platforms
  Platform *platforms;
  //number of platforms in platforms array
  int platformsCount;

  //holds all shapes and platforms, and updates shapes from the contacts between them
  World simulation;


  //holds all the spring forces that are applied to shapes
  //updateForces() is called on the registry to apply the forces
  ForceRegistry gravityRegistry;
  ForceRegistry buoyancyRegistry;

  //when true the element is enabled and appropriate forces are applied to shapes
  bool waterEnabled;
  bool windEnabled;

  //list of vertices used to draw blue rect representing water on-screen
  std::vector<Sint16> waterVerticesX;
  std::vector<Sint16> waterVerticesY;

  //list of vertices used to draw gray rect representing wind on-screen
  std::vector<Sint16> windVerticesX;
  std::vector<Sint16> windVerticesY;

public:
  BuoyancyDemo(); //constr
  ~BuoyancyDemo() {}; //destr

  //draws the shapes on screen
  virtual void draw();

  //applies physics to shapes
  virtual void update();

  //deletes all objects for cleanup upon closing demo
  virtual void clean();

  //restarts the scene, deleting necessary variables
  virtual void reset();

  //collects keyboard/mouse input and applies appropriate actions
  virtual void handleInput();

  //call to spawn shapes upon button press
  void spawnShapes(float mass, int colour);
    
  //switches water on/off
  void switchWater();

  //switches wind on/off
  void switchWind();
};

#endif
