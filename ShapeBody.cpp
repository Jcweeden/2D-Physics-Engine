#include "ShapeBody.h"
#include <math.h>

ShapeBody::ShapeBody(int p_x, int p_y,/* int p_rotation,*/ int p_mass, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA) :
    GameObject(), position(p_x, p_y), velocity(0,0), acceleration(0,0), forceAccumulated(0,0), colourR(p_colourR), colourG(p_colourG), colourB(p_colourB), colourA(p_colourA)
{
  arePhysicsEnabled = true;
  isHeldByMouse = false;
  //mouseReleased = true;
  damping = 0.0f;
  rotation = 0;
  setMass(p_mass);
}

ShapeBody::ShapeBody() :
    GameObject(), position(0, 0), velocity(0,0), acceleration(0,0), forceAccumulated(0,0), colourR(0), colourG(0), colourB(0), colourA(0)
{
  arePhysicsEnabled = true;
  isHeldByMouse = false;
  //mouseReleased = true;
  damping = 0.0f;
  rotation = 0;
  setMass(1.0f);
}


void ShapeBody::setMass(const float mass)
{
  if(mass == 0)
  {
    std::cout << "ShapeBody.cpp.setMass() - requires mass not of 0\n";
    return;
  } else
  {
    inverseMass = ((float)1.0f)/mass;
  }
}

void ShapeBody::draw() {}


void ShapeBody::update(){
  //APPLY PHYSICS
  if (arePhysicsEnabled) {  //if physics are enabled integrate
    physicsIntegration();
  }
}

//calculate and apply physics forces to the object should it have mass
void ShapeBody::physicsIntegration()
{
  //ensure has mass
  if (inverseMass <= 0.0f)
  {
    std::cout << "ShapeBody.cpp.Integration() - inverseMass below 0\n";
    return;
  }
    
  float duration = TheGame::Instance()->getFrameTime()* 0.01f;
  if (duration <= 0.0)
  {
    //std::cout << "ShapeBody.cpp.Integration() - duration below 0\n";
    return;
  }

  
  //update position
  position.addScaledVector(velocity, duration);

  //calc acceleration
  Vector2D resultingAcceleration = acceleration;
  resultingAcceleration.addScaledVector(forceAccumulated, inverseMass);

  //update velocity from acceleration
  velocity.addScaledVector(resultingAcceleration, duration);

  //add drag
  velocity *= powf(damping, duration);
  
  clearAccumForces();

  //std::cout << "Velocity x: " << velocity.getX() << " y: " << velocity.getY() << "\n";
  //std::cout << "Acceleration x: " << acceleration.getX() << " y: " << acceleration.getY() << "\n\n";
  
}


//adds force to the vector that will be applied at the end of each frame
void ShapeBody::addForce(const Vector2D &force)
{
  forceAccumulated += force;
}

void ShapeBody::clearAccumForces()
{
  forceAccumulated.clear();
}

void ShapeBody::clean(){};


