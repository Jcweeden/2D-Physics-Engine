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

  Polygon(int p_x, int p_y,/* int p_rotation, int p_width, int p_height,*/ int p_density, Uint32 p_colour, std::vector<Sint16> verticesX, std::vector<Sint16> verticesY );
  
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

  void calculateCenterOfMass();

};

#endif
