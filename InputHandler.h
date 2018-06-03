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


  //returns whether a button is being pressed or not
  bool getMouseButtonState(int buttonNumber)
  {
    return m_mouseButtonStates[buttonNumber];
  }

  Vector2D* getMousePosition()
  {
    return m_mousePosition;
  }

  bool getIsMouseButtonPressed()
  {
    return isMouseButtonPressed;
  }
  void setIsMouseButtonPressed(bool val)
  {
    isMouseButtonPressed = val;
  }

  
  //returns true if parameter key is pressed
  bool isKeyDown(SDL_Scancode key);

  //sets all mouse button presses to false
  void reset();

  void setIsHoldingObject(bool val) { isHoldingObject = val; }
  bool getIsHoldingObject() { return isHoldingObject; }


  bool keyReturnPressed,
      key1Pressed, key2Pressed, key3Pressed, key4Pressed,
      keyBPressed, keyWPressed, //Demo02
      keyQPressed, /*keyWPressed,*/ keyEPressed, keyRPressed, keyNPressed; //used in Demo03
  
private:

  InputHandler();
  ~InputHandler() {}

  static InputHandler* s_pInstance;

  bool isMouseButtonPressed;
  bool isHoldingObject;
  
  std::vector<std::vector<bool> > m_buttonStates;
  //a list of buttons for each controller, true if pressed


  std::vector<bool> m_mouseButtonStates;


  Vector2D* m_mousePosition;

  const Uint8* m_keystates;

};

typedef InputHandler TheInputHandler;

#endif
