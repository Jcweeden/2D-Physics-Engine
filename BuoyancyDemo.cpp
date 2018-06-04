#include "BuoyancyDemo.h"

#include <vector>
#include <iostream>

BuoyancyDemo::BuoyancyDemo()
    :
    shapeRadius(6.0f),
    platformsCount(1),
    simulation(platformsCount + 15),
    waterEnabled(true),
    windEnabled(false)
{
  srand (time(NULL)); //generate new randomised seed
  
  //calculate coords of screen to draw water and wind on-screen
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

  platforms[0].start = Vector2D((winWidth/5), (winHeight/5) * 4 );
  platforms[0].end = Vector2D((winWidth/5)*4, winHeight/2 );
  

  for (size_t i = 0; i < platformsCount; i++)
  {
    platforms[i].setRadius(shapeRadius);
    
    platforms[i].setRestitution(0.35f);
    simulation.getContactGenerators().push_back(platforms + i);
  }

  //create shape and add to vector
  /*
  for (unsigned i = 0; i < 0; i++)
  {
    int randXPos = rand() % 600 + 100;

    ShapeBody *shape = new ShapeBody();
    
    shape->setPosition(Vector2D(randXPos, -(rand() % 200 + 10)));
    
    shape->setVelocity(0,0);
    shape->setDamping(0.9f);
    shape->setAcceleration(0.0f, 0.0f); //no gravity
    shape->setMass(1.0f);
    shape->clearAccumForces();

    m_shapes.push_back(shape);
    
    ShapeBuoyancy* buoyancyForce = new ShapeBuoyancy(shapeRadius, 10, TheGame::Instance()->getWindowHeight()*.66, 1.2f);
    buoyancyRegistry.add(m_shapes[i], buoyancyForce);

    ShapeGravity* gravityForce = new ShapeGravity(Vector2D (0.0f,5.0f));
    gravityRegistry.add(m_shapes[m_shapes.size()-1], gravityForce);
  }
  simulation.setShapes(m_shapes);
  */

  //set static variable that keeps track of number of active shapes in shapes array
  platforms->numShapesToCheckCollisionsWith = m_shapes.size();
  platforms[0].shapesToCheckContactsWithVector = m_shapes;
}


void BuoyancyDemo::clean()
{
  //for each shape
  for (ShapeBody *shape: m_shapes)
  {
    //remove associated gravityRegistry
    gravityRegistry.remove(shape);

    //remove associated buoyancyRegistry
    buoyancyRegistry.remove(shape);

    //delete from m_shapes
    delete shape;
  }
  
  //clear the vector
  m_shapes.clear();
  //remove all entries from simulation shapes
  simulation.getShapes().clear();

  //delete platforms
  platforms[0].shapesToCheckContactsWithVector.clear();
  if (platforms) delete[] platforms;
}


//removes all shapes from the demo
void BuoyancyDemo::reset()
{
  //for each shape
  for (ShapeBody *shape: m_shapes)
  {
    //remove associated gravityRegistry
    gravityRegistry.remove(shape);
  
    //remove associated buoyancyRegistry
    buoyancyRegistry.remove(shape);

    //delete from m_shapes
    delete shape;
  }

  //clear the vector
  m_shapes.clear();
  
  //reset simulation shapes to m_shapes (empty)
  simulation.setShapes(m_shapes);

  //reset number of shapes platform can collide with
  platforms->numShapesToCheckCollisionsWith = 0 /*m_shapes.size()*/;

  //reset list of objects the platform checks to collide with to m_shapes (empty)
  platforms[0].shapesToCheckContactsWithVector = m_shapes;

  //reset the wind and water
  windEnabled = false;
  waterEnabled = true;
}

void BuoyancyDemo::draw()
{
  //draw each platform, double thickness
  for (size_t i = 0; i < platformsCount; i++)
  {
    aalineRGBA(TheGame::Instance()->getRenderer(), platforms[i].start.getX(), platforms[i].start.getY(), platforms[i].end.getX(), platforms[i].end.getY(), 0,0,0,255);
    aalineRGBA(TheGame::Instance()->getRenderer(), platforms[i].start.getX(), platforms[i].start.getY()-1, platforms[i].end.getX(), platforms[i].end.getY()-1, 0,0,0,255);
  }

  //draw each shape, with an outline
  for (size_t i = 0; i < m_shapes.size(); i++)
  {
    filledCircleRGBA(TheGame::Instance()->getRenderer(), m_shapes[i]->getPosition().m_x, m_shapes[i]->getPosition().m_y, shapeRadius+1, 0,0,0,255);
    filledCircleRGBA(TheGame::Instance()->getRenderer(), m_shapes[i]->getPosition().m_x, m_shapes[i]->getPosition().m_y, shapeRadius, m_shapes[i]->colourR,m_shapes[i]->colourG,m_shapes[i]->colourB,255);
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

void BuoyancyDemo::update()
{  
  //remove accumu forces from previous frame for each shape
  simulation.clearAccumulatedForces();

  //get duration of the last frame
  float duration = TheGame::Instance()->getFrameTime() * 0.01f;
  if (duration <= 0.0f) return; 
             
  //apply gravity
  gravityRegistry.updateForces(TheGame::Instance()->getFrameTime());

  //if enabled, apply buoyancy forces
  if (waterEnabled) buoyancyRegistry.updateForces(TheGame::Instance()->getFrameTime());
  
  //new vector that holds the vector indexs of shapes found to be deleted in for loop
  std::vector<int> toEraseIndex {};
  
  //for each shape, check if in wind, or if off the screen
  for (size_t i = 0; i < m_shapes.size(); i++)
  {
    
    if (windEnabled)
    {
      //if in the windy section
      if (m_shapes[i]->getPositionY() > TheGame::Instance()->getWindowHeight()* .33 &&
          m_shapes[i]->getPositionY() < TheGame::Instance()->getWindowHeight()* .66)
      {
        m_shapes[i]->setAcceleration((10.0f/m_shapes[i]->getMass()),0.0f); //apply wind to shape
      }
      else {
        //else wind is on, but shape is not in windy section
        m_shapes[i]->setAcceleration(0.0f,0.0f);
      }
    }
    else
    {
      //wind is off for all shapes
      m_shapes[i]->setAcceleration(0.0f,0.0f);
    }
      
    //check if on screen - if off the bottom or side of the screen, then remove the shape
    if (m_shapes[i]->getPositionY() > TheGame::Instance()->getWindowHeight()+30 ||
        m_shapes[i]->getPositionX() < -50 ||
        m_shapes[i]->getPositionX() > TheGame::Instance()->getWindowWidth()+30 )
    {
      //remove associated gravityRegistry
      gravityRegistry.remove(m_shapes[i]);
  
      //remove associated buoyancyRegistry
      buoyancyRegistry.remove(m_shapes[i]);
   
      //add index to be deleted from m_shapes
      toEraseIndex.push_back(i);
    }
  }

  //if any shapes are to be deleted
  if (toEraseIndex.size() > 0)
  {
    //erase any shapes specified to be deleted from m_shapes
    for (size_t i = toEraseIndex.size(); i > 0; i--)
    {
      delete m_shapes[toEraseIndex[i-1]];
      m_shapes.erase (m_shapes.begin() + toEraseIndex[i-1]);     
    }
    
    //reset list of shapes the platform checks to collide with
    platforms[0].shapesToCheckContactsWithVector = m_shapes;
  
    //reset all entries from simulation shapes
    simulation.setShapes(m_shapes);

    //reset number of shapes platform can collide with
    platforms->numShapesToCheckCollisionsWith = m_shapes.size();
  }
  
  //run simulation, applying any accumulated forces
  simulation.applyPhysics(duration);
}


void BuoyancyDemo::handleInput()
{
  //MOUSE

  //spawn white coloured lighter shapes from left-click
  if (TheInputHandler::Instance()->getMouseButtonState(0))
  {
    spawnShapes(1.0f, 255);
  }
  //spawn black coloured heavier shapes from right-click
  if (TheInputHandler::Instance()->getMouseButtonState(2))
  {
    spawnShapes(3.0f, 0);
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

void BuoyancyDemo::spawnShapes(float mass, int colour)
{
  //number of shapes spawned per button press
  int numToSpawn = 5;

  //spawn on mouse button position
  for (unsigned i = 0; i < numToSpawn; i++)
  {
    ShapeBody *shape = new ShapeBody();

    //place shape where mouse is
    shape->setPosition(Vector2D( TheInputHandler::Instance()->getMousePosition()->getX(), TheInputHandler::Instance()->getMousePosition()->getY()));

    //generate random velocity for each shape upon spawn 
    float randXVelocity = rand() % 60 - 30;
    float randYVelocity = rand() % 120 - 60;
    shape->setVelocity( randXVelocity,randYVelocity);

    shape->setDamping(0.9f);
    shape->setAcceleration(0.0f, 0.0f); //no gravity
    shape->setMass(mass);
    shape->setColour(colour,colour,colour,255);
    shape->clearAccumForces();

    m_shapes.push_back(shape);
    
    ShapeBuoyancy* buoyancyForce = new ShapeBuoyancy(shapeRadius, 10, TheGame::Instance()->getWindowHeight()*.66, 0.7f);
    buoyancyRegistry.add(m_shapes[m_shapes.size()-1], buoyancyForce);
    
    ShapeGravity* gravityForce = new ShapeGravity(Vector2D (0.0f,5.0f));
    gravityRegistry.add(m_shapes[m_shapes.size()-1], gravityForce);
  }

  //add new shapes to coll detection
  simulation.setShapes(m_shapes);
  
  //update list of shapes platforms are checking for collisions with
  platforms[0].shapesToCheckContactsWithVector = m_shapes;
  
  //update number of shapes platform can collide with simultaneously
  platforms->numShapesToCheckCollisionsWith = m_shapes.size();
}

void BuoyancyDemo::switchWater()
{
  waterEnabled = !waterEnabled;
}

void BuoyancyDemo::switchWind()
{
  windEnabled = !windEnabled;
}
