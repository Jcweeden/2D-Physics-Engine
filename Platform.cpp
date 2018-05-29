#include "Platform.h"

//#define BLOB_RADIUS 14.0f

unsigned Platform::numShapesToCheckCollisionsWith = 0;

unsigned Platform::addContact(ShapeContact *contact, unsigned limit) 
{

  unsigned used = 0;
  for (unsigned i = 0; i < numShapesToCheckCollisionsWith; i++) //FIX ME - BLOB_COUNT
  {
    if (used >= limit) break;

    // Check for penetration
    Vector2D toParticle = shapesToCheckContactsWith[i].getPosition() - start;
    Vector2D lineDirection = end - start;
    float projected = toParticle * lineDirection;
    float platformSqLength = lineDirection.squareMagnitude();
    if (projected <= 0)
    {
      // The blob is nearest to the start point
      if (toParticle.squareMagnitude() < shapeRadius*shapeRadius)
      {
        // We have a collision
        contact->contactNormal = toParticle.unit();
        //contact->contactNormal.z = 0;
        contact->restitution = restitution;
        contact->shapesInContact[0] = shapesToCheckContactsWith + i;
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = shapeRadius - toParticle.magnitude();
        used ++;
        contact ++;
      }

    }
    else if (projected >= platformSqLength)
    {
      // The blob is nearest to the end point
      toParticle = shapesToCheckContactsWith[i].getPosition() - end;
      if (toParticle.squareMagnitude() < shapeRadius * shapeRadius)
      {
        // We have a collision
        contact->contactNormal = toParticle.unit();
        // contact->contactNormal.z = 0;
        contact->restitution = restitution;
        contact->shapesInContact[0] = shapesToCheckContactsWith + i;
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = shapeRadius - toParticle.magnitude();
        used ++;
        contact ++;
      }
    }
    else
    {
      // the blob is nearest to the middle.
      float distanceToPlatform =
          toParticle.squareMagnitude() -
          projected*projected / platformSqLength;
      if (distanceToPlatform < shapeRadius * shapeRadius)
      {
        // We have a collision
        Vector2D closestPoint =
            start + lineDirection*(projected/platformSqLength);

        contact->contactNormal = (shapesToCheckContactsWith[i].getPosition()-closestPoint).unit();
        // contact->contactNormal.z = 0;
        contact->restitution = restitution;
        contact->shapesInContact[0] = shapesToCheckContactsWith + i;
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = shapeRadius - sqrt(distanceToPlatform);
        used ++;
        contact ++;
      }
    }
  }
  return used;
}
