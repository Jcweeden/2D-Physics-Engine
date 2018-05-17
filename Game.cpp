#include "Game.h"
#include <iostream>


//define static instance
Game* Game::s_pInstance = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
  int flags = 0;

  if (fullscreen) {//if method parameter = true then make variable in line below
    flags = SDL_WINDOW_SHOWN;
  }
  
  // initialise SDL
   if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
   {
     std::cout << "Game: SDL init success\n";
     // if succeeded create our window
     m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
 
 // if the window creation succeeded create our renderer
     if(m_pWindow != 0)
     {
       std::cout << "Game: Window creation success\n";
       m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

       if (m_pRenderer != 0) {
         std::cout << "Game: Renderer creation success\n";
         SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
       }
       else {
         std::cout << "Game: Renderer init failed\n";
         return false;
       }
     }
     else
     {
       std::cout << "Game: Window init failed\n";
       return false;
     }
   }
   else
   {
     std::cout << "Game: SDL init failed\n";
     return false; // sdl could not initialise
   }

   std::cout << "Game: Init success\n";
   m_bRunning = true;//start the main loop


   LoadTextures();
   LoadObjects();

   TheInputHandler::Instance()->initialiseJoysticks();


   
   
   return true;
}

//create and load objects
void Game::LoadObjects() {

  //create
  // m_go = new GameObject();
  // m_player = new Player();
  // m_enemy = new Enemy();

  // //load
  // m_go->load(100, 100, 128, 82, "animate");
  // m_player->load(300, 300, 128, 82, "animate");
  // m_enemy->load(0,0,128,82,"animate");

  // //add to GameObject* vector
  // m_gameObjects.push_back(m_go);
  // m_gameObjects.push_back(m_player);
  // m_gameObjects.push_back(m_enemy);


  //m_gameObjects.push_back(new Enemy(new LoaderParams(300,300,128,82,"animate")));
}

//put textures in memory
 void Game::LoadTextures() {
   //TheTextureManager::Instance()->load("assets/animate-alpha.png", "animate",m_pRenderer);
 }

void Game::render()
{
  SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour

  //loop through objects and draw them
  // for(size_t i = 0; i < m_gameObjects.size(); i++)
  // {
  //   m_gameObjects[i]->draw(/*m_pRenderer*/);
  // }
  
  SDL_RenderPresent(m_pRenderer);  //draw to the screen
}

void Game::update() {

  //loop through and update our objects
  // for(size_t i = 0; i < m_gameObjects.size(); i++)
  // {
  //   m_gameObjects[i]->update();
  // }

}

void Game::clean() {
  std::cout << "Game: cleaning game\n";

  TheInputHandler::Instance()->clean();//remove any controller connections

  
  SDL_DestroyWindow (m_pWindow);
  SDL_DestroyRenderer (m_pRenderer);
  std::cout << "Game: SDL_Quit()\n\n";
  SDL_Quit();
}

void Game::handleEvents()
{
  TheInputHandler::Instance()->update();

  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
  {
    
  }
}

void Game::Quit() {
  m_bRunning = false;
}
