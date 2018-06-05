#include "Contacts.h"
//#include "ShapeBody.h"
#include <vector>
#include <limits>

ShapeContact::ShapeContact(ShapeBody* p_shape01, ShapeBody* p_shape02, float p_restitution, Vector2D p_contactNormal)
    : restitution(p_restitution), contactNormal(p_contactNormal)
{
  //set the shapes that are in contact
  shapesInContact[0] = p_shape01;
  if (p_shape02) shapesInContact[1] = p_shape02;
}

ShapeContact::ShapeContact()
    : restitution(0.0f), contactNormal(Vector2D(0,0)), penetrationDepth(0.0f)
{}


void ShapeContact::resolve(float duration)
{
  resolveVelocity(duration);
  resolveInterpenetration(duration);
}


//subtract the velocity of the second object from first, and multiply by the direction of the contact
float ShapeContact::calculateSeparatingVelocity() const
{
  //get velocity of first shape in the contact
  Vector2D relativeVelocity = shapesInContact[0]->getVelocity();

  //if the second shape in contact is not scenery, and is movable
  if (shapesInContact[1])
  {
    //absorb the velocity of the second object
    relativeVelocity -= shapesInContact[1]->getVelocity();
  }

  //return the velocity * direction of contact from first obj's perspective
  return relativeVelocity * contactNormal; 
}

void ShapeContact::resolveVelocity(float duration)
{
  //get velocity in the direction of the contact
  float separatingVelocity = calculateSeparatingVelocity();

  if (separatingVelocity > 0)
  {
    //contact is either separating or sitting stationary - no impulse required
    return;
  }

  //calc new separating velocity - multiply the velocity by the restitution multiplier (bounciness)
  float newSepVelocity = -separatingVelocity * restitution;

  //check velocity build-up due to acceleration only
  Vector2D accCausedVelocity = shapesInContact[0]->getAcceleration();
  if (shapesInContact[1]) accCausedVelocity -= shapesInContact[1]->getAcceleration();
  float accCausedSepVelocity = accCausedVelocity * contactNormal * duration;

  //if have closing velocity due to acceleration build-up remove from the new separating velocity
  if (accCausedSepVelocity < 0)
  {
    newSepVelocity += restitution * accCausedSepVelocity;

    //ensure have not removed more than there was to remove
    if (newSepVelocity <0) newSepVelocity = 0;
  }

  
  float deltaVelocity = newSepVelocity - separatingVelocity;

  //use mass to proportionally change the velocity (higher mass gives a lesser change in velocity)
  float totalInverseMass = shapesInContact[0]->getMass(); //get first objects mass
  if (shapesInContact[1]) //if a second object (is not scenery)
  {
    totalInverseMass += shapesInContact[1]->getMass(); //add its mass
  }

  if ( totalInverseMass <= 0)
  {
    //all shapes have infinite mass - no impulse required
    return;
  }

  //calculate impulse to apply
  float impulse = deltaVelocity / totalInverseMass;

  //impulse per unit of inverse mass
  Vector2D impulsePerInverseMass = contactNormal * impulse;

  //apply impulses first object in contact - applied in direction of the contact, proportional to inverse mass
  shapesInContact[0]->setVelocity(shapesInContact[0]->getVelocity() +
                                  impulsePerInverseMass * shapesInContact[0]->getMass());
  
  //apply impulses first object in contact - applied in direction of the contact, proportional to inverse mass
  if (shapesInContact[1])
  {
    //shapes[1] moves in opposite direction, as contactNormal was in direction of shapesInContact[0]
    shapesInContact[1]->setVelocity(shapesInContact[1]->getVelocity() +
                                    impulsePerInverseMass * -shapesInContact[1]->getMass());
  }
}

void ShapeContact::resolveInterpenetration(float duration)
{
  //if no penetration then return
  if (penetrationDepth <= 0) return;

  //movement is based off mass, so sum mass's of objects
  float totalInverseMass = shapesInContact[0]->getMass();
  //if a second object add its mass
  if (shapesInContact[1]) totalInverseMass += shapesInContact[1]->getMass(); 

  //if all shapes have infinite mass - no impulse required
  if ( totalInverseMass <= 0) return;

  //amount of penetration resolution force that will be applied per single unit of mass each object has
  Vector2D movePerIMass = contactNormal * (penetrationDepth / totalInverseMass);

  //calculate the the movement amount for shape 0
  shapeMovement[0] = movePerIMass * shapesInContact[0]->getMass();

  //if a second object
  if (shapesInContact[1]) {
    //calculate the the movement amount for shape 0, negate the mass to get movement in opposite direction
    shapeMovement[1] = movePerIMass * -shapesInContact[1]->getMass();
  } else {
    shapeMovement[1].clear();
  }

  //apply the calculated impulses to resolve the penetration
  shapesInContact[0]->setPosition(shapesInContact[0]->getPosition() + shapeMovement[0]);
  
  if (shapesInContact[1]) {
    shapesInContact[1]->setPosition(shapesInContact[1]->getPosition() + shapeMovement[1]);
  }
}







ShapeContactResolver::ShapeContactResolver(unsigned p_maxIterations)
    : maxIterations(p_maxIterations)
{}

void ShapeContactResolver::setMaxIterations(unsigned p_maxIterations)
{
  maxIterations  = p_maxIterations;
}


void ShapeContactResolver::resolveContacts(ShapeContact *contactArray,
                                              unsigned numContacts,
                                              float duration)
{
    unsigned i;

    iterationsRun = 0;
    while(iterationsRun < maxIterations)
    {
        // Find the contact with the largest closing velocity;
        float max = std::numeric_limits<double>::max();
        unsigned maxIndex = numContacts;
        for (i = 0; i < numContacts; i++)
        {
          float sepVel = contactArray[i].calculateSeparatingVelocity();
            if (sepVel < max &&
                (sepVel < 0 || contactArray[i].penetrationDepth > 0))
            {
                max = sepVel;
                maxIndex = i;
            }
        }

        // Do we have anything worth resolving?
        if (maxIndex == numContacts) break;

        // Resolve this contact
        contactArray[maxIndex].resolve(duration);

        // Update the interpenetrations for all particles
        Vector2D *move = contactArray[maxIndex].shapeMovement;
        for (i = 0; i < numContacts; i++)
        {
            if (contactArray[i].shapesInContact[0] == contactArray[maxIndex].shapesInContact[0])
            {
                contactArray[i].penetrationDepth -= move[0] * contactArray[i].contactNormal;
            }
            else if (contactArray[i].shapesInContact[0] == contactArray[maxIndex].shapesInContact[1])
            {
                contactArray[i].penetrationDepth -= move[1] * contactArray[i].contactNormal;
            }
            if (contactArray[i].shapesInContact[1])
            {
                if (contactArray[i].shapesInContact[1] == contactArray[maxIndex].shapesInContact[0])
                {
                    contactArray[i].penetrationDepth += move[0] * contactArray[i].contactNormal;
                }
                else if (contactArray[i].shapesInContact[1] == contactArray[maxIndex].shapesInContact[1])
                {
                    contactArray[i].penetrationDepth += move[1] * contactArray[i].contactNormal;
                }
            }
        }

        iterationsRun++;
    }
}
