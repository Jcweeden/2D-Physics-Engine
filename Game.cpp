#include "Game.h"
#include <iostream>
#include "SDL2/SDL2_gfxPrimitives.h"


//DEMOS
#include "BlobDemo.h"
#include "AnchorSpringDemo.h"
#include "ElasticMeshDemo.h"


//define static instance
Game* Game::s_pInstance = 0;

bool Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
  int flags = 0;

  if (fullscreen) {
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
       
       windowWidth = width; windowHeight = height;
       
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

   m_bRunning = true;//start the main loops
   std::cout << "Game: Init success\n";
   

   frameTime = 0;

   demo = new AnchorSpringDemo;
   //demo = new BlobDemo;
   //demo = new ElasticMeshDemo;
   
   return true;
}


Uint32 Game::getFrameTime() { return frameTime; }

void Game::setFrameTime(Uint32 val) { frameTime = val; }
 
void Game::render()
{
  SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 0xFF); 
  SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour

  demo->draw();
  
  SDL_RenderPresent(m_pRenderer);  //draw to the screen
}

void Game::update() {
  demo->update();
}

void Game::clean() {
  std::cout << "Game: cleaning game\n";

  //clean up the currently open demo
  demo->clean();

  //deallocate the demo pointer itself
  delete demo;

  SDL_DestroyWindow (m_pWindow);
  SDL_DestroyRenderer (m_pRenderer);
  std::cout << "Game: SDL_Quit()\n\n";
  SDL_Quit();
}

void Game::handleEvents()
{
  
  TheInputHandler::Instance()->update();


  demo->handleInput();
  
  //press Enter to restart the current demo
  if (TheInputHandler::Instance()->keyReturnPressed)
  {
    TheInputHandler::Instance()->keyReturnPressed = false;
    std::cout << "Reset Demo\n";
    demo->reset(); 
  }
  else if (TheInputHandler::Instance()->key1Pressed)
  {
    TheInputHandler::Instance()->key1Pressed = false;
    
    demo->clean();
    delete demo;
    
    std::cout << "Demo01 opened\n";
    demo = new AnchorSpringDemo;
  }
  else if (TheInputHandler::Instance()->key2Pressed)
  {
    std::cout << "Demo02 opened\n";
    TheInputHandler::Instance()->key2Pressed = false;
    demo->clean();
    delete demo;

    demo = new BlobDemo;
  }
  else if (TheInputHandler::Instance()->key3Pressed)
  {
    std::cout << "Demo03 opened\n";    
    TheInputHandler::Instance()->key3Pressed = false;
    demo->clean();
    delete demo;

    demo = new ElasticMeshDemo;
  }
}

void Game::quit() {
  m_bRunning = false;
}
