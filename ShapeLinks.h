#ifndef _ShapeLink_
#define _ShapeLink_
#include "ShapeBody.h"
#include "Vector2D.h"
#include "Contacts.h"

//class ShapeBody;

class ShapeLink : public ShapeContactGenerator {
public:
  //two shapes held together by this link
  ShapeBody* linkedShapes[2];

protected:
  //return current length of link
  float currentLength() const;

public:
  virtual unsigned addContact(ShapeContact *contact, unsigned limit) = 0;
};



class ShapeCable : public ShapeLink
{
public:

  bool hasSnapped = false;
  
  //if the cable is longer than this length it will generate a contact and
  //pull the other linked shape with it
  float cableMaxLengthBeforeStretching;

  //if the cable is stetched to this length then the cable itself will snap
  float cableMaxLengthBeforeSnapping;
  
  float restitution;
  
  virtual unsigned addContact(ShapeContact *contact, unsigned limit);

  bool setSnapped(bool val) {hasSnapped = val;}
};


class ShapeRod : public ShapeLink
{
public:
  float rodLength;
  
  //restitution is always 0
  //float restitution;
  
  virtual unsigned addContact(ShapeContact *contact, unsigned limit);
};

#endif
