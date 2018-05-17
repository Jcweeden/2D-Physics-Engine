#ifndef _Game_
#define _Game_
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "GameObject.h"


//#include "Player.h"
//#include "Enemy.h"
#include "LoaderParams.h"
#include <vector>

class GameObject;

class Game {

public:

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

  void LoadTextures();//remove
  void LoadObjects();
  void Quit();
  //void DelayedClose(int ms) {SDL_Delay(ms); SDL_Quit();}
  
  //a function to access the private running variable
  bool running() {return m_bRunning;}

  int m_currentFrame;//can delete?
  
  std::vector<GameObject*> m_gameObjects;

  //function to return the renderer
  SDL_Renderer* getRenderer() const { return m_pRenderer; }

  
private:

  Game() {}

  //static instance variable
  static Game* s_pInstance;
  
  SDL_Window* m_pWindow;
  SDL_Renderer* m_pRenderer;


  
  //TextureManager m_textureManager;
    
  bool m_bRunning;

};

typedef Game TheGame;

#endif /*defined (_Game_)*/
