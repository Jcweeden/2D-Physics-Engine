#ifndef _ForceGen_
#define _ForceGen_
#include "ShapeBody.h"
#include "Vector2D.h"
#include <iostream>
#include <string>


//applies a single force to an object
class ForceGenerator {

public:

  virtual void updateForce(ShapeBody *shape, float duration) = 0;

};

//keeps a vector holding all of the forces being applied to an object
class ForceRegistry {
protected:

  //tracks a shape and the forceGenerator that applies to it
  struct ShapeForceRegistration
  {
    ShapeBody *shape;
    ForceGenerator *fg;
  };

  //list of force registrations
  typedef std::vector<ShapeForceRegistration> Registry;
  Registry registrations;

public:
  //registers force generator to apply to given shape
  void add(ShapeBody* shape, ForceGenerator *fg);

  //removes shape from registry
  void remove(ShapeBody* shape, ForceGenerator *fg);

  //removes all registrations 
  void clear();

  //Calls all force generators to update their corresponding shapes
  void updateForces(float duration);
  
};

class ShapeGravity: public ForceGenerator
{
  Vector2D gravity;

public:
  ShapeGravity(const Vector2D &gravity); //constr

  virtual void updateForce(ShapeBody* shape, float durations);
};


class ShapeDrag: public ForceGenerator
{
  float k1; //velocity drag coefficient
  float k2; //velocity squared drag coefficient

public:
  ShapeDrag(float k1, float k2); //constr

  virtual void updateForce(ShapeBody *particle, float duration);
};

#endif
