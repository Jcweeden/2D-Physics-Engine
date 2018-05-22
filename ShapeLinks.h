#ifndef _ShapeLink_
#define _ShapeLink_
#include "ShapeBody.h"
#include "Vector2D.h"
#include "Contacts.h"

//class ShapeBody;

class ShapeLink {
public:
  //two shapes held together by this link
  ShapeBody* linkedShapes[2];

protected:
  //return current length of link
  float currentLength() const;

public:
  virtual unsigned fillContact(ShapeContact *contact, unsigned limit) const = 0;
};



class ShapeCable : public ShapeLink
{
public:
  float cableMaxLength;

  float restitution;
  
  virtual unsigned fillContact(ShapeContact *contact, unsigned limit) const;
};


class ShapeRod : public ShapeLink
{
public:
  float rodLength;
  
  //restitution is always 0
  //float restitution;
  
  virtual unsigned fillContact(ShapeContact *contact, unsigned limit) const;
};

#endif
