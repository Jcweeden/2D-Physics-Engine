#ifndef _ElasticMeshDemo_
#define _ElasticMeshDemo_


#include "Demo.h"
#include <math.h>
#include <vector>
#include "Vector2D.h"
#include "Contacts.h"
#include "InputHandler.h"
#include "World.h"
#include "ForceGenerator.h"
#include "SDL2/SDL2_gfxPrimitives.h" 
#include "ShapeLinks.h"
#include "Circle.h"


class ElasticMeshDemo : public Demo
{

  Circle *shapes;

  int shapesCount; //must be a square number (e.g. 4,9,16,25,36,49)

  int shapeRadius;
  
  //array holding cables
  ShapeCable *cables;
  //number of cables in platforms array
  int cablesCount;

  World world;

public:
    ElasticMeshDemo(); //constr
    virtual ~ElasticMeshDemo(); //destr

    // draws the blobs on screen
    virtual void draw();

    // applies physics to blobs
    virtual void update();

    //deletes all objects for cleanup upon closing demo
    virtual void clean();

    //restarts the scene
    virtual void reset();

    //collects keyboard/mouse input and applies appropriate actions
    virtual void handleInput();
};

#endif
