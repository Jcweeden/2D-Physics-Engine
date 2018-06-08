#include "Platform.h"

unsigned Platform::numShapesToCheckCollisionsWith = 0;

//check if the platform is in collisions with objects in shapesToCheckContactsWithVector
unsigned Platform::addContact(ShapeContact *contact, unsigned limit) 
{
  //holds number of collisions found thus far
  unsigned used = 0;

  //for each object to check contacts with
  for (unsigned i = 0; i < shapesToCheckContactsWithVector.size(); i++)
  {
    //if have reached the max number of collisions to find, then break.
    if (used >= limit) break;

    //check for penetration between platform and object i

    //vector bewteen start of platform and obj
    /*a*/Vector2D toParticle = shapesToCheckContactsWithVector[i]->getPosition() - start;

    //vector representing platform
    /*b*/Vector2D lineDirection = end - start;

    //scalar product
    /*c*/float projected = toParticle * lineDirection;
    
    /*d*/float platformSqLength = lineDirection.squareMagnitude();

    //if obj is nearest to the start point of platform   
    if (projected <= 0)
    {
      if (toParticle.squareMagnitude() < shapeRadius*shapeRadius)
      {
        //objs collided
        contact->contactNormal = toParticle.unit(); //normalise
        contact->restitution = restitution;
        contact->shapesInContact[0] = shapesToCheckContactsWithVector[i];
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = shapeRadius - toParticle.magnitude();
        used ++;
        contact ++;
      }
      //else there is no collision

    }
    //if obj is nearest to the end point of platform  
    else if (projected >= platformSqLength)
    {
      toParticle = shapesToCheckContactsWithVector[i]->getPosition() - end;
      if (toParticle.squareMagnitude() < shapeRadius * shapeRadius)
      {
        //objs collided
        contact->contactNormal = toParticle.unit();
        contact->restitution = restitution;
        contact->shapesInContact[0] = shapesToCheckContactsWithVector[i];
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = shapeRadius - toParticle.magnitude();
        used ++;
        contact ++;
      }
      //else there is no collision
    }
    else
    {
    //if obj is nearest to the middle of platform  
      float distanceToPlatform =
          toParticle.squareMagnitude() - projected*projected / platformSqLength;
      if (distanceToPlatform < shapeRadius * shapeRadius)
      {
        //objs collided
        Vector2D closestPoint = start + lineDirection*(projected/platformSqLength);

        contact->contactNormal = (shapesToCheckContactsWithVector[i]->getPosition() - closestPoint).unit();
        contact->restitution = restitution;
        contact->shapesInContact[0] = shapesToCheckContactsWithVector[i];
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = shapeRadius - sqrt(distanceToPlatform);
        used ++;
        contact ++;
      }
    }
  }
  return used;
}
