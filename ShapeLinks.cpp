#include "ShapeLinks.h"



float ShapeLink::currentLength() const
{
  Vector2D relativePosition = linkedShapes[0]->getPosition() - linkedShapes[1]->getPosition();

  //return length of distance between two link ends
  return relativePosition.magnitude();
}

//acts as col detect - examines state of cable and returns a contact if the cable has reached its limit
//this contact is added to all others, and processed in the contact resolver algorithm
unsigned ShapeCable::addContact(ShapeContact *contact, unsigned limit) 
{
  //check second element does exist - that cable has not already snapped
  if (hasSnapped) return 0;
  
  //get cable length
  float length = currentLength();

  //if not over max length, return 0 (no contact required)
  if (length < cableMaxLengthBeforeStretching) return 0;

  //if the cable has overstretched it's max length before snapping
  if (length > cableMaxLengthBeforeSnapping)
  {
    std::cout << "snap - length: " << length << " lenBeforeSnapping: " << cableMaxLengthBeforeSnapping << "\n";

    setSnapped(true);
    
    //remove the second object from the array
    //if (linkedShapes[1])
    //linkedShapes[1] = 0;
  }
  
  //if overstretched, then return a contact
  contact->shapesInContact[0] = linkedShapes[0];
  contact->shapesInContact[1] = linkedShapes[1];

  //calc normal
  Vector2D normal = linkedShapes[1]->getPosition() - linkedShapes[0]->getPosition();
  normal.normalise();
  contact->contactNormal = normal;

  //length of the penetration 
  contact->penetrationDepth = length - cableMaxLengthBeforeStretching;
  contact->restitution = restitution;

  return 1;
}

unsigned ShapeRod::addContact(ShapeContact *contact, unsigned limit)
{
  //get the current rod length
  float currLength = currentLength();

  //if not over max length, return 0
  if (currLength == rodLength) return 0;

  //if overstretched, then return the contact
  contact->shapesInContact[0] = linkedShapes[0];
  contact->shapesInContact[1] = linkedShapes[1];

  //calc normal
  Vector2D normal = linkedShapes[1]->getPosition() - linkedShapes[0]->getPosition();
  normal.normalise();

  if (currLength > rodLength)
  {
    contact->contactNormal = normal;
    contact->penetrationDepth = currLength-rodLength; //calc difference in size 
  } else
  {
    contact->contactNormal = normal * -1;
    contact->penetrationDepth = rodLength - currLength;
  }
  
  //always use 0 rest for a rod, it does not bounce
  contact->restitution = 0;

  return 1;
}
