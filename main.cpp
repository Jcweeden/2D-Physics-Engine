#include <SDL2/SDL.h>
#include "Game.h"

#include <iostream>

Game* g_game = 0;

const int FPS = 30; //how many FPS we want to run at
const int DELAY_TIME = 1000.0f / FPS;//divided by num of ms in a second, giving time to delay game between loops to keep a constant frame ratex
Uint32 lastFrame = SDL_GetTicks();
 
int main(int argc, char* argv[])
 {
   Uint32 frameStart, frameTime;
   
   std::cout << "\n\nMain: game init attempt..\n";

   if(TheGame::Instance()->init("2D RigidBody Physics Engine", 100, 100, 600, 600, true)) {
   std::cout << "Main: game init successful\n";

   while (TheGame::Instance()->isRunning())
     {
       frameStart = SDL_GetTicks(); // get time at start of frame (num ms since calling SDL_Init)
       TheGame::Instance()->setFrameTime(frameStart-lastFrame);
       lastFrame = frameStart;
           
       TheGame::Instance()->handleEvents();
       TheGame::Instance()->update();
       TheGame::Instance()->render();

       frameTime = ( SDL_GetTicks() - frameStart);//how long the frame took to run

       if(frameTime < DELAY_TIME)//if time taken to run frame is less than desired
       {
         SDL_Delay((int)(DELAY_TIME - frameTime));//call delay to make wait for time to reach desired FPS
       }
     }
   }
   else
   {
     std::cout << "Main: game init failure - " << SDL_GetError() << "\n";
     return -1;
   }
  
   std::cout << "Main: game closing\n";
   TheGame::Instance()->clean();

   return 0;
 }
