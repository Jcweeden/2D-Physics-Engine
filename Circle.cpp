#include "Circle.h"
#include "SDL2/SDL2_gfxPrimitives.h" 


Circle::Circle(int p_x, int p_y, float p_radius, /* int p_rotation,*/ int p_mass, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA) :
    ShapeBody( p_x,  p_y, /*p_rotation,*/ p_mass, p_colourR,p_colourG,p_colourB,p_colourA )
{
  //init other variables
  radius = p_radius;
  //std::cout << "Circle.cpp: New Circle - radius: " << getRadius() << " - area: " << getArea() << "\n";
   
  area = getArea();
}

Circle::Circle() :
    ShapeBody( 0,  0, /*p_rotation,*/ 1, 0, 0, 0, 255)
{
  //init other variables
  radius = 30;
  //std::cout << "Circle.cpp: New Circle - radius: " << getRadius() << " - area: " << getArea() << "\n";
   
  area = getArea();
}



float Circle::getArea() {
  return pi * radius * radius;
}

void Circle::draw()
{  
  filledCircleRGBA(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), radius, colourR, colourG, colourB, colourA);
}

void Circle::update()
{
  checkIfHeldByMouse();
  
  ShapeBody::update();
}

void Circle::checkIfHeldByMouse()
{
 //CHECK IF HELD BY POINTER
  Vector2D* pMousePos = TheInputHandler::Instance()->getMousePosition();

  //HOLDING OBJECT
  if (isHeldByMouse) //if held by mouse
  {
    if (TheInputHandler::Instance()->getIsMouseButtonPressed())
    {
      setVelocity(0.0f,0.0f); //reset velocity
      isHeldByMouse = false;
      arePhysicsEnabled = true;
      TheInputHandler::Instance()->setIsHoldingObject(false);
      TheInputHandler::Instance()->setIsMouseButtonPressed(false);  //prevent other objects being picked up in same frame    
    } else
    {
      //move shape to mouse pointer
      setPositionX(pMousePos->getX() - offsetFromMouseX);
      setPositionY(pMousePos->getY() - offsetFromMouseY);
    }
  }
  //if not already holding an object
  else if (!TheInputHandler::Instance()->getIsHoldingObject())
  { //and if pointer is over object
    if (pMousePos->getX() < (position.getX() + radius) &&
        pMousePos->getX() > (position.getX() - radius) &&
        pMousePos->getY() > (position.getY() - radius) &&
        pMousePos->getY() < (position.getY() + radius)
        )
    {
      //if pointer is clicked - grab hold of the object
      if (TheInputHandler::Instance()->getIsMouseButtonPressed())   
      {
        setVelocity(0.0f,0.0f); //reset Velocity
        TheInputHandler::Instance()->setIsHoldingObject(true);
        TheInputHandler::Instance()->setIsMouseButtonPressed(false);      
        isHeldByMouse = true;
        arePhysicsEnabled = false;
        
        //calculate offset from mouse pos to object
        offsetFromMouseX = pMousePos->getX() - getPositionX();
        offsetFromMouseY = pMousePos->getY() - getPositionY();
      }
    }
  }
}

void Circle::clean() {}
