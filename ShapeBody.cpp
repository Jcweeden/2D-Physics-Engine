#include "ShapeBody.h"


ShapeBody::ShapeBody(int p_x, int p_y,/* int p_rotation,*/ int p_density, Uint32 p_colour) :
    GameObject(), density(p_density), colour(p_colour), position(p_x, p_y), velocity(0,0), acceleration(0,0)
{
  rotation = 0;
}

void ShapeBody::draw() {}


void ShapeBody::update(){

  //TODO apply gravity
  
  velocity += acceleration;
  position += velocity;

  
};
void ShapeBody::clean(){};


