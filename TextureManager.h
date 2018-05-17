#ifndef _TextureManager_
#define _TextureManager_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>
#include <map>

class TextureManager {

public:

    static TextureManager* Instance()
  {
    if(s_pInstance == 0)
    {
      s_pInstance = new TextureManager();
      return s_pInstance;
    }

    return s_pInstance;
  }

  bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);

  void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
  void drawFrame(std::string id, int x, int y, int width, int height, int currentRow, int currentFrame, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

  //using ID removes texture from current state, not the entire textureMap
  void clearFromTextureMap(std::string id);

  
  // map of pointers to SDL_Texture objects, keyed using strings
  std::map<std::string, SDL_Texture*> m_textureMap;



private:
  // private constructor for singleton - can only be created by Instance() function
  TextureManager () {};


  static TextureManager* s_pInstance;


};


typedef TextureManager TheTextureManager;


#endif
