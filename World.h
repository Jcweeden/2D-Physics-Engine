#ifndef _World_
#define _World_
#include "Contacts.h"
#include "ForceGenerator.h"
#include "ShapeBody.h"
#include "Vector2D.h"
#include <vector>
class ShapeContact;


/**
   World operates as a structure that holds all the rigidbodies with a simulation. From within
   this class the appropriate methods are called to keep track of and update the rigidbodies.
   
   The core implementation of the class is applyPhysics(), of which runs through a list of
   methods that result in the application of physics to the objects.
   
   Every frame five key steps are undertaken:

   1. By running clearAccumulatedForces() for each shape in shapes the forces that it
   accumulated in the last frame are cleared.
   2. Run updateForces() to calculate the forces to be applied to each shape in this frame.
   3. Call integrate() to update the position and velocity of each shape according to its
   acceleration and the forces applied on it in step 1.
   4. With the forces applied we run generateContacts() to check if there are now any collisions
   between the shapes.
   5. If there are any contacts then these collisions are passed to ShapeContactResolver where
   resolveContacts() appropriate applies forces to resolve any interpenetrations.
 **/
class World {

public:
  //a list of all of all objects that will generate contacts, e.g. objects which can be collided with
  //each of thse objects will have objects assigned to a vector (shapesToCheckContactsWithVector) that
  //dictates the objects they are checking for contacts with
  
  typedef std::vector<ShapeContactGenerator*> ContactGenerators;

protected:

  //Shapes shapes;
  std::vector<ShapeBody*> shapes;

  //true if world will calculate the number of iterations to give to contactResolver each frame
  bool maxIterationsOfResolverSet;

  //holds all force generators for shapes in world - e.g. for gravity, springs, buoyancy etc.
  ForceRegistry registry;

  //holds the resolver for Contacts. any coliisions in shapes are passed here where the interpenetrations
  //are resolved
  ShapeContactResolver resolver;

  //vector of ShapeContactGenerators
  ContactGenerators contactGenerators;
  //std::vector<ShapeContactGenerator*> contactGenerators;

  //hold list of contacts
  //std::vector<ShapeContact*> contacts;
  ShapeContact* contacts;

  //max number of contacts allowed (size of contacts array)
  unsigned maxContacts;

public:

  //new simulation,
  //will handle up to max contacts per frame (maxContacts is usually num of objs in scene).
  //Optional - supply num of contact-resolution p_iterations to use (default contacts number x2)
  World(unsigned maxContacts, unsigned iterations = 0);
  
  ~World(); //destructor

  //calls each contact generator to report their contacts
  //returns number of generated contacts
  unsigned generateContacts();

  //applies physics to all objs in world
  void integrate(float duration);

  //processes all physics for the World
  void applyPhysics(float duration);

  //run at start of each frame - clears forces added to every object in shapes
  void clearAccumulatedForces();

  //getters
  std::vector<ShapeBody*> getShapes();

  void setShapes (std::vector<ShapeBody*> p_shapes);
  
  ContactGenerators& getContactGenerators();

  ForceRegistry& getForceRegistry();
};

#endif
