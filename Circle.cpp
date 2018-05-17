#include "Circle.h"
#include "SDL2/SDL2_gfxPrimitives.h" 


Circle::Circle(int p_x, int p_y, float p_radius, /* int p_rotation,*/ int p_density, Uint32 p_colour) :
    ShapeBody( p_x,  p_y,  /*p_rotation,*/  p_density, p_colour)
{
  //init other variables
  radius = p_radius;
  std::cout << "Circle.cpp: New Circle - radius: " << getRadius() << " - area: " << getArea() << "\n";
   
  area = getArea();
}


float Circle::getArea() {
  return pi * radius * radius;
}

void Circle::draw()
{  
  filledCircleColor(TheGame::Instance()->getRenderer(), getPositionX(), getPositionY(), radius, colour);
  //filledCircleColor(TheGame::Instance()->getRenderer(), 300, 450, 30, 0xff00ffcc); 

  //SDL_Delay(3000);
}

void Circle::update()
{
  ShapeBody::update();
}

void Circle::clean() {}
