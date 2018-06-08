#ifndef _ShapeLink_
#define _ShapeLink_
#include "ShapeBody.h"
#include "Vector2D.h"
#include "Contacts.h"


/**
   A ShapeLink describes the connection between two points. Parent Class of Rod and Cable.
 **/
class ShapeLink : public ShapeContactGenerator {
public:
  //two points held together by this link
  ShapeBody* linkedShapes[2];

protected:
  //return current length of link
  float currentLength() const;

public:
  //varying according to type of ShapeLink (cable or rod) contacts are generated if the start and end of the
  //shape are extended beyond its maximum length. The force applied from the contact will pull the link
  //back towards its normal length
  virtual unsigned addContact(ShapeContact *contact, unsigned limit) = 0;
};


/**
   Cables are flexible in their length, and once past the maxLength will apply force to return back to the max
   length. If under the maxLengthBeforeStretching no forces will be applied, but once stretching past a certain
   length the cable will snap and no further force will be applied.
 **/
class ShapeCable : public ShapeLink
{
public:
  bool hasSnapped = false;
  
  //if the cable is longer than this length it will generate a contact and
  //pull the other linked shape with it
  float cableMaxLengthBeforeStretching;

  //if the cable is stetched to this length then the cable itself will snap
  float cableMaxLengthBeforeSnapping;

  //the amount of stiffness of the cable, closer to 1 will ping more
  float restitution;

  //once past maxLengthBeforeSnapping force will be applied to pull the start and end back together
  //if overstretched past snapping length the cable will no longer have force applied
  virtual unsigned addContact(ShapeContact *contact, unsigned limit);

  //set whether the cable has snapped
  bool setSnapped(bool val) {hasSnapped = val;}
};

/**
   Rods are not flexible, and will retain the same length (rodLength) between the start and end points
 **/
class ShapeRod : public ShapeLink
{
public:
  //the designated length a rod should remain at, the distance between start and end points
  float rodLength;
  
  //rods are solid and therefore restitution is always 0 - not required
  //float restitution;

  //if the rod is overstetched the rod will have force applied to snap it back to the rodLength between
  //the start and end points
  virtual unsigned addContact(ShapeContact *contact, unsigned limit);
};

#endif
