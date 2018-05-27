#ifndef _BlobDemo_
#define _BlobDemo_


#include "Demo.h"
#include "Random.h"
#include <math.h>
#include "Vector2D.h"
#include "Contacts.h"
#include "World.h"
#include "Platform.h"
#include "SDL2/SDL2_gfxPrimitives.h" 


#define BLOB_COUNT 5
#define PLATFORM_COUNT 5
#define BLOB_RADIUS 14.0f

/**
 * BlobDemo .
 */
class BlobDemo : public Demo
{
    ShapeBody *blobs;

    Platform *platforms;

    World world;

    BlobForceGenerator blobForceGenerator;

    /* The control for the x-axis. */
    //    float xAxis;

    /* The control for the y-axis. */
    //    float yAxis;


public:
    // Instantiates all objects in a new BlobDemo
    BlobDemo(); //constr
    virtual ~BlobDemo(); //destr

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
