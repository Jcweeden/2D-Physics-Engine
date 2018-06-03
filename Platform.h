#ifndef _Platform_
#define _Platform_

#include "Contacts.h"
#include <vector>
#include "ShapeBody.h"

//an object defined by two points
class Platform : public ShapeContactGenerator
{
 public:

  //start and end coords of the platform
  Vector2D start;
  Vector2D end;

  //list of all objects the platform is to check for contacts with
  ShapeBody *shapesToCheckContactsWith;

  //**
  std::vector<ShapeBody*> shapesToCheckContactsWithVector;

  
  //number of items in the shapesToCheckContactsWith array
  static unsigned numShapesToCheckCollisionsWith;

  //the radius that the plaform will check against for collisions
  float shapeRadius;

  //bounciness of the platforms
  float restitution;

public:
  
  Platform()
      : start(0,0), end(0,0), restitution(0), shapeRadius(0)
  {}

Platform(float p_restitution, float p_shapeRadius)
    : start(0,0), end(0,0), restitution(p_restitution), shapeRadius(p_shapeRadius)
  {}

  void setRadius(float p_radius) { shapeRadius = p_radius; }
  void setRestitution(float p_restitution) { restitution = p_restitution; }

  
  virtual unsigned addContact(ShapeContact *contact, unsigned limit);
};

#endif
