#ifndef _InputHandler_
#define _InputHandler_
#include "SDL2/SDL.h"
#include "Vector2D.h"
#include <iostream>
#include <vector> 

enum mouse_buttons
{
  LEFT = 0,
  MIDDLE = 1,
  RIGHT = 2
};


class InputHandler {
public:
  static InputHandler* Instance()
  {
    if(s_pInstance ==0)
    {
      s_pInstance = new InputHandler();
    }

    return s_pInstance;

  }

  void update();
  void clean();//will clear any initialized devices

  void updateJoystickInput(SDL_Event event);//uses event to determine the controller input

  std::vector<SDL_Joystick*> m_joysticks;//pointers needed to close connection in clean
  bool m_bJoysticksInitialised;

  void initialiseJoysticks();
  bool joysticksInitialised() {
    return m_bJoysticksInitialised;
  }

  int yvalue(int joy, int stick);//joy is ID of stick we want to use, 1 for left, 2 for right
  int xvalue(int joy, int stick);

  //returns whether controller and button number is being pressed
  bool getButtonState(int joy, int buttonNumber)
  {
    return m_buttonStates[joy][buttonNumber];
  }

  //returns whether a button is being pressed or not
  bool getMouseButtonState(int buttonNumber)
  {
    return m_mouseButtonStates[buttonNumber];
  }

  Vector2D* getMousePosition()
  {
    //std::cout << "getMousePosition(): x: " << m_mousePosition->getX() << " y: " << m_mousePosition->getY() << std::endl; 
    return m_mousePosition;
  }

  //returns true if parameter key is pressed
  bool isKeyDown(SDL_Scancode key);

  void reset();
  
private:

  InputHandler();
  ~InputHandler() {}
  
  static InputHandler* s_pInstance;

  const int m_joystickDeadzone = 10000;
  
  std::vector<std::pair<Vector2D*, Vector2D*> > m_joystickValues;
  //pair, one for each stick - Vector2D will contains movements of stick

  
  std::vector<std::vector<bool> > m_buttonStates;
  //a list of buttons for each controller, true if pressed


  std::vector<bool> m_mouseButtonStates;


  Vector2D* m_mousePosition;

  const Uint8* m_keystates;

};

typedef InputHandler TheInputHandler;

#endif
