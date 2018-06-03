#ifndef _World_
#define _World_
#include "Contacts.h"
#include "ForceGenerator.h"
#include "ShapeBody.h"
#include "Vector2D.h"
#include <vector>

class ShapeContact;


class World {

public:
  //typedef std::vector<ShapeBody*> Shapes;
  typedef std::vector<ShapeContactGenerator*> ContactGenerators;

protected:

  //Shapes shapes;
  std::vector<ShapeBody*> shapes;

  //true if world will calculate the number of iterations to give to contactResolver each frame
  bool calculateIterations;

  //holds all force generators for shapes in world
  ForceRegistry registry;

  //holds resolver for Contacts
  ShapeContactResolver resolver;

  //list of ShapeContactGenerators
  ContactGenerators contactGenerators;

  //hold list of contacts
  //std::vector<ShapeContact*> contacts;
  ShapeContact* contacts;

  //max number of contacts allowed (size of contacts array)
  unsigned maxContacts;

public:

  //new world, handles up to max contacts per frame (maxContacts is usually num of objs in scene).
  //Optional - supply num of contact-resolution p_iterations to use (default contacts number x2)
  World(unsigned maxContacts, unsigned iterations = 0);
  
  ~World(); //destructor

  //calls each contact generator to report their contacts
  //returns number of generatred contacts
  unsigned generateContacts();

  //integrates all objs in world forward in time by given value
  void integrate(float duration);

  //processes all physics for the World
  void applyPhysics(float duration);

  //run at start of each frame - clears all forces added to every shape//shapes all have forces added ready for force accumulation
  void clearAccumulatedForces();

  //Getters
  
  std::vector<ShapeBody*>* getShapes();

  ContactGenerators& getContactGenerators();

  ForceRegistry& getForceRegistry();
};

#endif
