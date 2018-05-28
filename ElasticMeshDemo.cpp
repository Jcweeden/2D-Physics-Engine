#include "ElasticMeshDemo.h"

#include <vector>

ElasticMeshDemo::ElasticMeshDemo()
    :
    shapesCount(144), //must be square num - 4,9,16,25,36,49
    shapeRadius(5),
    world (//1000, 1000
        shapesCount + ((sqrt(shapesCount)-1) * ((2*(sqrt(shapesCount) - 1)) + 2)),
           shapesCount  //same as shapesCount
           )
{
  //calculate number of cables required to connect all shapes
  int n = sqrt(shapesCount) - 1; //sqrt -1
  cablesCount = n * ((2*n) +2);
  //std::cout << "cablesCount: " << cablesCount << "\n";

  

  
  Sint16 winWidth = TheGame::Instance()->getWindowWidth();
  Sint16 winHeight = TheGame::Instance()->getWindowHeight();

  int percentageBorderWidth = 50;
  
  float borderX = ((winWidth / 100) * (percentageBorderWidth/2));
  float widthPerX = ((winWidth / 100) * (100-percentageBorderWidth))
      / 100 * (100 / (sqrt(shapesCount)-1));
  
  float borderY = ((winHeight / 100) * (percentageBorderWidth/2));
  float heightPerY = ((winHeight / 100) * (100-percentageBorderWidth))
      / 100 * (100 / (sqrt(shapesCount)-1));
  
  shapes = new Circle[shapesCount];


  //create and place circles
  for (size_t i = 0; i < shapesCount; i++)
  {

    int x = borderX + (((i % (int)sqrt(shapesCount)) * widthPerX));
    int y = borderY + (int)(i / sqrt(shapesCount)) * heightPerY;

    shapes[i].setPosition(Vector2D(x, y));
    shapes[i].setRadius(shapeRadius);

  
    shapes[i].setVelocity(0,0);
    shapes[i].setDamping(0.9f);
    shapes[i].setMass(1.0f);
    shapes[i].clearAccumForces();

    
    //each side pulling away - shows maximum stetch
    /*
    if (i % (int)sqrt(shapesCount) == 0) {
      shapes[i].setAcceleration(-0.1f, 0.0f); //no gravity
    }
    else if ( i % (int)sqrt(shapesCount) == (int)sqrt(shapesCount) -1)
    {
      shapes[i].setAcceleration(0.1f, 0.0f); //no gravity
    }
    */
    /////

    if (i > (int)shapesCount / 2)
    {
      if (i % (int)sqrt(shapesCount) == 0) {
        shapes[i].setAcceleration(-0.4f, 0.0f);
      }
    }
    else if(i < (int)shapesCount / 2)
    {
      if (i % (int)sqrt(shapesCount) == 0)
      {
        shapes[i].setAcceleration(0.4, 0.0f);
      }
    }
  }

  //create and link circles in cables
  cables = new ShapeCable[cablesCount];

  //create horizontal cable links
  int cableNum = 0;
  for (size_t i = 0; i < sqrt(shapesCount); i++)
  {
    for (size_t j = 0; j < sqrt(shapesCount)-1; j++)
    {
      cables[cableNum].linkedShapes[0] = &shapes[(int)(i*sqrt(shapesCount)+j)];
      cables[cableNum].linkedShapes[1] = &shapes[(int)(i*sqrt(shapesCount)+j)+1];

      
      cables[cableNum].cableMaxLength = widthPerX * 1.2;
      cables[cableNum].restitution = 0.0f;
      world.getContactGenerators().push_back(cables + cableNum);
      cableNum++;
      //std::cout << cableNum << "\n";     
    }
  }

   //create vertical cable links
  for (size_t i = 0; i < sqrt(shapesCount)-1; i++)
  {
    for (size_t j = 0; j < sqrt(shapesCount); j++)
    {
      cables[cableNum].linkedShapes[0] = &shapes[(int)(i*sqrt(shapesCount)+j)];
      cables[cableNum].linkedShapes[1] = &shapes[(int)(i*sqrt(shapesCount)+j)+(int)sqrt(shapesCount)];
      
      cables[cableNum].cableMaxLength = widthPerX * 1.2;
      cables[cableNum].restitution = 0.0f;
      world.getContactGenerators().push_back(cables + cableNum);
      cableNum++;
      //std::cout << cableNum << "\n";    
    }
  }
}


ElasticMeshDemo::~ElasticMeshDemo()
{}


void ElasticMeshDemo::reset()
{}



void ElasticMeshDemo::draw()
{  
  //draw horizontal lines
  for (size_t i = 0; i < sqrt(shapesCount); i++)
  {
    for (size_t j = 0; j < sqrt(shapesCount)-1; j++)
    {
      //draw line from shapes[(i*sqrt(shapesCount)+j] to +1
      aalineRGBA(TheGame::Instance()->getRenderer(),
                 shapes[(int)(i*sqrt(shapesCount)+j)].getPositionX(),
                 shapes[(int)(i*sqrt(shapesCount)+j)].getPositionY(),
                 shapes[(int)(i*sqrt(shapesCount)+j)+1].getPositionX(),
                 shapes[(int)(i*sqrt(shapesCount)+j)+1].getPositionY(),
                 0,0,0,255);
    }
  }

  //draw vertical lines
  for (size_t i = 0; i < sqrt(shapesCount)-1; i++)
  {
    for (size_t j = 0; j < sqrt(shapesCount); j++)
    {
      aalineRGBA(TheGame::Instance()->getRenderer(),
                 shapes[(int)(i*sqrt(shapesCount)+j)].getPositionX(),
                 shapes[(int)(i*sqrt(shapesCount)+j)].getPositionY(),
                 shapes[(int)(i*sqrt(shapesCount)+j)+(int)sqrt(shapesCount)].getPositionX(),
                 shapes[(int)(i*sqrt(shapesCount)+j)+(int)sqrt(shapesCount)].getPositionY(),
                 0,0,0,255);

    }
  }

  //draw shapes from their base class
  for (size_t i = 0 ; i < shapesCount ; i++)
  {
    shapes[i].draw();
  }
}

void ElasticMeshDemo::update()
{

  //remove accumu forces from previous frame for each obj
  world.startFrame();

  //get duration of the last frame
  float duration = TheGame::Instance()->getFrameTime() * 0.01f;
  if (duration <= 0.0f) return; 
  
  
  //draw shapes from their base class - updates if an object is being held by mouse
  for (size_t i = 0 ; i < shapesCount ; i++)
  {
    shapes[i].update();
  }
      
  //run simulation
  world.runPhysics(duration);
}

void ElasticMeshDemo::clean()
{
  delete[] shapes;
  delete[] cables;
}


void ElasticMeshDemo::handleInput()
{

}
