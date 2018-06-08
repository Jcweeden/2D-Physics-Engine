#include "ForceGenerator.h"
#include <cmath>

void ForceRegistry::updateForces(float duration)
{
  //for each force in the registry, apply it
  for (size_t i = 0; i < registrations.size(); i++)
  {
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
        //std::cout << "ForceRegistry::remove() - Erased\n";
        return;
      }
    }
  } 
}

//removes pair of shape and forceGen from registry should they exist, given just the shape to search with
void ForceRegistry::remove(ShapeBody* shape)
{
  for (size_t i = 0; i < registrations.size(); i++)
  {
    if (registrations[i].shape == shape)
    {
      registrations.erase(registrations.begin()+i);
      //std::cout << "ForceRegistry::remove() - Erased\n";
      return;
    }
  } 
}

//removes pair of shape and forceGen from registry should they exist, given just the fg to search with
void ForceRegistry::remove(ForceGenerator *fg)
{
  for (size_t i = 0; i < registrations.size(); i++)
  {
    if (registrations[i].fg == fg)
    {
      registrations.erase(registrations.begin()+i);
      //std::cout << "ForceRegistry::remove() - Erased\n";
      return;
    }
  } 
}

//removes all registrations 
void ForceRegistry::clear()
{
  
  if (registrations.size() > 0) {
    for (ShapeForceRegistration reg : registrations)
    {
      //delete reg.shape;
      //delete reg.fg;
    }
  }
  
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

ShapeSpring::ShapeSpring(ShapeBody* p_endOfSpringObj, float p_springConstant, float p_restLength, float p_snapLength) :
    endOfSpringObj(p_endOfSpringObj), springConstant(p_springConstant), restLength(p_restLength), snapLength(p_snapLength), hasSnapped(false)
{}

void ShapeSpring::updateForce(ShapeBody* shape, float duration)
{
  //calc distance between two ends of bungee
  Vector2D force = shape->getPosition() - endOfSpringObj->getPosition();
  //calc magnitude of force
  float length = force.magnitude();


  //if the bungee is compressed, and not long enough to compress 
  if (length <= restLength)
  {
    return; // compressed (not extended beyond rest length) - do not apply force
  }

  
  //else if long enough to compress, but not long enough to snap
  else if (length < snapLength)
  {
    //get the length extended past the cable's restLength
    length = length - restLength;

    //multiple by the stiffness of the spring
    length *= springConstant;
  
    //calc and apply
    force.normalise();
    force *= -length;
    shape-> addForce(force);
  }

  
  //else the bungee has exceeded its max length, and has snapped
  else
  {
    setSnapped(true);
  }
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

ShapeBungee::ShapeBungee(ShapeBody* p_endOfBungeeObj, float p_springConstant, float p_restLength, float p_snapLength)
    : endOfBungeeObj(p_endOfBungeeObj), springConstant(p_springConstant), restLength(p_restLength), snapLength(p_snapLength), hasSnapped(false)
{}

void ShapeBungee::updateForce(ShapeBody* shape, float duration)
{
  //std::cout << "shpae x: " << shape->getPosition().getX() << "y: " << shape->getPosition().getY() << "\n";
  Vector2D force = shape->getPosition() - endOfBungeeObj->getPosition(); //calc distance between two ends of bungee

  float length = force.magnitude();

  //if the bungee is compressed, and not long enough to compress 
  if (length <= restLength)
  {
    return; // compressed (not extended beyond rest length) - do not apply force
  }
  //else if long enough to compress, but not long enough to snap
  else if (length < snapLength)
  {
    //calc magnitude of force
    length = springConstant * (restLength - length);
  
    //calc and apply
    force.normalise();
    force *= -length;
    //std::cout << "x: " << force.getX() << "y: " << force.getY() << "\n";
    shape-> addForce(force);
  }
  //else the bungee has exceeded its max length, and has snapped
  else
  {
    //std::cout << "snap - length: " << length << " lenBeforeSnapping: " << snapLength << "\n";
    setSnapped(true);
    //delete pointer
  }
}

//BUOYANCY

ShapeBuoyancy::ShapeBuoyancy(float p_depthForMaxBuoyancyForce, float p_shapeVolume, float p_waterHeight, float p_waterDensity) :
    depthForMaxBuoyancyForce(p_depthForMaxBuoyancyForce), shapeVolume(p_shapeVolume), waterHeight(p_waterHeight), waterDensity(p_waterDensity)
{}

void ShapeBuoyancy::updateForce(ShapeBody* shape, float duration)
{
  float shapeDepth = shape->getPositionY();
  // std::cout << "ShapeBuoy.updateForce(): shapeDepth: " << shapeDepth << "waterHeight: " << waterHeight << "\n";

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
    force.setY(-(waterDensity * shapeVolume)); //set the force to it's max (full volume of shape * water density)
    shape->addForce(force);
    //std::cout << "ShapeBuoy.updateForce(): Fully submerged, adding force: " << force.getX() << ", " << force.getY() << "\n";
    return;
  }

  //PARTIALLY SUBMERGED

  float depthInWater = (shapeDepth + depthForMaxBuoyancyForce) - waterHeight; //y value of object in the water
  float percentageInWater = depthInWater / depthForMaxBuoyancyForce; //percentage of object in the water
  float volumeInWater = percentageInWater * shapeVolume; //value of volume that is in water
  float forceY = volumeInWater * waterDensity;

  //std::cout << "ShapeBuoy.updateForce(): Partially submerged, adding forceY: " << -forceY << "\n";

  force.setY(-forceY);

  shape->addForce(force);
}


void BlobForceGenerator::updateForce(ShapeBody *shape, float duration)
{
  unsigned joinCount = 0;
  for (unsigned i = 0; i < shapesCount; i++)
  {
    // Don't attract yourself
    if (shapes + i == shape) continue;

    // Work out the separation distance
    Vector2D separation =
        shapes[i].getPosition() - shape->getPosition();
    //separation.z = 0.0f;
    float distance = separation.magnitude();

    if (distance < minNaturalDistance)
    {
      // Use a repulsion force.
      distance = 1.0f - distance / minNaturalDistance;
      shape->addForce(
          separation.unit() * (1.0f - distance) * maxReplusion * -1.0f
                         );
      joinCount++;
    }
    else if (distance > maxNaturalDistance && distance < maxDistance)
    {
      std::cout << " using attraction force\n";
      // Use an attraction force.
      distance =
          (distance - maxNaturalDistance) /
          (maxDistance - maxNaturalDistance);
      shape->addForce(
          separation.unit() * distance * maxAttraction
                         );
      joinCount++;
    }
  }

  // If the particle is the head, and we've got a join count, then float it.
  if (shape == shapes && joinCount > 0 && maxFloat > 0)
  {
    float force = float(joinCount / maxFloat) * floatHead;
    if (force > floatHead) force = floatHead;
    shape->addForce(Vector2D(0, force)); //dubious
  }

}
