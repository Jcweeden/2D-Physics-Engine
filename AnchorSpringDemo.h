#ifndef _AnchorSpringDemo_
#define _AnchorSpringDemo_

#include "Game.h"
#include "Demo.h"
#include "Polygon.h"
#include "Circle.h"
#include "ForceGenerator.h"
#include "SDL2/SDL2_gfxPrimitives.h" 

#include <vector>

class AnchorSpringDemo : public Demo
{
public:
  
std::vector<ShapeBody*> springs;

//holds all forceGenerators with their associated objects
ForceRegistry registry;

  
public:
    // Instantiates all objects in a new BlobDemo
    AnchorSpringDemo(); //constr
    ~AnchorSpringDemo(); //destr

    // draws the blobs on screen
    virtual void draw();

    // applies physics to blobs
    virtual void update();

    /** Handle a key press. */
    //virtual void key(unsigned char key);

    //deletes all objects for cleanup upon closing demo
    virtual void clean();

    //restarts the scene
    virtual void reset();



};

#endif
