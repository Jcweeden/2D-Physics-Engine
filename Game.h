#ifndef _Game_
#define _Game_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
//#include "InputHandler.h"
//#include "TextureManager.h"
//#include "GameObject.h"
#include "Demo.h"

//#include "LoaderParams.h"
#include <vector>

class GameObject;

class Game {

public:

  //singleton for Game
  static Game* Instance()
  {
    if(s_pInstance == 0)
    {
      s_pInstance = new Game();
      return s_pInstance;
    }

    return s_pInstance;
  }

  
  ~Game() {}

  // simply set the running variable to true
  bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
  void render();
  void update();
  void handleEvents();
  void clean();

  //void LoadTextures();//remove
  //void LoadObjects();
  void quit();
  //void DelayedClose(int ms) {SDL_Delay(ms); SDL_Quit();}
  
  //a function to access the private running variable
  bool isRunning() {return m_bRunning;}

  //int m_currentFrame;//can delete?
  
  //std::vector<GameObject*> m_gameObjects;

  //function to return the renderer
  SDL_Renderer* getRenderer() const { return m_pRenderer; }

  int getWindowWidth() { return windowWidth; }
  int getWindowHeight() { return windowHeight; }

  //the time it took to render the last frame
  Uint32 getFrameTime();
  void setFrameTime(Uint32 val);

  //the currently running physics demo
  Demo* demo;

  
private:

  Game() {}

  //static instance variable
  static Game* s_pInstance;

  Uint32 frameTime;

  
  SDL_Window* m_pWindow;
  SDL_Renderer* m_pRenderer;

  int windowWidth;
  int windowHeight;
      
  bool m_bRunning;

};

typedef Game TheGame;

#endif /*defined (_Game_)*/
