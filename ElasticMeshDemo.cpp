#include "ElasticMeshDemo.h"


ElasticMeshDemo::ElasticMeshDemo()
    :
    meshNodesCount(144), //must be square num - 4,9,16,25,36,49
    nodeRadius(5),
    world (
        //number of contacts handled per frame = number of nodes + number of cables
        meshNodesCount + ((sqrt(meshNodesCount)-1) * ((2*(sqrt(meshNodesCount) - 1)) + 2))
           )
{
  //calculate number of cables required to connect all meshNodes
  int n = sqrt(meshNodesCount) - 1; //sqrt -1
  cablesCount = n * ((2*n) +2);
  //std::cout << "cablesCount: " << cablesCount << "\n";

  //create array of nodes
  meshNodes = new Circle[meshNodesCount];

  //place nodes in correct positions
  float distanceBetweenNodes = placeNodes();
  
  //create array of cables to link nodes together
  cables = new ShapeCable[cablesCount];

  //connect nodes with cables
  connectNodesWithCables(distanceBetweenNodes);
}


ElasticMeshDemo::~ElasticMeshDemo()
{}


void ElasticMeshDemo::draw()
{


  /*
  //draw horizontal lines between nodes
  for (size_t i = 0; i < sqrt(meshNodesCount); i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount)-1; j++)
    {
      //draw line from meshNodes[(i*sqrt(meshNodesCount)+j] to +1 (the node next to it horizontally)
      aalineRGBA(TheGame::Instance()->getRenderer(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)].getPositionX(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)].getPositionY(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)+1].getPositionX(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)+1].getPositionY(),
                 0,0,0,255);
    }
  }

  //draw vertical lines between nodes
  for (size_t i = 0; i < sqrt(meshNodesCount)-1; i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount); j++)
    {
      aalineRGBA(TheGame::Instance()->getRenderer(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)].getPositionX(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)].getPositionY(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)+(int)sqrt(meshNodesCount)].getPositionX(),
                 meshNodes[(int)(i*sqrt(meshNodesCount)+j)+(int)sqrt(meshNodesCount)].getPositionY(),
                 0,0,0,255);

    }
  }
*/
  //for each cable
  for (size_t i = 0; i < cablesCount; i++)
  { //if the cable has not snapped
    if (!cables[i].hasSnapped)
    {
      
      aalineRGBA(TheGame::Instance()->getRenderer(),
                 cables[i].linkedShapes[0]->getPositionX(),
                 cables[i].linkedShapes[0]->getPositionY(),
                 cables[i].linkedShapes[1]->getPositionX(),
                 cables[i].linkedShapes[1]->getPositionY(),
                 0,0,0,255);
    }
  }

  
  //draw meshNodes from their base class
  for (size_t i = 0 ; i < meshNodesCount ; i++)
  {
    meshNodes[i].draw();
  }
}


void ElasticMeshDemo::update()
{

  //remove accumu forces from previous frame for each node
  world.startFrame();

  //get duration of the last frame
  float duration = TheGame::Instance()->getFrameTime() * 0.01f;
  if (duration <= 0.0f) return; 
  
  
  //update meshNodes from their base class - updates if an object is being held by mouse
  for (size_t i = 0 ; i < meshNodesCount ; i++)
  {
    meshNodes[i].update();
  }
      
  //run simulation
  world.runPhysics(duration);
}


void ElasticMeshDemo::clean()
{
  delete[] meshNodes;
  delete[] cables;
}


void ElasticMeshDemo::reset()
{
  float widthPerX = placeNodes(); //reset the position and forces applied to the nodes
  connectNodesWithCables(widthPerX); //and reset the cable links between the nodes
}


void ElasticMeshDemo::handleInput()
{}


float ElasticMeshDemo::placeNodes()
{

  Sint16 winWidth = TheGame::Instance()->getWindowWidth();
  Sint16 winHeight = TheGame::Instance()->getWindowHeight();

  int percentageBorderWidth = 50;
  
  float borderX = ((winWidth / 100) * (percentageBorderWidth/2));
  float widthPerX = ((winWidth / 100) * (100-percentageBorderWidth))
      / 100 * (100 / (sqrt(meshNodesCount)-1));
  
  float borderY = ((winHeight / 100) * (percentageBorderWidth/2));
  float heightPerY = ((winHeight / 100) * (100-percentageBorderWidth))
      / 100 * (100 / (sqrt(meshNodesCount)-1));
  


  //create and place nodes
  for (size_t i = 0; i < meshNodesCount; i++)
  {

    //calculate x,y of node according to size of the border
    int x = borderX + (((i % (int)sqrt(meshNodesCount)) * widthPerX));
    int y = borderY + (int)(i / sqrt(meshNodesCount)) * heightPerY;

    meshNodes[i].setPosition(Vector2D(x, y));
    meshNodes[i].setRadius(nodeRadius);
    meshNodes[i].setVelocity(0,0);
    meshNodes[i].setDamping(0.9f);
    meshNodes[i].setMass(1.0f);
    meshNodes[i].clearAccumForces();

    //for leftmost nodes on top half of mesh - add acc to move right, underneath the mesh itself
    if (i > (int)meshNodesCount / 2)
    {
      if (i % (int)sqrt(meshNodesCount) == 0) {
        meshNodes[i].setAcceleration(-1.0f, 0.0f);
      }
    }//for leftmost nodes on bottom half of mesh - add acc to move left, away from the mesh itself
    else if(i < (int)meshNodesCount / 2)
    {
      if (i % (int)sqrt(meshNodesCount) == 0)
      {
        meshNodes[i].setAcceleration(1.0, 0.0f);
      }
    }
  }

  return widthPerX;
}

void ElasticMeshDemo::connectNodesWithCables(float distanceBetweenNodes)
{
  //create horizontal cable links
  int cableNum = 0;
  for (size_t i = 0; i < sqrt(meshNodesCount); i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount)-1; j++)
    {
      cables[cableNum].linkedShapes[0] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)];
      cables[cableNum].linkedShapes[1] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+1];

      
      cables[cableNum].cableMaxLengthBeforeStretching = distanceBetweenNodes * 1.2;
      cables[cableNum].cableMaxLengthBeforeSnapping = distanceBetweenNodes * 1.25;

      cables[cableNum].restitution = 0.0f;
      world.getContactGenerators().push_back(cables + cableNum);
      cableNum++;
    }
  }

   //create vertical cable links
  for (size_t i = 0; i < sqrt(meshNodesCount)-1; i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount); j++)
    {
      cables[cableNum].linkedShapes[0] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)];
      cables[cableNum].linkedShapes[1] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+(int)sqrt(meshNodesCount)];
      
      cables[cableNum].cableMaxLengthBeforeStretching = distanceBetweenNodes * 1.2;
      cables[cableNum].cableMaxLengthBeforeSnapping = cables[cableNum].cableMaxLengthBeforeStretching * 1.02;

      cables[cableNum].restitution = 0.0f;
      world.getContactGenerators().push_back(cables + cableNum);
      cableNum++;
    }
  }
}
