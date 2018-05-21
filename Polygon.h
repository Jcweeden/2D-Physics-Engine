#ifndef _Polygon_
#define _Polygon_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "ShapeBody.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include <iostream>
#include <string>
#include <vector>

class Polygon : public ShapeBody {
public:

  Polygon(int p_x, int p_y,/* int p_rotation,*/ int p_mass, std::vector<Sint16> verticesX, std::vector<Sint16> verticesY, Uint8 p_colourR, Uint8 p_colourG, Uint8 p_colourB, Uint8 p_colourA );
  
  virtual void draw();
  virtual void update();
  virtual void clean();
  
  void addVertex(Vector2D newVertex);
  Vector2D getVertex(int index);
  
  float getArea(); //calculate and return polygon area
  int getVertexCount() { return verticesX.size(); } //return num of vertices in polygon

  
protected:
  std::vector<Sint16> verticesX; //stored in clockwise order
  std::vector<Sint16> verticesY; //stored in clockwise order

  void checkIfHeldByMouse();
  
  void calculateCenterOfMass();
  void findMinMaxXY();
  int minX, maxX, minY, maxY;
};

#endif
