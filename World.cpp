#include "World.h"


World::World(unsigned maxContacts, unsigned iterations)
:
    resolver(iterations),    //max number of contacts that will be resolved by ShapeContactResolver per frame
    maxContacts(maxContacts) //max number of contacts that will collected from generateContacts() per frame
{

  //set the contacts array to be of size defined by the parameter (max number of contacts)
  contacts = new ShapeContact[maxContacts];

  //if a maximum number of contacts to check each frame has been set, then set maxIterationsOfResolverSet to true
  if (iterations == 0) { maxIterationsOfResolverSet = true; }
  else { maxIterationsOfResolverSet = false; }
}

World::~World()
{
  delete[] contacts;
}

void World::applyPhysics(float duration)
{
  //apply the force generators to the shapes
  registry.updateForces(duration);

  //integrate to update and the velocity and position of each shape
  integrate(duration);

  //check if there are any contacts betweent the newly positioned shapes
  unsigned usedContacts = generateContacts();

  //if there are contacts
  if (usedContacts) 
  {
    //if a max number of contacts to deal with each frame has been set, then set this maximum
    if (maxIterationsOfResolverSet) resolver.setMaxIterations(usedContacts * 2);

    //resolve the contacts
    resolver.resolveContacts(contacts, usedContacts, duration);
  }
}


//run at start of each frame - clears all forces added each obj in shapes, ready for new forces in this frame
void World::clearAccumulatedForces()
{
  for (size_t i = 0; i < shapes.size(); i++)
  {
    //for each object remove all forces added in last frame from the accumulator
    shapes[i]->clearAccumForces();
  }
}


//applies physics to all objs in World
void World::integrate(float duration)
{
  for (size_t i = 0; i < shapes.size(); i++)
  {
    shapes[i]->physicsIntegration();
  }
}


unsigned World::generateContacts()
{
  //initially set limit as the max num of contacts that can be added
  //this value will be decremented for each contact added
  unsigned limit = maxContacts;

  //get the first contact from contacts
  ShapeContact *nextContact = contacts;

  //for each contact generator (objects looking for collisions)
  for (ContactGenerators::iterator g = contactGenerators.begin();
       g != contactGenerators.end();
       g++)
  {
    //ShapeContactGenerator.getContact() - get the the number of contacts found in collision that object
    //and its set vector of objects to check for collisions with, searching for contacts only up to the
    //limit of contacts left to be found
    unsigned used =(*g)->addContact(nextContact, limit);

    //deduct from the total number of contacts left to find the number that was just found
    limit -= used;

    //increment to load the next contact generator
    nextContact += used;

    //if have reached the max number of contacts to be processed per frame then break
    //any further contacts in the frame will not be processed
    if (limit <= 0) break;
  }

  //return number of contacts found, if above numContacts > 0 then resolveContacts() will be run
  return maxContacts - limit; 
}





std::vector<ShapeBody*> World::getShapes()
{
  return shapes;
}

void World::setShapes (std::vector<ShapeBody*> p_shapes)
{
  shapes = p_shapes;
}

World::ContactGenerators& World::getContactGenerators()
{
  return contactGenerators;
}

ForceRegistry& World::getForceRegistry()
{
  return registry;
}



