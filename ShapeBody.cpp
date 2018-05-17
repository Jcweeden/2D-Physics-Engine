#include "ShapeBody.h"


ShapeBody::ShapeBody(const LoaderParams* pParams) :
    GameObject(pParams), position(pParams->getX(), pParams->getY()), velocity(0,0), acceleration(0,0)
{
  // m_x = pParams->getX();
  // m_y = pParams->getY();

  width =  pParams->getWidth();
  height =  pParams->getHeight();

  textureID = pParams->getTextureID();

  currentRow = 1;
  currentFrame = 1;
}

void ShapeBody::draw() {

  if(velocity.getX() > 0) {
    TextureManager::Instance()->drawFrame(textureID, (int)position.getX(), (int)position.getY(), width, height, currentRow, currentFrame, TheGame::Instance()->getRenderer(), SDL_FLIP_HORIZONTAL);
  }
  else
  {
    TextureManager::Instance()->drawFrame(textureID, (int)position.getX(), (int)position.getY(), width, height, currentRow, currentFrame, TheGame::Instance()->getRenderer());
  }
}


void ShapeBody::update(){

  velocity += acceleration;
  position += velocity;

  
};
void ShapeBody::clean(){};
