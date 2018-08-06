#include "AnchorSpringDemo.h"


AnchorSpringDemo::AnchorSpringDemo()
{  
  const Vector2D grav (0.0f,3.5f);

  //LIGHT-WEIGHT
  Circle* lightCircle = new Circle(100,400, 12, 1, 255,255,255,255);
  lightCircle->setMass(3.0f);
  lightCircle->setVelocity(0.0f, 0.0f);
  lightCircle->setAcceleration(0.0f, 0.0f); //grav set in forceGen
  lightCircle->setDamping(0.85f);
  
  springs.push_back(lightCircle);

  ShapeGravity* gravLightWeight = new ShapeGravity(grav);
  registry.add(lightCircle, gravLightWeight);

  ShapeAnchoredSpring* anchoredSpringLightWeight = new ShapeAnchoredSpring(new Vector2D(100,100), 0.5f, 100.0f);
  registry.add(lightCircle, anchoredSpringLightWeight);
  
  //Mid-Weight
  Circle* midWeightCircle = new Circle(300,400, 24, 1, 128,128,128,255);
  midWeightCircle->setMass(6.0f);
  midWeightCircle->setVelocity(0.0f, 0.0f);
  midWeightCircle->setAcceleration(0.0f, 0.0f); //grav set in forceGen
  midWeightCircle->setDamping(0.90f);
  
  springs.push_back(midWeightCircle);

  ShapeGravity* gravMidWeight = new ShapeGravity(grav);
  registry.add(midWeightCircle, gravMidWeight);

  ShapeAnchoredSpring* anchoredSpringMidWeight = new ShapeAnchoredSpring(new Vector2D(300,100), 0.5f, 100.0f);
  registry.add(midWeightCircle, anchoredSpringMidWeight);
  
  //HEAVY-WEIGHT  
  Circle* heavyCircle = new Circle(500,400, 40, 1, 0,0,0,255);
  heavyCircle->setMass(10.0f);
  heavyCircle->setVelocity(0.0f, 0.0f);
  heavyCircle->setAcceleration(0.0f, 0.0f);
  heavyCircle->setDamping(0.95f);
  
  springs.push_back(heavyCircle);

  ShapeGravity* gravHeavyWeight = new ShapeGravity(grav);
  registry.add(heavyCircle, gravHeavyWeight);

  ShapeAnchoredSpring* anchoredSpringHeavyWeight = new ShapeAnchoredSpring(new Vector2D(500,100), 0.5f, 100.0f);
  registry.add(heavyCircle, anchoredSpringHeavyWeight);
}


void AnchorSpringDemo::update()
{
  for (size_t i = 0; i < springs.size(); i++)
  {
    springs[i]->update();
  }

  registry.updateForces(TheGame::Instance()->getFrameTime());

}
  
void AnchorSpringDemo::draw()
{

 //draw line from spring's anchored position to the obj itself
  //light-weight line
  aalineRGBA(TheGame::Instance()->getRenderer(), 100,100, springs[0]->getPositionX(), springs[0]->getPositionY(), 192,192,192,255);

  //mid-weight line
  aalineRGBA(TheGame::Instance()->getRenderer(), 300,100, springs[1]->getPositionX(), springs[1]->getPositionY(), 192,192,192,255);

  //heavy line
  aalineRGBA(TheGame::Instance()->getRenderer(), 500,100, springs[2]->getPositionX(), springs[2]->getPositionY(), 192,192,192,255);
    
  //loop through objects and draw them
  for (size_t i = 0; i < springs.size(); i++)
  {
    springs[i]->draw();
  }

   //draw light spring outline
  circleRGBA(TheGame::Instance()->getRenderer(), springs[0]->getPositionX(), springs[0]->getPositionY(), 12, 0,0,0,255);
  
  circleRGBA(TheGame::Instance()->getRenderer(), springs[1]->getPositionX(), springs[1]->getPositionY(), 24, 0,0,0,255);
}

void AnchorSpringDemo::clean()
{
  //delete each obj in vector
  for (GameObject* obj : springs)
    delete obj;

  //clear vector
  springs.clear();

  //clear registry
  registry.clear();
}

/*
AnchorSpringDemo::~AnchorSpringDemo()
{}
*/

void AnchorSpringDemo::reset()
{
  //remove all accumulated forces
  for (size_t i = 0; i < springs.size(); i++)
  {
    springs[i]->setVelocity(0.0f, 0.0f);
    springs[i]->setAcceleration(0.0f, 0.0f);
  }

  //reset to starting positions
  springs[0]->setPosition(Vector2D(200,400));
  springs[1]->setPosition(Vector2D(400,400));
  springs[2]->setPosition(Vector2D(600,400));
    
}

void AnchorSpringDemo::handleInput()
{

}
