#include "InputHandler.h"
#include "Game.h"


//define static instance
InputHandler* InputHandler::s_pInstance = 0;


InputHandler::InputHandler()
{
  isHoldingObject = false;
  keyReturnPressed = key1Pressed = key2Pressed = key3Pressed = key4Pressed =
      keyBPressed =
      keyQPressed = keyWPressed = keyEPressed = keyRPressed = keyNPressed =
      false;

  
  //MOUSE
  for (size_t i = 0; i < 3; i++)
  {
    m_mouseButtonStates.push_back(false);
  }
  m_mousePosition = new Vector2D(0,0);
    
  isMouseButtonPressed = false;
}

void InputHandler::clean()
{}

//called each frame to update event state
void InputHandler::update()
{
  isMouseButtonPressed = false;
  //isMouseButtonReleased = false;
  
  for (size_t i = 0; i < 3; i++)
  {
    m_mouseButtonStates[i] = false;
  }
  
  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    m_keystates = SDL_GetKeyboardState(0);//update presssed keys

    //QUIT
    if (event.type == SDL_QUIT)
    {
      std::cout << "InputHandler: Quit event\n";
      TheGame::Instance()->quit();
    }
    
    //MOUSE BUTTONS
    if(event.type == SDL_MOUSEBUTTONDOWN)
    {
      if (event.button.button == SDL_BUTTON_LEFT)
      {
        m_mouseButtonStates[LEFT] = true;
        isMouseButtonPressed = true;
      }
      /*if (event.button.button == SDL_BUTTON_MIDDLE)
      {
        m_mouseButtonStates[MIDDLE] = true;
      }*/
      if (event.button.button == SDL_BUTTON_RIGHT)
      {
        m_mouseButtonStates[RIGHT] = true;
        }
    }
    if (event.type == SDL_MOUSEBUTTONUP)
    {
      if (event.button.button == SDL_BUTTON_LEFT)
      {
        //m_mouseButtonStates[LEFT] = false;
        isMouseButtonPressed = false;        
      }/*
      if (event.button.button == SDL_BUTTON_MIDDLE)
      {
        m_mouseButtonStates[MIDDLE] = false;
      }
      if (event.button.button == SDL_BUTTON_RIGHT)
      {
        m_mouseButtonStates[RIGHT] = false;
      }*/
    }

    //MOUSE MOTION
    if(event.type == SDL_MOUSEMOTION)
    {
      m_mousePosition->setX((float)event.motion.x);
      m_mousePosition->setY((float)event.motion.y);
    }

    //KEYBOARD
    if(event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
        case SDLK_RETURN:
          keyReturnPressed = true;          
          break;
        case SDLK_1:
          key1Pressed = true;
          break;
        case SDLK_2:
          key2Pressed = true;
          break;
        case SDLK_3:
          key3Pressed = true;          
          break;
        case SDLK_4:
          key4Pressed = true;          
          break;
        case SDLK_q:
          keyQPressed = true;
          break;
        case SDLK_w:
          keyWPressed = true;
          break;
        case SDLK_e:
          keyEPressed = true;          
          break;
        case SDLK_r:
          keyRPressed = true;          
          break;
        case SDLK_n:
          keyNPressed = true;          
          break;
        case SDLK_b:
          keyBPressed = true;          
          break;
      
      }
    }
  }
}



//returns true is passed in key is down
bool InputHandler::isKeyDown(SDL_Scancode key)
{
  if (m_keystates != 0)
  {
    if (m_keystates[key] == 1) {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}

void InputHandler::reset()
{
  m_mouseButtonStates[LEFT] = false;
  m_mouseButtonStates[MIDDLE] = false;
  m_mouseButtonStates[RIGHT] = false;
}
