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
  typedef std::vector<ShapeBody*> Shapes;
  typedef std::vector<ShapeContactGenerator*> ContactGenerators;

protected:

  Shapes shapes;

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
  void runPhysics(float duration);

  //run at start of each frame - clears all forces added to every shape//shapes all have forces added ready for force accumulation
  void startFrame();

  //Getters
  
  Shapes& getShapes();

  ContactGenerators& getContactGenerators();

  ForceRegistry& getForceRegistry();
};

#endif


  /*
public:
  typedef std::vector<ShapeBody*> Shapes; //
  typedef std::vector<ParticleContactGenerator*> ContactGenerators; //


  //holds one shape and pointer to the next in the linked list of shapes
  //struct ShapeRegistration
  //{
  // ShapeBody *shape;
  // ShapeRegistration *next;
  //};

  
  //ShapeRegistration* firstShape;
public: //members

  ForceRegistry registry;

  ShapeContactResolver resolver;

  //holds a single contact register and a link to the next
  struct ContactGenRegistration
  {
    ShapeContactGenerator *gen;
    ContactGenRegistration *next;
  };

  //hold first in linked list of contact generators
  ContactGenRegistration * firstContactGenRegistration;

  ShapeContact *contacts;

  unsigned maxContacts;

  bool calculateIterations;
  
public: //methods

  World(unsigned p_maxContacts, unsigned p_iterations=0);

  void startFrame();


  unsigned generateContacts();

  void integrate(float duration);

  void runPhysics(float duration);

  */

