#ifndef _ShapeContact_
#define _ShapeContact_
#include "ShapeBody.h"
#include "Vector2D.h"

//class ShapeBody;

class ShapeContact {

  friend class ShapeContactResolver;
  
public:

ShapeContact(const ShapeContact& other);
ShapeContact& operator=(const ShapeContact& other);

  
  ShapeContact(ShapeBody* p_shape01, ShapeBody* p_shape02, float p_restitution, Vector2D p_contactNormal);

  ShapeContact();

  //the shapes involved in the collision (second element can be NULL to represent contact with scenery)
  ShapeBody* shapesInContact[2];

  //normal restitution coefficent of contact (how bouncy, 1 = keeping all velocity after contact)
  float restitution;

  //direction of the contact - from first object's perspective
  Vector2D contactNormal;

  //depth of contact penetration, given in direction of contactNormal //if negative there is no interpenetration between two objs, 0 is touching
  //by moving objects in direction of contactNormal by penDepth then there will be no contact
  float penetrationDepth;

  //contains amount each shape is moved by penetration - applied in resolveContacts()
  Vector2D shapeMovement[2];

protected:
  //resolve contact - both velociy and interpenetration
  void resolve(float duration);

  float calculateSeparatingVelocity() const;

private:
  //handles impulse for collision
  void resolveVelocity(float duration);

  //interpen resolution for collision
  void resolveInterpenetration(float duration);
};



//one resolver instance is shared for whole system
class ShapeContactResolver
{
protected:
  //max number of iterations
  unsigned maxIterations;

  //num iterations used thus far
  unsigned iterationsRun;

public:
  ShapeContactResolver(unsigned p_maxIterations); //const

  void setMaxIterations(unsigned p_maxIterations);

  //resolves shape contacts for penetration and velocity
  void resolveContacts(/*std::vector<ShapeContact*> contactsArray*/ShapeContact *contactsArray, unsigned numContacts, float duration);
};


//each ShapeContactGenerator gets called in turn from the world and can contribute any contacts it finds back to the world
//using its addContact() method
class ShapeContactGenerator
{
public:

  //fills shapeContact with generated contact
  //returns number of contacts that have been written
  virtual unsigned addContact(ShapeContact* contact, unsigned limit) = 0;
};


#endif
