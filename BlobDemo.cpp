#include "BlobDemo.h"



// Method definitions
BlobDemo::BlobDemo()
    :
    //xAxis(0), yAxis(0),
    world(PLATFORM_COUNT+BLOB_COUNT, PLATFORM_COUNT)
{

  srand (time(NULL)); //randomise the seed.

  // Create the blob storage
  blobs = new ShapeBody[BLOB_COUNT];

  // Create the force generator
  blobForceGenerator.shapes = blobs;
  blobForceGenerator.shapesCount = BLOB_COUNT;  
  blobForceGenerator.maxAttraction = 20.0f;
  blobForceGenerator.maxReplusion = 10.0f;
  blobForceGenerator.minNaturalDistance = BLOB_RADIUS*0.75f;
  blobForceGenerator.maxNaturalDistance = BLOB_RADIUS*1.5f;
  blobForceGenerator.maxDistance = BLOB_RADIUS * 2.5f;
  blobForceGenerator.maxFloat = 2;
  blobForceGenerator.floatHead = 8.0f;
    
  // Create the platforms
  platforms = new Platform[PLATFORM_COUNT];

  platforms[0].start = Vector2D(100, 100 );
  platforms[0].end = Vector2D(600, 200 );

  platforms[1].start = Vector2D(700, 300 );
  platforms[1].end = Vector2D(200, 400 );

  platforms[2].start = Vector2D(100, 500 );
  platforms[2].end = Vector2D(600, 600 );

  platforms[3].start = Vector2D(700, 700 );
  platforms[3].end = Vector2D(200, 800 );

  platforms[4].start = Vector2D(100, 900 );
  platforms[4].end = Vector2D(600, 1000 );
    
  // Create the blobs.
  blobs = new ShapeBody[BLOB_COUNT];

  //set static variable that keeps track of number of active blobs in blobs array
  platforms->numShapesToCheckCollisionsWith = BLOB_COUNT;

  for (size_t i = 0; i < PLATFORM_COUNT; i++)
  {
    platforms[i].setRadius(14.0f);
    
    platforms[i].setRestitution(0.5f);
    platforms[i].shapesToCheckContactsWith = blobs;
    world.getContactGenerators().push_back(platforms + i);
  }
  
  for (unsigned i = 0; i < BLOB_COUNT; i++)
  {

    int randXPos = rand() % 600 + 100;
    blobs[i].setPosition(Vector2D(randXPos, -10));
    
    blobs[i].setVelocity(0,0);
    blobs[i].setDamping(0.9f);
    blobs[i].setAcceleration(0.0f, 5.0f); //gravity
    blobs[i].setMass(1.0f);
    blobs[i].clearAccumForces();

    world.getShapes().push_back(blobs + i);
    world.getForceRegistry().add(blobs + i, &blobForceGenerator);
  }
}

//restarts the location of each of the shapes
void BlobDemo::reset()
{
  for (unsigned i = 0; i < BLOB_COUNT; i++)
  {
    int randXPos = rand() % 600 + 100;
    blobs[i].setPosition(Vector2D(randXPos,  -(rand() % 100 + 30)));
    blobs[i].setVelocity(0,0);
    blobs[i].clearAccumForces();
  }
}

BlobDemo::~BlobDemo()
{}

void BlobDemo::draw()
{

  for (size_t i = 0; i < PLATFORM_COUNT; i++)
  {
    aalineRGBA(TheGame::Instance()->getRenderer(), platforms[i].start.getX(), platforms[i].start.getY(), platforms[i].end.getX(), platforms[i].end.getY(), 0,0,0,255);
  }

  for (size_t i = 0; i < BLOB_COUNT; i++)
  {
    //if water enabled
    //if below water level draw in this colour,
    
    filledCircleRGBA(TheGame::Instance()->getRenderer(), blobs[i].getPosition().m_x, blobs[i].getPosition().m_y, BLOB_RADIUS, 0,0,0,255);
  }
}

void BlobDemo::update()
{
    //remove  accumu forces from prev frame for each obj
    world.startFrame();

    for (size_t i = 0; i < BLOB_COUNT; i++)
    {
      if (blobs[i].getPositionY() > TheGame::Instance()->getWindowHeight()+30)
      {
        blobs[i].setPosition(Vector2D(rand() % 600 + 100, -(rand() % 100 + 30)));
      }
    }
    
    // Find the duration of the last frame in seconds
     float duration = TheGame::Instance()->getFrameTime() * 0.01f;
     if (duration <= 0.0f) return; 

    // Recenter the axes
     // xAxis *= pow(0.1f, duration); ///****
     //yAxis *= pow(0.1f, duration); ///****

    // Move the controlled blob
     //blobs[0].addForce(Vector2D(xAxis, yAxis)*10.0f); //****

    // Run the simulation
    world.runPhysics(duration); //****


    //std::cout << "End of frame\n\n";
    // Bring all the particles back to 2d
    /* Vector2D position;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        blobs[i].getPosition(&position);
        position.z = 0.0f;
        blobs[i].setPosition(position);
    }*/

}


void BlobDemo::clean()
{
  delete[] platforms;
  delete[] blobs;
}


/*
void BlobDemo::key(unsigned char key)
{
    switch(key)
    {
    case 'w': case 'W':
        yAxis = 1.0;
        break;
    case 's': case 'S':
        yAxis = -1.0;
        break;
    case 'a': case 'A':
        xAxis = -1.0f;
        break;
    case 'd': case 'D':
        xAxis = 1.0f;
        break;
    case 'r': case 'R':
        reset();
        break;
    }
}
*/
