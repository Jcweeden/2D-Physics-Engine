#include "Game.h"
#include <iostream>
#include "Polygon.h"
#include "Circle.h"


enum COLOURS
{
  RED = 0x000000FF,
  GREEN = 0x00ff00cc,
  BLUE = 0x0000ffcc, //does not work
  WHITE = 0xffffffcc,
  YELLOW = 0xffff00cc, //purple
  PURPLE = 0xff00ffcc,
  CYAN = 0x00ffffcc
};

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

   std::cout << "Game: Init success\n";
   m_bRunning = true;//start the main loop

   frameTime  = 0;
       
   LoadTextures();
   LoadObjects();

   TheInputHandler::Instance()->initialiseJoysticks();


   
   
   return true;
}


Uint32 Game::getFrameTime() { return frameTime; }

void Game::setFrameTime(Uint32 val) { frameTime = val; }


//create and load objects
void Game::LoadObjects() {

  std::vector<Sint16>  polygonTestX{0, 20, 20, 0};
  std::vector<Sint16>  polygonTestY{0, 0, 20, 20};
  
  Polygon* m_polygonTest = new Polygon(150,50, 1, polygonTestX, polygonTestY, 255,0,0,255);
  m_polygonTest->setMass(1.0f);
  m_polygonTest->setVelocity(15.0f, 0.0f);
  m_polygonTest->setAcceleration(0.0f, 0.1f);
  m_polygonTest->setDamping(0.99f);

  
  m_gameObjects.push_back(m_polygonTest);

  
  Polygon* m_polygonTest1 = new Polygon(150,50, 1, polygonTestX, polygonTestY, 0,255,0,255);
  m_polygonTest1->setMass(1.0f);
  m_polygonTest1->setVelocity(15.0f, 0.0f);
  m_polygonTest1->setAcceleration(0.0f, 0.1f);
  m_polygonTest1->setDamping(0.97f);
  
  
  m_gameObjects.push_back(m_polygonTest1);

  //Circle* m_circleTest = new Circle(450,450, 20, 1, 0xffffffcc);

  //m_gameObjects.push_back(m_circleTest);
}

//put textures in memory
 void Game::LoadTextures() {
   //TheTextureManager::Instance()->load("assets/animate-alpha.png", "animate",m_pRenderer);
 }

void Game::render()
{
  SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0xFF); 
  SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour

  //loop through objects and draw them
  for(size_t i = 0; i < m_gameObjects.size(); i++)
  {
    m_gameObjects[i]->draw(/*m_pRenderer*/);
  }
  
  SDL_RenderPresent(m_pRenderer);  //draw to the screen
}

void Game::update() {

  //loop through and update our objects
  for(size_t i = 0; i < m_gameObjects.size(); i++)
  {
    m_gameObjects[i]->update();
  }

}

void Game::clean() {
  std::cout << "Game: cleaning game\n";

  TheInputHandler::Instance()->clean();//remove any controller connections

  // for(size_t i = 0; i < m_gameObjects.size(); i++)
  // {
  //   delete m_gameObjects[i];
  // }

  m_gameObjects.clear();
  
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
