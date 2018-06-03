#ifndef _BlobDemo_
#define _BlobDemo_


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


class BlobDemo : public Demo
{

//array holding blobs
ShapeBody *blobs;
//number of blobs in blobs array
int blobsCount;
//radius of a blob
float blobRadius;

//CONVERSION TO VECTOR
std::vector<ShapeBody*> m_blobs;



//array holding platforms
Platform *platforms;
//number of platforms in platforms array
int platformsCount;

World simulation;

BlobForceGenerator blobForceGenerator;


ForceRegistry gravityRegistry;
ForceRegistry buoyancyRegistry;

bool waterEnabled;
bool windEnabled;

//list of vertices used to draw blue rect representing water on-screen
std::vector<Sint16> waterVerticesX;
std::vector<Sint16> waterVerticesY;

//list of vertices used to draw gray rect representing wind on-screen
std::vector<Sint16> windVerticesX;
std::vector<Sint16> windVerticesY;

public:
    BlobDemo(); //constr
    ~BlobDemo() {}; //destr

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

    //call to spawn shapes upon button press
    void spawnShapes();
    
    //switches water on/off
    void switchWater();

    //switches wind on/off
    void switchWind();

};

#endif
