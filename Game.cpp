#include "Game.h"
#include <iostream>
//#include <stdlib.h>     /* srand, rand */
//#include <time.h>
#include "SDL2/SDL2_gfxPrimitives.h"


//DEMOS
#include "BlobDemo.h"
#include "AnchorSpringDemo.h"

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
   // demo = new BlobDemo;
   
   return true;
}


Uint32 Game::getFrameTime() { return frameTime; }

void Game::setFrameTime(Uint32 val) { frameTime = val; }


//create and load objects
//void Game::LoadObjects() {

  //BUOYANCY
  /*
  const Vector2D grav (0.0f,1.0f);
    
  std::vector<Sint16>  polygonTestX{0, 20, 20, 0};
  std::vector<Sint16>  polygonTestY{0, 0, 20, 20};
  
  Polygon* m_polygonTest = new Polygon(200,400, 1, polygonTestX, polygonTestY, 255,0,0,255);
  m_polygonTest->setMass(1.0f);
  m_polygonTest->setVelocity(0.0f, 0.0f);
  m_polygonTest->setAcceleration(0.0f, 0.0f);
  m_polygonTest->setDamping(0.95f);

  //m_gameObjects.push_back(m_polygonTest);

  ShapeGravity* sg1 = new ShapeGravity(grav);
  registry.add(m_polygonTest, sg1);

  ShapeBuoyancy* sb1 = new ShapeBuoyancy(20, 10, 300, 0.2);
  registry.add(m_polygonTest, sb1);
*/
  /*
  Circle* m_circleTest = new Circle(400,400, 20, 1, 0,0,255,255);
  m_circleTest->setMass(1.0f);
  m_circleTest->setVelocity(0.0f, 0.0f);
  m_circleTest->setAcceleration(0.0f, 0.0f); //grav set in forceGen
  m_circleTest->setDamping(0.95f);
  
  //m_gameObjects.push_back(m_circleTest);

  ShapeGravity* sg2 = new ShapeGravity(grav);
  registry.add(m_circleTest, sg2);

  ShapeBuoyancy* sb2 = new ShapeBuoyancy(20, 10, 300, 0.2);
  registry.add(m_circleTest, sb2);
  
  //ENDBUOYANCE
  
  //basic spring
  //ShapeSpring* ssA = new ShapeSpring(m_circleTest, 0.5f, 100.0f);
  //registry.add(m_polygonTest, ssA);

  //ShapeSpring* ssB = new ShapeSpring(m_polygonTest, 0.5f, 100.0f);
  //registry.add(m_circleTest, ssB);

  */
//}

void Game::render()
{
  SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 0xFF); 
  SDL_RenderClear(m_pRenderer); // clear the renderer to the draw colour

  //loop through objects and draw them
  /* for(size_t i = 0; i < m_gameObjects.size(); i++)
  {
    m_gameObjects[i]->draw();
    }*/

  //BUOYANCY
  //aalineRGBA(m_pRenderer, 0,300, 800,300, 0,0,255,255); // water line
  //ENDBUOYANCY

  demo->draw();
  
  SDL_RenderPresent(m_pRenderer);  //draw to the screen
}

void Game::update() {
  demo->update();
}

void Game::clean() {
  std::cout << "Game: cleaning game\n";

  //  TheInputHandler::Instance()->clean(); //remove any controller connections

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

  //press Enter to restart the current demo
  if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_RETURN))
  {
    demo->reset(); 
  }
}

void Game::quit() {
  m_bRunning = false;
}
