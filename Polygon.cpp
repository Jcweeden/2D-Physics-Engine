#include "Polygon.h"

#include "SDL2/SDL2_gfxPrimitives.h" 


Polygon::Polygon(int p_x, int p_y,/* int p_rotation,*/ int p_mass, std::vector<Sint16> p_verticesX, std::vector<Sint16> p_verticesY, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA ) :
    ShapeBody( p_x,  p_y,  /*p_rotation,*/  p_mass, p_colourR,p_colourG,p_colourB,p_colourA), verticesX(p_verticesX), verticesY(p_verticesY)
{
  //init other variables

  std::cout << "Polygon.cpp: New Polygon - vertices: " << getVertexCount() << " - area: " << getArea() << "\n";
   
  area = getArea();
}

void Polygon::addVertex(Vector2D newVertex)
{
  verticesX.push_back(newVertex.getX());
  verticesY.push_back(newVertex.getY());
}

Vector2D Polygon::getVertex(int index)
{
  return Vector2D(verticesX[index], verticesY[index]);
}

float Polygon::getArea() {
  float area = 0;
  int numVertices = Polygon::getVertexCount();
  int prevVertexIndex = numVertices -1; //minus 1 to get last element in vector

  for (size_t i = 0; i < numVertices; i++)
  {
    area += (verticesX[prevVertexIndex] + verticesX[i]) * (verticesY[prevVertexIndex] - verticesY[i]);
    prevVertexIndex = i; //get prev vertex for next loop iteration
  }
  //going clockwise so result will be negative
  return -(area/2);
}

void Polygon::draw()
{
  std::vector<Sint16> vertPosX;
  std::vector<Sint16> vertPosY;

  for (size_t i = 0; i < verticesX.size(); i++)
  {
    vertPosX.push_back(verticesX[i]+getPositionX());    
    vertPosY.push_back(verticesY[i]+getPositionY());
  }
  
  filledPolygonRGBA (TheGame::Instance()->getRenderer(), &vertPosX[0], &vertPosY[0], getVertexCount(), colourR, colourG, colourB, colourA);	
  
  //circleColor(TheGame::Instance()->getRenderer(), 600, 450, 33, 0xff00ff00);
  //filledCircleColor(TheGame::Instance()->getRenderer(), 300, 450, 30, 0xff00ffcc); 

  //SDL_Delay(3000);
}

void Polygon::update()
{
  ShapeBody::update();
}

void Polygon::clean() {}

void Polygon::calculateCenterOfMass()
{
  int totalX = 0, totalY = 0;
  
  for (size_t i = 0 ; i < verticesX.size(); i++) {
    totalX += verticesX[i];
    totalY += verticesY[i];
  }
  
  Vector2D centerOfMass = Vector2D(totalX/getVertexCount(), totalY/getVertexCount());
}
