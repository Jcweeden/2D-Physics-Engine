#include "ForceGenerator.h"
#include <cmath>

void ForceRegistry::updateForces(float duration)
{
  //for each force in the registry, apply it
  for (size_t i = 0; i < registrations.size(); i++)
  {
    //registrations[i].fg->printForceGenType();
    registrations[i].fg->updateForce(registrations[i].shape, duration);
  }
}

void ForceRegistry::add(ShapeBody* shape, ForceGenerator *fg)
{
  //create a new struct that holds a shape and the forceGenerator that applies to it
  ForceRegistry::ShapeForceRegistration registration;
  registration.shape = shape;
  registration.fg = fg;

  //and save this in the vector holding all applied forces
  registrations.push_back(registration);
}

//removes pair of shape and forceGen from registry should they exist
void ForceRegistry::remove(ShapeBody* shape, ForceGenerator *fg)
{
  for (size_t i = 0; i < registrations.size(); i++)
  {
    if (registrations[i].shape == shape)
    {
      if (registrations[i].fg == fg)
      {
        registrations.erase(registrations.begin()+i);
        std::cout << "ForceRegistry::remove() - Erased\n";
        return;
      }
    }
  } 
}

//removes all registrations 
void ForceRegistry::clear()
{
  registrations.clear();
}

///// GRAVITY

ShapeGravity::ShapeGravity(const Vector2D &gravity) :
    gravity(gravity)
{
}


void ShapeGravity::updateForce(ShapeBody* shape, float duration)
{
  //ensure the obj has mass
  if (!shape->hasFiniteMass() ) { return; }
  
  Vector2D grav = gravity * shape->getMass(); //dubious
  shape->addForce(grav);
   
}

////// DRAG

void ShapeDrag::updateForce(ShapeBody* shape, float duration)
{
  Vector2D force = shape->getVelocity();

  //calc drag coefficient
  float dragCoefficient = force.magnitude();
  dragCoefficient = k1 * dragCoefficient + k2 * dragCoefficient * dragCoefficient;

  force.normalise();
  force *= -dragCoefficient;
  shape->addForce(force);
}

//SPRING

ShapeSpring::ShapeSpring(ShapeBody* p_endOfSpringObj, float p_springConstant, float p_restLength) :
    endOfSpringObj(p_endOfSpringObj), springConstant(p_springConstant), restLength(p_restLength)
{}

void ShapeSpring::updateForce(ShapeBody* shape, float duration)
{
  Vector2D force = shape->getPosition();
  force -= endOfSpringObj->getPosition();
  
  //calc magnitude of force
  float magnitude = force.magnitude();
  magnitude = /*std::abs*/(magnitude-restLength);
  magnitude *= springConstant;
  
  //calc and apply

  force.normalise();
  force *= -magnitude;
  shape->addForce(force);
}

//ANCHOERED SPRING

ShapeAnchoredSpring::ShapeAnchoredSpring(Vector2D* p_anchor, float p_springConstant, float p_restLength) :
    anchor(p_anchor), springConstant(p_springConstant), restLength(p_restLength)
{}

void ShapeAnchoredSpring::updateForce(ShapeBody* shape, float duration)
{
  //std::cout << "shpae x: " << shape->getPosition().getX() << "y: " << shape->getPosition().getY() << "\n";
  Vector2D force = shape->getPosition();
  force -= *anchor;
  
  //calc magnitude of force
  float magnitude = force.magnitude();
  magnitude = /*std::abs*/(magnitude-restLength);
  magnitude *= springConstant;
  
  //calc and apply

  force.normalise();
  force *= -magnitude;
  //std::cout << "x: " << force.getX() << "y: " << force.getY() << "\n";
  shape->addForce(force);
}


//// BUNGEE

ShapeBungee::ShapeBungee(ShapeBody* p_endOfBungeeObj, float p_springConstant, float p_restLength) :
    endOfBungeeObj(p_endOfBungeeObj), springConstant(p_springConstant), restLength(p_restLength)
{}

void ShapeBungee::updateForce(ShapeBody* shape, float duration)
{
  //std::cout << "shpae x: " << shape->getPosition().getX() << "y: " << shape->getPosition().getY() << "\n";
  Vector2D force = shape->getPosition();
  force -= endOfBungeeObj->getPosition(); //calc distance between two ends of bungee

  //check if bungee is compressed
  float magnitude = force.magnitude();
  if (magnitude <= restLength) return; // compressed (not extended beyond rest length) - do not apply force
  
  //calc magnitude of force
  magnitude = springConstant * (restLength - magnitude);
  
  //calc and apply
  force.normalise();
  force *= -magnitude;
  //std::cout << "x: " << force.getX() << "y: " << force.getY() << "\n";
  shape->addForce(force);
}

//BUOYANCY

ShapeBuoyancy::ShapeBuoyancy(float p_depthForMaxBuoyancyForce, float p_shapeVolume, float p_waterHeight, float p_waterDensity) :
    depthForMaxBuoyancyForce(p_depthForMaxBuoyancyForce), shapeVolume(p_shapeVolume), waterHeight(p_waterHeight), waterDensity(p_waterDensity)
{}

void ShapeBuoyancy::updateForce(ShapeBody* shape, float duration)
{
  float shapeDepth = shape->getPositionY();
  // std::cout << "shapeDepth: " << shapeDepth << "waterHeight: " << waterHeight << "\n";

  //if out of the water return (- maxBuoy as coords are at top of obj, so when at depth would be fully submerged)
  if (shapeDepth <= waterHeight - depthForMaxBuoyancyForce)
  {
    //std::cout << "out of water\n";
    return;
  }
  
  Vector2D force(0,0);

  //FULLY SUBMERGED
  if (shapeDepth >= waterHeight)
  {
    //std::cout << "Fully submerged\n";
    force.setY(-(waterDensity * shapeVolume)); //set the force to it's max (full volume of shape * water density)
    shape->addForce(force);
    return;
  }

  //PARTIALLY SUBMERGED

  float depthInWater = (shapeDepth + depthForMaxBuoyancyForce) - waterHeight; //y value of object in the water
  float percentageInWater = depthInWater / depthForMaxBuoyancyForce; //percentage of object in the water
  float volumeInWater = percentageInWater * shapeVolume; //value of volume that is in water
  float forceY = volumeInWater * waterDensity;
    
  force.setY(-forceY);
}
