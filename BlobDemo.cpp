#include "BlobDemo.h"



// Method definitions
BlobDemo::BlobDemo()
    :
    world(
        platformsCount + blobsCount,
        platformsCount),
    blobRadius(14.0f),
    blobsCount(5),
    platformsCount(3),
    waterEnabled(false),
    windEnabled(false)
{

  Sint16 winWidth = TheGame::Instance()->getWindowWidth();
  Sint16 winHeight = TheGame::Instance()->getWindowHeight();
  
  waterVerticesX = {0, winWidth, winWidth, 0};
    
  waterVerticesY = {
    (short)(winHeight*.66),
    (short)(winHeight*.66),
    winHeight,
    winHeight};

  windVerticesX = {0, winWidth, winWidth, 0};
    
  windVerticesY = {
    (short)(winHeight*.33),
    (short)(winHeight*.33),
    (short)(winHeight*.66), 
    (short)(winHeight*.66)};
  
  srand (time(NULL)); //generate new randomised seed

  // Create the blob storage
  blobs = new ShapeBody[blobsCount];

  // Create the force generator
  blobForceGenerator.shapes = blobs;
  blobForceGenerator.shapesCount = blobsCount;  
  blobForceGenerator.maxAttraction = 20.0f;
  blobForceGenerator.maxReplusion = 10.0f;
  blobForceGenerator.minNaturalDistance = blobRadius*0.75f;
  blobForceGenerator.maxNaturalDistance = blobRadius*1.5f;
  blobForceGenerator.maxDistance = blobRadius * 2.5f;
  blobForceGenerator.maxFloat = 2;
  blobForceGenerator.floatHead = 8.0f;
    
  // Create the platforms
  platforms = new Platform[platformsCount];

  platforms[0].start = Vector2D(100, 100 );
  platforms[0].end = Vector2D(600, 200 );

  platforms[1].start = Vector2D(700, 300 );
  platforms[1].end = Vector2D(200, 400 );

  platforms[2].start = Vector2D(100, 500 );
  platforms[2].end = Vector2D(600, 600 );
  /*
  platforms[3].start = Vector2D(700, 700 );
  platforms[3].end = Vector2D(200, 800 );

  platforms[4].start = Vector2D(100, 900 );
  platforms[4].end = Vector2D(600, 1000 );
  */
  //create the blobs
  blobs = new ShapeBody[blobsCount];

  //set static variable that keeps track of number of active blobs in blobs array
  platforms->numShapesToCheckCollisionsWith = blobsCount;

  for (size_t i = 0; i < platformsCount; i++)
  {
    platforms[i].setRadius(14.0f);
    
    platforms[i].setRestitution(0.5f);
    platforms[i].shapesToCheckContactsWith = blobs;
    world.getContactGenerators().push_back(platforms + i);
  }
  
  for (unsigned i = 0; i < blobsCount; i++)
  {

    int randXPos = rand() % 600 + 100;
    blobs[i].setPosition(Vector2D(randXPos, -(rand() % 200 + 10)));
    
    blobs[i].setVelocity(0,0);
    blobs[i].setDamping(0.9f);
    blobs[i].setAcceleration(0.0f, 0.0f); //no gravity
    blobs[i].setMass(1.0f);
    blobs[i].clearAccumForces();

    ShapeBuoyancy* buoyancyForce = new ShapeBuoyancy(blobRadius, 10, TheGame::Instance()->getWindowHeight()*.66, 0.2);
    buoyancyRegistry.add(&blobs[i], buoyancyForce);


    ShapeGravity* gravityForce = new ShapeGravity(Vector2D (0.0f,5.0f));
    gravityRegistry.add(&blobs[i], gravityForce);
    
    world.getShapes().push_back(blobs + i);
    world.getForceRegistry().add(blobs + i, &blobForceGenerator);
  }
}

//restarts the location of each of the shapes
void BlobDemo::reset()
{
  for (unsigned i = 0; i < blobsCount; i++)
  {
    int randXPos = rand() % 600 + 100;
    blobs[i].setPosition(Vector2D(randXPos,  -(rand() % 100 + 30)));
    blobs[i].setVelocity(0,0);
    blobs[i].clearAccumForces();
  }

  waterEnabled = false;
}

BlobDemo::~BlobDemo()
{}

void BlobDemo::draw()
{
  //draw each platform
  for (size_t i = 0; i < platformsCount; i++)
  {
    aalineRGBA(TheGame::Instance()->getRenderer(), platforms[i].start.getX(), platforms[i].start.getY(), platforms[i].end.getX(), platforms[i].end.getY(), 0,0,0,255);
  }

  //draw each blob
  for (size_t i = 0; i < blobsCount; i++)
  {    
    filledCircleRGBA(TheGame::Instance()->getRenderer(), blobs[i].getPosition().m_x, blobs[i].getPosition().m_y, blobRadius, 0,0,0,255);
  }

  //if water enabled - draw it
  if (waterEnabled)
  {
      filledPolygonRGBA (TheGame::Instance()->getRenderer(), &waterVerticesX[0], &waterVerticesY[0], 4, 0, 0, 255, 51);
  }

  //if wind enabled  - draw it
  if (windEnabled)
  {
      filledPolygonRGBA (TheGame::Instance()->getRenderer(), &windVerticesX[0], &windVerticesY[0], 4, 88, 88, 88, 51);
  }
}

void BlobDemo::update()
{
    //get duration of the last frame
     float duration = TheGame::Instance()->getFrameTime() * 0.01f;
     if (duration <= 0.0f) return; 
  
    //remove accumu forces from previous frame for each obj
    world.startFrame();

    //apply gravity
    gravityRegistry.updateForces(TheGame::Instance()->getFrameTime());
    
    //for each obj
    for (size_t i = 0; i < blobsCount; i++)
    {
      if (windEnabled)
      {
        //if in the windy section
        if (blobs[i].getPositionY() > TheGame::Instance()->getWindowHeight()* .33 &&
            blobs[i].getPositionY() < TheGame::Instance()->getWindowHeight()* .66)
        {
          blobs[i].setAcceleration(-5.0f,0.0f); //apply wind to blob
        }
        else {
          //else wind is on, but blob is not in windy section
          blobs[i].setAcceleration(0.0f,0.0f);
        }
      }
      else
      {
        //wind is off for all blobs
        blobs[i].setAcceleration(0.0f,0.0f);
      }

      if (waterEnabled)
      {
        buoyancyRegistry.updateForces(TheGame::Instance()->getFrameTime());
      }
      
      //if off the bottom or side of the screen, respawn at top of screen
      if (blobs[i].getPositionY() > TheGame::Instance()->getWindowHeight()+30 ||
        blobs[i].getPositionX() < -20)
      {
        blobs[i].setPosition(Vector2D(rand() % 600 + 100, -(rand() % 100 + 30)));
        blobs[i].setAcceleration(0.0f, 0.0f); //set acc to 0
        blobs[i].setVelocity(0.0f, 0.0f); //reset any velocity the blob may have gained
      }
    }
    
    //run simulation
    world.runPhysics(duration);
}


void BlobDemo::clean()
{
  delete[] platforms;
  delete[] blobs;
}

void BlobDemo::handleInput()
{
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_B))
  {
    switchWater();
  }
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
  {
    switchWind();
  }
}

void BlobDemo::switchWater()
{
  waterEnabled = !waterEnabled;
}

void BlobDemo::switchWind()
{
  windEnabled = !windEnabled;
}
