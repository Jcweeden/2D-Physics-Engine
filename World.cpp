#include "World.h"


World::World(unsigned maxContacts, unsigned iterations)
:
resolver(iterations),
maxContacts(maxContacts)
{
  //contacts = new std::vector<ShapeContact> arr(maxContacts);
  contacts = new ShapeContact[maxContacts];

  calculateIterations = (iterations == 0);

}


  World::~World()
{
    delete[] contacts;
}


void World::clearAccumulatedForces()
{
  for (size_t i = 0; i < shapes.size(); i++)
  {
    // Remove all forces from the accumulator
    shapes[i]->clearAccumForces();
    
  }
}

unsigned World::generateContacts()
{
  //max num of contacts that can be added //val will be decremented as contacts are added

  unsigned limit = maxContacts;
  ShapeContact *nextContact = contacts;

  int counter = 0;
  
  //WORKS FOR AMOUNT IN LIST
  for (ContactGenerators::iterator g = contactGenerators.begin();
       g != contactGenerators.end();
       g++)
  {
    
    unsigned used =(*g)->addContact(nextContact, limit);
    limit -= used;
    nextContact += used;

    // We've run out of contacts to fill. This means we're missing
    // contacts.
    if (limit <= 0) break;

    counter++;
  }

  // Return the number of contacts used.
  return maxContacts - limit; 
}


void World::integrate(float duration)
{
  for (size_t i = 0; i < shapes.size(); i++)
  {
    // Remove all forces from the accumulator
    shapes[i]->physicsIntegration(/*duration*/);
  }
}

void World::applyPhysics(float duration)
{
    // First apply the force generators
  registry.updateForces(duration);

    // Then integrate the objects
  integrate(duration); //***

    // Generate contacts
  unsigned usedContacts = generateContacts(); //***

  // And process them
  if (usedContacts) 
  {
   if (calculateIterations) resolver.setMaxIterations(usedContacts * 2);
   resolver.resolveContacts(contacts, usedContacts, duration);
  }
}

std::vector<ShapeBody*>* World::getShapes()
{
  return &shapes;
}

World::ContactGenerators& World::getContactGenerators()
{
    return contactGenerators;
}

ForceRegistry& World::getForceRegistry()
{
    return registry;
}



