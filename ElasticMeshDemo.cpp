#include "ElasticMeshDemo.h"


ElasticMeshDemo::ElasticMeshDemo()
    :
    meshNodesCount(36), //must be an EVEN square num - 4,16,36,56,100,144
    nodeRadius(5),
    world (
        //number of contacts handled per frame = number of nodes + number of cables
        meshNodesCount +
        ((sqrt(meshNodesCount)-1) * ((2*(sqrt(meshNodesCount) - 1)) + 2)) +
        sqrt(meshNodesCount) - 1 -1
           )
{
  //calculate number of cables required to connect all meshNodes
  int meshHeight = sqrt(meshNodesCount) - 1; //gets the height of the mesh (number of links required)

  //rodsCount is the name as the height of the mesh, minus 1 for the link that will remain a cable
  rodsCount = meshHeight - 1; //minus from height of mesh to get number of rods required

  //get total amount of links for entire mesh
  cablesCount = meshHeight * ((2*meshHeight) +2);
  //and remove the rods that will be the mesh
  cablesCount -= rodsCount ;
  
  std::cout << "meshNodesCount: " << meshNodesCount << "\n";
  std::cout << "cablesCount: " << cablesCount << "\n";
  std::cout << "rodsCount: " << rodsCount << "\n";

  //create array of nodes
  meshNodes = new Circle[meshNodesCount];
  //place nodes in correct positions
  placeNodes();

  rods = new ShapeRod[rodsCount];
  connectNodesWithRods();
  
  //create array of cables to link nodes together
  cables = new ShapeCable[cablesCount];
  //connect nodes with cables
  connectNodesWithCables();
}


ElasticMeshDemo::~ElasticMeshDemo()
{}


void ElasticMeshDemo::draw()
{
  
  //draw each cable
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
  

  
  //draw rods
  for (size_t i = 0; i < rodsCount; i++)
  {
    aalineRGBA(TheGame::Instance()->getRenderer(),
               rods[i].linkedShapes[0]->getPositionX(),
               rods[i].linkedShapes[0]->getPositionY(),
               rods[i].linkedShapes[1]->getPositionX(),
               rods[i].linkedShapes[1]->getPositionY(),
               255,0,0,255);
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
  placeNodes(); //reset the position and forces applied to the nodes
  connectNodesWithCables(); //and reset the cable links between the nodes
}


void ElasticMeshDemo::handleInput()
{}


void ElasticMeshDemo::placeNodes()
{

  Sint16 winWidth = TheGame::Instance()->getWindowWidth();
  Sint16 winHeight = TheGame::Instance()->getWindowHeight();

  int percentageBorderWidth = 50;
  
  float borderX = ((winWidth / 100) * (percentageBorderWidth/2));
  horizontalDistanceBetweenNodes = ((winWidth / 100) * (100-percentageBorderWidth))
      / 100 * (100 / (sqrt(meshNodesCount)-1));
  
  float borderY = ((winHeight / 100) * (percentageBorderWidth/2));
  verticalDistanceBetweenNodes = ((winHeight / 100) * (100-percentageBorderWidth))
      / 100 * (100 / (sqrt(meshNodesCount)-1));
  


  //create and place nodes
  for (size_t i = 0; i < meshNodesCount; i++)
  {

    //calculate x,y of node according to size of the border
    int x = borderX + (((i % (int)sqrt(meshNodesCount)) * horizontalDistanceBetweenNodes));
    int y = borderY + (int)(i / sqrt(meshNodesCount)) * verticalDistanceBetweenNodes;

    meshNodes[i].setPosition(Vector2D(x, y));
    meshNodes[i].setRadius(nodeRadius);
    meshNodes[i].setVelocity(0,0);
    meshNodes[i].setDamping(0.9f);
    meshNodes[i].setMass(1.0f);
    meshNodes[i].clearAccumForces();

    //for leftmost nodes on top half of mesh - add acc to move right, underneath the mesh itself
    if (i >= (int)meshNodesCount / 2)
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
}


void ElasticMeshDemo::connectNodesWithRods()
{
 int rodNum = 0;

  for (size_t i = 0; i < rodsCount+1; i++)
  {
    //if not the middle connector
    if ((int)(i*sqrt(meshNodesCount))+(int)sqrt(meshNodesCount) != meshNodesCount/2)
    {
    rods[rodNum].linkedShapes[0] = &meshNodes[(int)(i*sqrt(meshNodesCount))];
    rods[rodNum].linkedShapes[1] = &meshNodes[(int)(i*sqrt(meshNodesCount))+(int)sqrt(meshNodesCount)];

    rods[rodNum].rodLength = verticalDistanceBetweenNodes;

    world.getContactGenerators().push_back(rods + rodNum);
    rodNum++;
    }
  }
}


void ElasticMeshDemo::connectNodesWithCables()
{
  //create horizontal cable links
  int cableNum = 0;
  for (size_t i = 0; i < sqrt(meshNodesCount); i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount)-1; j++)
    {
      /*
      if (i == 0) {
        int num = (int)(j*sqrt(meshNodesCount))+(int)sqrt(meshNodesCount);
        if ((num == meshNodesCount/2) == false)
        {
          std::cout << "skipped\n";
          continue;
        }
      }
      */
      cables[cableNum].linkedShapes[0] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)];
      cables[cableNum].linkedShapes[1] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+1];

      
      cables[cableNum].cableMaxLengthBeforeStretching = horizontalDistanceBetweenNodes * 1.2;
      cables[cableNum].cableMaxLengthBeforeSnapping = horizontalDistanceBetweenNodes * 1.25;

      cables[cableNum].restitution = 1.4f;
      world.getContactGenerators().push_back(cables + cableNum);
      cableNum++;
      
    }
  }

  //create vertical cable links
  for (size_t i = 0; i < sqrt(meshNodesCount)-1; i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount); j++)
    {
      //if any connection in first column, that is not the only cable connection
      if ( j == 0  &&  i != (sqrt(meshNodesCount) / 2) - 1)
        {
          //then skip this iteration
        }
      else //connect the node and vertical node beneath it with a cable
      {
        cables[cableNum].linkedShapes[0] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)];
        cables[cableNum].linkedShapes[1] = &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+(int)sqrt(meshNodesCount)];
      
        cables[cableNum].cableMaxLengthBeforeStretching = verticalDistanceBetweenNodes * 1.2;
        cables[cableNum].cableMaxLengthBeforeSnapping = verticalDistanceBetweenNodes * 1.41;

        cables[cableNum].restitution = 1.4f;
        world.getContactGenerators().push_back(cables + cableNum);
        cableNum++;
      }
    }
  }


  
}
