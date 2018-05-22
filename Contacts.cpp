#include "Contacts.h"
//#include "ShapeBody.h"


ShapeContact::ShapeContact(ShapeBody* p_shape01, ShapeBody* p_shape02, float p_restitution, Vector2D p_contactNormal)
    : restitution(p_restitution), contactNormal(p_contactNormal)
{
  shapesInContact[0] = p_shape01;
  if (p_shape02) shapesInContact[1] = p_shape02;
}

void ShapeContact::resolve(float duration)
{
  resolveVelocity(duration);
  resolveInterpenetration(duration);
}

float ShapeContact::calculateSeparatingVelocity() const
{
  Vector2D relativeVelocity = shapesInContact[0]->getVelocity();
  if (shapesInContact[1])
  {
    relativeVelocity -= shapesInContact[1]->getVelocity();
  }
  float ans = relativeVelocity * contactNormal; //velocity * direction of contact from first obj's perspective

  return ans;
}

void ShapeContact::resolveVelocity(float duration)
{
  //get velocity in the direction of the contact
  float separatingVelocity = calculateSeparatingVelocity();

  if (separatingVelocity > 0)
  {
    //contact is either separating or stationary - no impulse required
    return;
  }

  //calc new separating velocity - multiply the velocity by the multiplier (bounciness)
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
  float totalInverseMass = shapesInContact[0]->getMass(); //get first objects mass //CHECK getMass is correct - was origin get IVMass
  if (shapesInContact[1]) //if a second object
  {
    totalInverseMass += shapesInContact[1]->getMass(); //add its mass //CHECK getMass is correct - was origin get IVMass
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

  //apply impulses - applied in direction of the contact, proportional to inverse mass
  shapesInContact[0]->setVelocity(shapesInContact[0]->getVelocity() +
                                  impulsePerInverseMass * shapesInContact[0]->getMass());//CHECK getMass is correct - was origin get IVMass

  if (shapesInContact[1])
  {
    //shapes[1] moves in pposite direction
    shapesInContact[1]->setVelocity(shapesInContact[1]->getVelocity() +
                                    impulsePerInverseMass * -shapesInContact[1]->getMass());//CHECK getMass is correct - was origin get IVMass
  }
}

void ShapeContact::resolveInterpenetration(float duration)
{
  //if no penetration then return
  if (penetrationDepth <= 0) return;

  //movement is based off mass
  float totalInverseMass = shapesInContact[0]->getMass();
   //if a second object add its mass
  if (shapesInContact[1]) totalInverseMass += shapesInContact[1]->getMass(); //CHECK getMass is correct - was origin get IVMass

  //if all shapes have infinite mass - no impulse required
  if ( totalInverseMass <= 0) return;

  //amount of penetration resolution per unit of mass
  Vector2D movePerIMass = contactNormal * (penetrationDepth / totalInverseMass);

  //apply pen resolution
  shapesInContact[0]->setPosition(shapesInContact[0]->getPosition() + movePerIMass * shapesInContact[0]->getMass());

  if (shapesInContact[1])
  {
    shapesInContact[1]->setPosition(shapesInContact[1]->getPosition() + movePerIMass * -shapesInContact[1]->getMass());
  }
}

ShapeContactResolver::ShapeContactResolver(unsigned p_maxIterations)
    : maxIterations(p_maxIterations)
{}

void ShapeContactResolver::setMaxIterations(unsigned p_maxIterations)
{
  maxIterations  = p_maxIterations;
}

void ShapeContactResolver::resolveContacts(ShapeContact *contactsArray, unsigned numContacts, float duration)
{
  iterationsRun = 0;
  while (iterationsRun < maxIterations)
  {
    //Find contact with the largest closing velocity
    float max = 0;
    unsigned maxIndex = numContacts;
    for (size_t i = 0; i < numContacts; i++)
    {
      float separatingVelocity = contactsArray[i].calculateSeparatingVelocity();
      if (separatingVelocity < max)
      {
        max  = separatingVelocity;
        maxIndex = i;
      }
    }

    //resolve this contact
    contactsArray[maxIndex].resolve(duration);
    iterationsRun++;
  }
}
