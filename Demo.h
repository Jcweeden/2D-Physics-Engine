#ifndef _Demo_
#define _Demo_

class Demo /*: public Demo*/
{
public:
  virtual void draw() = 0;

  virtual void update() = 0;

  virtual void clean() = 0;

  virtual void reset() = 0;

  virtual void handleInput() = 0;
};

#endif
