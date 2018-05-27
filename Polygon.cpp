#include "Polygon.h"

#include "SDL2/SDL2_gfxPrimitives.h" 


Polygon::Polygon(int p_x, int p_y,/* int p_rotation,*/ int p_mass, std::vector<Sint16> p_verticesX, std::vector<Sint16> p_verticesY, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA ) :
    ShapeBody( p_x,  p_y,  /*p_rotation,*/  p_mass, p_colourR,p_colourG,p_colourB,p_colourA), verticesX(p_verticesX), verticesY(p_verticesY)
{

  std::cout << "Polygon.cpp: New Polygon - vertices: " << getVertexCount() << " - area: " << getArea() << "\n";
  findMinMaxXY();
  area = getArea();
}

Polygon::Polygon() :
    ShapeBody( 200,  400,  /*p_rotation,*/  1, 255,0,0,255), verticesX(std::vector<Sint16> {0, 20, 20, 0}), verticesY(std::vector<Sint16> {0, 0, 20, 20})
{

  std::cout << "Polygon.cpp: New Polygon - vertices: " << getVertexCount() << " - area: " << getArea() << "\n";
  findMinMaxXY();
  area = getArea();
}

void Polygon::findMinMaxXY()
{
  minX = maxX = verticesX[0];
  minY = maxY = verticesY[0];

  for (size_t i = 1; i < verticesX.size(); i++ )
  {
    if (verticesX[i] > maxX) maxX = verticesX[i];
    if (verticesX[i] < minX) minX = verticesX[i];
    if (verticesY[i] > maxY) maxY = verticesY[i];
    if (verticesY[i] < minY) minY = verticesY[i];
  }
  
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
}

void Polygon::update()
{
  //checks and updates if object is being held by the mouse
  checkIfHeldByMouse();

  
  //updates physics
  ShapeBody::update();
}

void Polygon::checkIfHeldByMouse()
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
    if (pMousePos->getX() < (position.getX() + (maxX - minX) /*WIDTH*/) &&
        pMousePos->getX() > position.getX() &&
        pMousePos->getY() > position.getY() &&
        pMousePos->getY() < position.getY() + (maxY - minY) /*HEIGHT*/
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
