#ifndef _ForceGen_
#define _ForceGen_
#include "ShapeBody.h"
#include "Vector2D.h"
#include <iostream>
#include <string>
#include <iostream>

//applies a single force to an object
class ForceGenerator {

public:
  virtual ~ForceGenerator(){};
  
  virtual void updateForce(ShapeBody *shape, float duration) = 0;
  //virtual void printForceGenType();
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

public:
  //list of force registrations
  typedef std::vector<ShapeForceRegistration> Registry;
  Registry registrations;

public:
  //registers force generator to apply to given shape
  void add(ShapeBody* shape, ForceGenerator *fg);

  //removes shape from registry
  void remove(ShapeBody* shape, ForceGenerator *fg);
  
  void remove(ForceGenerator *fg);
  void remove(ShapeBody* shape);

  
  //removes all registrations 
  void clear();

  //Calls all force generators to update their corresponding shapes
  void updateForces(float duration);
  
};

class ShapeGravity: public ForceGenerator
{
  Vector2D gravity;

public:
  ~ShapeGravity(){};
  ShapeGravity(const Vector2D &gravity); //constr
  ShapeGravity(ShapeGravity const &) = delete; //delete copy constr

  
  //virtual void printForceGenType() { std::cout << "ForceGen type : gravity\n";}
  virtual void updateForce(ShapeBody* shape, float durations);
};


class ShapeDrag: public ForceGenerator
{
  float k1; //velocity drag coefficient
  float k2; //velocity squared drag coefficient

public:
  ~ShapeDrag(){};
  ShapeDrag(ShapeDrag const &) = delete; //delete copy constr

  ShapeDrag(float k1, float k2); //constr
  //virtual void printForceGenType() { std::cout << "ForceGen type : drag\n"; }
  virtual void updateForce(ShapeBody* shape, float duration);
};


class ShapeSpring: public ForceGenerator
{
public:
  ShapeBody *endOfSpringObj;

  float springConstant;
  float restLength;
  float snapLength;
  bool hasSnapped;
  

public:
  ~ShapeSpring(){};
  ShapeSpring(ShapeSpring const &) = delete; //delete copy constr

  ShapeSpring(ShapeBody *p_endOfSpringObj, float p_springConstant, float p_restLength, float p_snapLength );

    ShapeSpring()
      : endOfSpringObj(nullptr), springConstant(1.0f), restLength(1.0f), snapLength(1.0f), hasSnapped(false)
  {}

  virtual void updateForce(ShapeBody* shape, float duration);

  void setSnapped(bool val) { hasSnapped = val; }
  bool getSnapped() { return hasSnapped; }
};


class ShapeAnchoredSpring: public ForceGenerator
{
  Vector2D *anchor;

  float springConstant;
  float restLength;

public:
  ShapeAnchoredSpring(Vector2D *p_anchor, float p_springConstant, float p_restLength);

  ShapeAnchoredSpring(ShapeAnchoredSpring const &) = delete; //delete copy constr

  virtual void updateForce(ShapeBody* shape, float duration);
};

class ShapeBungee: public ForceGenerator
{
public:
  ShapeBody *endOfBungeeObj;

  float springConstant;
  float restLength;
  float snapLength;
  bool hasSnapped;

public:
  ~ShapeBungee(){};
  ShapeBungee(ShapeBungee const &) = delete; //delete copy constr

  ShapeBungee(ShapeBody* p_endOfBungeeObj, float p_springConstant, float p_restLength, float p_snapLength);
  
  ShapeBungee()
      : endOfBungeeObj(nullptr), springConstant(1.0f), restLength(1.0f), snapLength(1.0f), hasSnapped(false)
  {}

  virtual void updateForce(ShapeBody* shape, float duration);

  void setSnapped(bool val) { hasSnapped = val; }
  bool getSnapped() { return hasSnapped; }
  
};


class ShapeBuoyancy: public ForceGenerator
{
  float depthForMaxBuoyancyForce; //when submerged at this depth the shape will generate max buoyancy force
  float shapeVolume;
  float waterHeight; //height of water, below which obj will be submerged
  float waterDensity; //set at standard of pure water - 1000kg per cubic meter

public:
  ~ShapeBuoyancy(){};
  ShapeBuoyancy(ShapeBuoyancy const &) = delete; //delete copy constructor
  
  ShapeBuoyancy(float p_depthForMaxBuoyancyForce, float p_shapeVolume, float p_waterHeight, float p_waterDensity = 1000.0f);
  //virtual void printForceGenType() { std::cout << "ForceGen type : buoyancy\n"; }
  virtual void updateForce(ShapeBody* shape, float duration);
};


class BlobForceGenerator : public ForceGenerator
{
 public:
  /**
   * Holds a pointer to the particles we might be attracting.
   */
  ShapeBody *shapes;

  //number of objects in shapes array
  unsigned shapesCount;
  
  /**
   * The maximum force used to push the particles apart.
   */
  float maxReplusion;

  /**
   * The maximum force used to pull particles together.
   */
  float maxAttraction;

  /**
   * The separation between particles where there is no force.
   */
  float minNaturalDistance, maxNaturalDistance;

  /**
   * The force with which to float the head particle, if it is
   * joined to others.
   */
  float floatHead;

  /**
   * The maximum number of particles in the blob before the head
   * is floated at maximum force.
   */
  unsigned maxFloat;

  /**
   * The separation between particles after which they 'break' apart and
   * there is no force.
   */
  float  maxDistance;

public:

  ~BlobForceGenerator(){};


  virtual void updateForce(
      ShapeBody *particle,
      float duration
                           );
};


#endif
