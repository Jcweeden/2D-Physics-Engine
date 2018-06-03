#include "BlobDemo.h"

#include <vector>
#include <iostream>

BlobDemo::BlobDemo()
    :
    simulation(
        platformsCount + blobsCount,
        platformsCount),
    blobRadius(10.0f),
    blobsCount(5),
    platformsCount(1),
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
  //blobs = new ShapeBody[blobsCount];

  // Create the force generator
  /*
  blobForceGenerator.shapes = blobs;
  blobForceGenerator.shapesCount = blobsCount;  
  blobForceGenerator.maxAttraction = 20.0f;
  blobForceGenerator.maxReplusion = 10.0f;
  blobForceGenerator.minNaturalDistance = blobRadius*0.75f;
  blobForceGenerator.maxNaturalDistance = blobRadius*1.5f;
  blobForceGenerator.maxDistance = blobRadius * 2.5f;
  blobForceGenerator.maxFloat = 2;
  blobForceGenerator.floatHead = 8.0f;
  */
  
  //create the platforms
  platforms = new Platform[platformsCount];

  platforms[0].start = Vector2D(600, 300 );
  platforms[0].end = Vector2D(200, 450 );
  
  //set static variable that keeps track of number of active blobs in blobs array
  //** platforms->numShapesToCheckCollisionsWith = blobsCount;

  for (size_t i = 0; i < platformsCount; i++)
  {
    platforms[i].setRadius(blobRadius);
    
    platforms[i].setRestitution(0.35f);
    simulation.getContactGenerators().push_back(platforms + i);
  }


  //create blob and add to vector
  for (unsigned i = 0; i < 5; i++)
  {
    //std::cout << "creating blob " << i << " \n";

    int randXPos = rand() % 600 + 100;

    ShapeBody *blob = new ShapeBody();
    
    blob->setPosition(Vector2D(randXPos, -(rand() % 200 + 10)));
    
    blob->setVelocity(0,0);
    blob->setDamping(0.9f);
    blob->setAcceleration(0.0f, 0.0f); //no gravity
    blob->setMass(1.0f);
    blob->clearAccumForces();

    m_blobs.push_back(blob);
    
    ShapeBuoyancy* buoyancyForce = new ShapeBuoyancy(blobRadius, 10, TheGame::Instance()->getWindowHeight()*.66, 0.2);
    buoyancyRegistry.add(m_blobs[i], buoyancyForce);


    ShapeGravity* gravityForce = new ShapeGravity(Vector2D (0.0f,5.0f));
    gravityRegistry.add(m_blobs[m_blobs.size()-1], gravityForce);
    

    simulation.getShapes()->push_back(m_blobs[i]);
      
    //simulation.getForceRegistry().add(blobs + i, &blobForceGenerator);
  }
  //std::cout << "created blobs " << " \n";
  platforms[0].shapesToCheckContactsWithVector = m_blobs; //**

}

//removes all balls from the demo
void BlobDemo::reset()
{
  //for each ball
  for (ShapeBody *shape: m_blobs)
  {
  //remove associated gravityRegistry
  gravityRegistry.remove(shape);
  
  //remove associated buoyancyRegistry
  buoyancyRegistry.remove(shape);

  //delete from m_blobs
  delete shape;
  }

  //clear the vector
  m_blobs.clear();
  
  //remove all entries from simulation shapes
  simulation.getShapes()->clear();

  //reset number of shapes platform can collide with
  platforms->numShapesToCheckCollisionsWith = 0 /*m_blobs.size()*/;

  //clear list of objects the platform checks to collide with
  platforms[0].shapesToCheckContactsWithVector.clear();

  //reset the number of blobs in the scene to 0
  blobsCount = 0;

  //turn off the wind and water
  windEnabled = false;
  waterEnabled = false;
}

void BlobDemo::draw()
{
  //draw each platform
  for (size_t i = 0; i < platformsCount; i++)
  {
    aalineRGBA(TheGame::Instance()->getRenderer(), platforms[i].start.getX(), platforms[i].start.getY(), platforms[i].end.getX(), platforms[i].end.getY(), 0,0,0,255);
  }

  //draw each blob
  for (size_t i = 0; i < m_blobs.size(); i++)
  {    
    filledCircleRGBA(TheGame::Instance()->getRenderer(), m_blobs[i]->getPosition().m_x, m_blobs[i]->getPosition().m_y, blobRadius, 0,0,0,255);
  }

  //if water is enabled - draw it
  if (waterEnabled)
  {
      filledPolygonRGBA (TheGame::Instance()->getRenderer(), &waterVerticesX[0], &waterVerticesY[0], 4, 0, 0, 255, 51);
  }

  //if wind is enabled - draw it
  if (windEnabled)
  {
      filledPolygonRGBA (TheGame::Instance()->getRenderer(), &windVerticesX[0], &windVerticesY[0], 4, 88, 88, 88, 51);
  }
}

void BlobDemo::update()
{
  
    //remove accumu forces from previous frame for each obj
    simulation.clearAccumulatedForces();

    //get duration of the last frame
     float duration = TheGame::Instance()->getFrameTime() * 0.01f;
     if (duration <= 0.0f) return; 
             
    //apply gravity
    gravityRegistry.updateForces(TheGame::Instance()->getFrameTime());

    //for each obj
    for (size_t i = 0; i < m_blobs.size(); i++)
    {

      if (windEnabled)
      {
        //if in the windy section
        if (m_blobs[i]->getPositionY() > TheGame::Instance()->getWindowHeight()* .33 &&
            m_blobs[i]->getPositionY() < TheGame::Instance()->getWindowHeight()* .66)
        {
          m_blobs[i]->setAcceleration(5.0f,0.0f); //apply wind to blob
        }
        else {
          //else wind is on, but blob is not in windy section
          m_blobs[i]->setAcceleration(0.0f,0.0f);
        }
      }
      else
      {
        //wind is off for all blobs
        m_blobs[i]->setAcceleration(0.0f,0.0f);
      }

      if (waterEnabled)
      {
        buoyancyRegistry.updateForces(TheGame::Instance()->getFrameTime());
      }
      
      //if off the bottom or side of the screen, respawn at top of screen
      if (m_blobs[i]->getPositionY() > TheGame::Instance()->getWindowHeight()+30 ||
          m_blobs[i]->getPositionX() < -20 ||
          m_blobs[i]->getPositionX() > TheGame::Instance()->getWindowWidth()+30 )
      {
        /*
        m_blobs[i]->setPosition(Vector2D(rand() % 600 + 100, -(rand() % 100 + 30)));
        m_blobs[i]->setAcceleration(0.0f, 0.0f); //set acc to 0
        m_blobs[i]->setVelocity(0.0f, 0.0f); //reset any velocity the blob may have gained
        */
      }
    }

    //run simulation
    simulation.applyPhysics(duration);
}


void BlobDemo::clean()
{
  buoyancyRegistry.clear();
  gravityRegistry.clear();

  if (platforms) delete[] platforms;

  for (ShapeBody *shape: m_blobs)
  {
    delete shape;
  }
  m_blobs.clear();
}

void BlobDemo::handleInput()
{
  //MOUSE
  if (TheInputHandler::Instance()->getMouseButtonState(0))
  {
    spawnShapes();
  }


  //BUTTON PRESSES
  if (TheInputHandler::Instance()->keyBPressed)
  {
    TheInputHandler::Instance()->keyBPressed = false;
    switchWater();
  }
    if (TheInputHandler::Instance()->keyWPressed)
  {
    TheInputHandler::Instance()->keyWPressed = false;
    switchWind();
  }
}

void BlobDemo::spawnShapes()
{
  int numToSpawn = 1;

  
  //spawn on mouse button position
  TheInputHandler::Instance()->getMousePosition();

  //  simulation.getShapes().push_back(m_blobs[m_blobs.size()-1]);


  for (unsigned i = 0; i < numToSpawn; i++)
  {
    //std::cout << "creating blob " << i << " \n";

    int randXPos = rand() % 600 + 100;

    ShapeBody *blob = new ShapeBody();
    
    blob->setPosition(Vector2D(randXPos, 300));
    
    blob->setVelocity(0,0);
    blob->setDamping(0.9f);
    blob->setAcceleration(0.0f, 0.0f); //no gravity
    blob->setMass(1.0f);
    blob->clearAccumForces();

    m_blobs.push_back(blob);
    
    ShapeBuoyancy* buoyancyForce = new ShapeBuoyancy(blobRadius, 10, TheGame::Instance()->getWindowHeight()*.66, 0.2);
    buoyancyRegistry.add(m_blobs[m_blobs.size()-1], buoyancyForce);


    ShapeGravity* gravityForce = new ShapeGravity(Vector2D (0.0f,5.0f));
    gravityRegistry.add(m_blobs[m_blobs.size()-1], gravityForce);

    //add shape to coll detection
    simulation.getShapes()->push_back(m_blobs[m_blobs.size()-1]);
  }

  //update list of shapes platforms are looking for collisions with
  platforms[0].shapesToCheckContactsWithVector = m_blobs; //**
  
  //update counter of number of blobs in array
  //blobsCount += numToSpawn;
  
  //update number of shapes platform can collide with
  platforms->numShapesToCheckCollisionsWith = m_blobs.size();


}

void BlobDemo::switchWater()
{
  waterEnabled = !waterEnabled;
}

void BlobDemo::switchWind()
{
  windEnabled = !windEnabled;
}
