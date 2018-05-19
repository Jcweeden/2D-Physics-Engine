#include "ForceGenerator.h"




void ForceRegistry::updateForces(float duration)
{
  Registry::iterator i = registrations.begin();

  for (; i != registrations.end(); i++)
  {
    i->fg->updateForce(i->shape, duration);
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


void ShapeGravity::updateForce(ShapeBody* shape, float duration)
{
  //ensure the obj has mass
  if (shape->getMass() != 0)
  {
    Vector2D grav = gravity * shape->getMass(); //dubious
    shape->addForce(grav);
  } 
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
