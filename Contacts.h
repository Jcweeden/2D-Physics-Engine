#ifndef _ShapeContact_
#define _ShapeContact_
#include "ShapeBody.h"
#include "Vector2D.h"


/*
  ShapeContact does not resolve a contact, or apply any forces itself, but is used within the running of
  ShapeContactResolver.
  
  ShapeContact holds the two shapes currently colliding (where the shapes are interpenetrating). This collision
  is resolved by applying an impulse force to separate them, taking into account the mass of each object, and the
  original velocities they were travelling at.
 */
class ShapeContact {

  friend class ShapeContactResolver;
  
public:

  //constructors
  ShapeContact(const ShapeContact& other); 
  ShapeContact(ShapeBody* p_shape01, ShapeBody* p_shape02, float p_restitution, Vector2D p_contactNormal);
  ShapeContact();

  //copy operator
  ShapeContact& operator=(const ShapeContact& other); //disable copy operator


  //the shapes involved in the collision (second element can be NULL to represent contact with scenery)
  ShapeBody* shapesInContact[2];

  //normal restitution coefficent of contact (how bouncy, 1 = keeping all velocity after contact)
  float restitution;

  //direction of the contact - from first object's perspective
  Vector2D contactNormal;

  //depth of contact penetration, given in direction of contactNormal
  //if negative there is no interpenetration between two objs, 0 is touching
  //by moving objects in direction of contactNormal by penDepth then there will be no contact
  float penetrationDepth;

  //contains impulse amount each shape is moved by from penetration - applied in resolveContacts()
  Vector2D shapeMovement[2];

protected:
  //resolves a contact -the velocity of objects is affected to resolve any interpenetrations
  void resolve(float duration);

  //subtract the velocity of the second object from first, and multiply by the direction of the contact
  //from first obj's perspective
  float calculateSeparatingVelocity() const;

private:
  //handles impulse for collision
  void resolveVelocity(float duration);

  //interpen resolution for collision
  void resolveInterpenetration(float duration);
};


/**
   ShapeContactResolver applies impulse force to objects to simulate collisions. A list of contacts is received
   and passed into the resolveContacts() method. resolveContacts() generates an impulse force which changes ONLY
   the velocity of each shape in collision, and not the acceleration. To generalise, the following actions are
   taken (for step-by-step pseudo-code see comment above resolveContacts()):

   1. Checks are conducted to ensure that objects are not resting but actually colliding.
   2. For those that are colliding impulse is applied to shapes in contact to simulate them bouncing apart.
   2. Any interpenetrating objects are moved apart.

   One ShapeContactResolver instance is shared for whole system.
   The maximum number of contacts ShapeContactResolver will resolve in a single frame is dictated by maxIterations.
**/
class ShapeContactResolver
{
protected:
  //max number of iterations of contact resolution that will be run in a single frame
  unsigned maxIterations;

  //num iterations used thus far
  unsigned iterationsRun;

public:
  ShapeContactResolver(unsigned p_maxIterations); //const

  //sets the number of times resolveContacts will iterate
  void setMaxIterations(unsigned p_maxIterations);
  
  /**
     For the list of contacts passed they are iterated on and their contacts resolved to remove penetration, and apply
     impulse force. The working is as follows:
     
     1. Calculate the separating velocity of each contact, and hold the contact with the strongest separating force.
     2. If this value is above 0 it is already moving away, and there is/are no collisions to be dealt with. Return.
     3. If there are collisions to be resolved, the ShapeContact method resolve() is called on the contact found in
        step 1, and the contact is resolved by adding impulse to remove any interpenetration.
     4. Repeat from step 1 until all collisions are resolved.

     The approach taken in this method repeatedly examines the entire list of collisions, even after a collision has
     been resolved. This is undertaken as the resolution of a collision may well create movement that creates yet
     another contact. The maximum number of times this method will loop is limited at the value of maxIteration.
   **/
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
