#include "ElasticMeshDemo.h"

/**
   ElasticMeshDemo README
   
   The mesh is designed to rip apart through the middle as the rods (drawn in red)  at either end move
   in opposing directions. As the rods do not deform, they will continue moving whilst the elastic
   cables (drawn in black) holding the nodes together stretch and snap.

   Following Hook's Law, spring force is used to replicate the properties of elastic, defining the
   stiffness (spring constant) of each connection in the mesh, the length at which a spring will
   contact back together, and the length whereby it has stretched too far and will snap.
 **/


//default constructor
ElasticMeshDemo::ElasticMeshDemo(int p_numOfMeshNodes)
    :
    simulation ( //number of contacts handled per frame = number of nodes + number of rods
        p_numOfMeshNodes +
        //-1 as height needs ones less connection, and -1 for the one connector that is a cable not a rod
        sqrt(meshNodesCount) - 1 -1
                ),
    drawMeshNodes(false)
{
  setupDemo();
}

void ElasticMeshDemo::setupDemo(int p_numOfMeshNodes)
{
  meshNodesCount = p_numOfMeshNodes;
  
  //variables set according to meshNodesCount to enable simulations using different number of nodes
  switch (meshNodesCount)
  {
    case 16:
      percentageBorderWidth = 75;
          
      rodAcceleration = Vector2D(-7.0f, 7.0f);
      nodeDamping = 0.75f;
      nodeRadius = 4;
      
      cableSpringConstant = 0.2f;
      cableRestLengthMultipler = 1.0f;
      cableSnapLengthMultipler = 2.5f;
      break;
    
    case 36:
      percentageBorderWidth = 75;
          
      rodAcceleration = Vector2D(-6.0f, 14.0f);
      nodeDamping = 0.75f;
      nodeRadius = 4;
      
      cableSpringConstant = 0.2f;
      cableRestLengthMultipler = 1.0f;
      cableSnapLengthMultipler = 3.8f;
      break;
    
    case 64:
      percentageBorderWidth = 60;
      
      rodAcceleration = Vector2D(-6.5f, 0.0f);
      nodeDamping = 0.8f;
      nodeRadius = 5;
      
      cableSpringConstant = 0.2f;
      cableRestLengthMultipler = 1.0f;
      cableSnapLengthMultipler = 2.6f;
      break;
      
    case 144:
      percentageBorderWidth = 60;

      rodAcceleration = Vector2D(-6.0f, 8.0f);
      nodeDamping = 0.7f;
      nodeRadius = 4;
      
      cableSpringConstant = 0.25f;
      cableRestLengthMultipler = 1.0f;
      cableSnapLengthMultipler = 3.6f;
      break;
  }
  
  //calculate number of rods required in the mesh

  //gets the height of the mesh, minus 1 (number of vertical cables required)
  int meshHeight = sqrt(meshNodesCount) - 1; 

  //rodsCount is the name as the height of the mesh, minus another 1 to remove the link that will remain a cable and not a rod
  rodsCount = meshHeight - 1; //minus from height of mesh to get number of rods required

  //get total amount of links for entire mesh
  //cablesCount = (meshHeight * ((2*meshHeight) +2)) * 2;
  //and remove the rods that will be the mesh
  //cablesCount -= rodsCount * 2;
  
  
  //create array of nodes
  meshNodes = new Circle[meshNodesCount];
  //place nodes in correct positions
  placeNodes();
  //set nodes to draw by default
  //drawMeshNodes = false;

  //create array of rods
  rods = new ShapeRod[rodsCount];
  //place rods in correct positions
  connectNodesWithRods();
  
  //cable springs are assigned to cableRegisty, not saved in an array
  //connect nodes with cables using springForces
  connectNodesWithCables();
}

/*
ElasticMeshDemo::~ElasticMeshDemo()
{ }
*/

void ElasticMeshDemo::draw()
{
  
  //draw cables in black
  for (size_t i = 0; i < cableRegistry.registrations.size(); i++)
  {
    ForceGenerator* a = cableRegistry.registrations[i].fg;
    ShapeSpring* b = static_cast<ShapeSpring*>(a);

    
          aalineRGBA(TheGame::Instance()->getRenderer(),
                     cableRegistry.registrations[i].shape->getPositionX(),
                     cableRegistry.registrations[i].shape->getPositionY(),
                     b->endOfSpringObj->getPositionX(),
                     b->endOfSpringObj->getPositionY(),
                     0,0,0,255);
  }
  
  
  //draw rods in red (drawn with 3px thick line)
  for (size_t i = 0; i < rodsCount; i++)
  {
    aalineRGBA(TheGame::Instance()->getRenderer(),
               rods[i].linkedShapes[0]->getPositionX(),
               rods[i].linkedShapes[0]->getPositionY(),
               rods[i].linkedShapes[1]->getPositionX(),
               rods[i].linkedShapes[1]->getPositionY(),
               255,0,0,255);
        aalineRGBA(TheGame::Instance()->getRenderer(),
               rods[i].linkedShapes[0]->getPositionX()+1,
               rods[i].linkedShapes[0]->getPositionY()+1,
               rods[i].linkedShapes[1]->getPositionX()+1,
               rods[i].linkedShapes[1]->getPositionY()+1,
               255,0,0,255);
            aalineRGBA(TheGame::Instance()->getRenderer(),
               rods[i].linkedShapes[0]->getPositionX()-1,
               rods[i].linkedShapes[0]->getPositionY()-1,
               rods[i].linkedShapes[1]->getPositionX()-1,
               rods[i].linkedShapes[1]->getPositionY()-1,
               255,0,0,255);
  }
  
  if (drawMeshNodes) {
    //draw meshNodes using their base class draw() method
    for (size_t i = 0 ; i < meshNodesCount ; i++)
    {
      meshNodes[i].draw();
    }
  }
}

void ElasticMeshDemo::update()
{
  
  //remove accumu forces from previous frame for each node
  simulation.startFrame();

  //check for snapped cables and remove them
  checkForSnappedCables();
  
  //get duration of the last frame
  float duration = TheGame::Instance()->getFrameTime() * 0.01f;
  if (duration <= 0.0f) return; 

  //apply forces to the cables
  cableRegistry.updateForces(TheGame::Instance()->getFrameTime());
  
  //update meshNodes from their base class - updates if an object is being held by mouse
  for (size_t i = 0 ; i < meshNodesCount ; i++)
  {
    meshNodes[i].update();
  }
      
  //run simulation - apply forces from collsions
  simulation.runPhysics(duration);
}


void ElasticMeshDemo::clean()
{
  if (cableRegistry.registrations.size() > 0) cableRegistry.clear();
  if (meshNodes) delete[] meshNodes;
  if (rods) delete[] rods; 
}

void ElasticMeshDemo::reset()
{
  if (cableRegistry.registrations.size() > 0) cableRegistry.clear(); //remove all added forces

  setupDemo(meshNodesCount);
}

//through a key press the meshNodesCount can be selected, and the simulation restarted
void ElasticMeshDemo::handleInput()
{
  if (TheInputHandler::Instance()->keyNPressed)
  {
    TheInputHandler::Instance()->keyNPressed = false;
    drawMeshNodes = !drawMeshNodes; 
  }
  else if (TheInputHandler::Instance()->keyQPressed)
  {
    TheInputHandler::Instance()->keyQPressed = false;
    if (cableRegistry.registrations.size() > 0) cableRegistry.clear();
    setupDemo(16);
  }
  else if (TheInputHandler::Instance()->keyWPressed)
  {
    TheInputHandler::Instance()->keyWPressed = false;
    if (cableRegistry.registrations.size() > 0) cableRegistry.clear();
    setupDemo(36);
  }
  else if (TheInputHandler::Instance()->keyEPressed)
  {
    TheInputHandler::Instance()->keyEPressed = false;
    if (cableRegistry.registrations.size() > 0) cableRegistry.clear();
    setupDemo(64);
  }
  else if (TheInputHandler::Instance()->keyRPressed)
  {
    TheInputHandler::Instance()->keyRPressed = false;
    if (cableRegistry.registrations.size() > 0) cableRegistry.clear();
    setupDemo(144);
  }
}

//spawns the nodes in their initial starting positions with forces applied according to meshNodeCount
void ElasticMeshDemo::placeNodes()
{
  //get screen size
  Sint16 winWidth = TheGame::Instance()->getWindowWidth();
  Sint16 winHeight = TheGame::Instance()->getWindowHeight();

  //calculate size of the borders according to screen size_t
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

    //set each node's physics settings
    meshNodes[i].setPosition(Vector2D(x, y));
    meshNodes[i].setRadius(nodeRadius);
    meshNodes[i].setVelocity(0,0);
    meshNodes[i].setDamping(nodeDamping);
    meshNodes[i].setMass(1.0f);
    meshNodes[i].clearAccumForces();

    //for leftmost rods on top half of mesh - add acc to move right, underneath the mesh itself
    if (i >= (int)meshNodesCount / 2)
    {
      if (i % (int)sqrt(meshNodesCount) == 0) {
        meshNodes[i].setAcceleration(rodAcceleration.getX(), rodAcceleration.getY());
      }
    }//for leftmost rods on bottom half of mesh - add acc to move left, away from the mesh itself
    else if(i < (int)meshNodesCount / 2)
    {
      if (i % (int)sqrt(meshNodesCount) == 0)
      {
        meshNodes[i].setAcceleration(-rodAcceleration.getX(), -rodAcceleration.getY());
      }
    }
  }
}


void ElasticMeshDemo::connectNodesWithRods()
{
 int rodNum = 0;

 //for the first column of nodes, connect each node with a rod apart from the middle connection
  for (size_t i = 0; i < rodsCount+1; i++)
  {
    //if not the middle connector, of which is a cable not a rod
    if ((int)(i*sqrt(meshNodesCount))+(int)sqrt(meshNodesCount) != meshNodesCount/2)
    {
    rods[rodNum].linkedShapes[0] = &meshNodes[(int)(i*sqrt(meshNodesCount))];
    rods[rodNum].linkedShapes[1] = &meshNodes[(int)(i*sqrt(meshNodesCount))+(int)sqrt(meshNodesCount)];

    rods[rodNum].rodLength = verticalDistanceBetweenNodes;

    simulation.getContactGenerators().push_back(rods + rodNum);
    rodNum++;
    }
  }
}

void ElasticMeshDemo::connectNodesWithCables()
{ 
  //create horizontal cable links
  for (size_t i = 0; i < sqrt(meshNodesCount); i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount)-1; j++)
    {
      //spring force generator only creates force for one object, so both ends of the spring require
      //their own ShapeSpring

      //create spring to generate force for first node in the cable
      ShapeSpring* springForce = new ShapeSpring(
          &meshNodes[(int)(i*sqrt(meshNodesCount)+j)],
          cableSpringConstant,
          horizontalDistanceBetweenNodes * cableRestLengthMultipler,
          horizontalDistanceBetweenNodes * cableSnapLengthMultipler);
      cableRegistry.add(
          &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+1],
          springForce);
      
      //create spring to generate force for other node in the cable
      ShapeSpring* springForce1 = new ShapeSpring(
          &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+1],
          cableSpringConstant,
          horizontalDistanceBetweenNodes * cableRestLengthMultipler,
          horizontalDistanceBetweenNodes * cableSnapLengthMultipler);
      cableRegistry.add(
          &meshNodes[(int)(i*sqrt(meshNodesCount)+j)],
          springForce1);
    }
  }

  //create vertical cable links
  for (size_t i = 0; i < sqrt(meshNodesCount)-1; i++)
  {
    for (size_t j = 0; j < sqrt(meshNodesCount); j++)
    {
      //if i is in the first column, and i is not the middle connection (rod connection)
      if ( j == 0  &&  i != (sqrt(meshNodesCount) / 2) - 1)
      { /*then skip this iteration, we are one of the rod connectors*/  }
      else  
      {
        //create spring to generate force for first node in the cable
        ShapeSpring* springForce = new ShapeSpring(
            &meshNodes[(int)(i*sqrt(meshNodesCount)+j)],
            cableSpringConstant,
            verticalDistanceBetweenNodes * cableRestLengthMultipler,
            verticalDistanceBetweenNodes * cableSnapLengthMultipler);
        cableRegistry.add(
            &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+(int)sqrt(meshNodesCount)],
            springForce);

        //create spring to generate force for other node in the cable
        ShapeSpring* springForce1 = new ShapeSpring(
            &meshNodes[(int)(i*sqrt(meshNodesCount)+j)+(int)sqrt(meshNodesCount)],
            cableSpringConstant,
            verticalDistanceBetweenNodes * cableRestLengthMultipler,
            verticalDistanceBetweenNodes * cableSnapLengthMultipler);
        cableRegistry.add(
            &meshNodes[(int)(i*sqrt(meshNodesCount)+j)],
            springForce1);
      }
    }
  }
}


void ElasticMeshDemo::checkForSnappedCables()
{
  //for each cable in the cableRegistry
  for (size_t i = 0; i < cableRegistry.registrations.size(); i++)
  {
    //get the springForce from the forceGenerator
    ForceGenerator* a = cableRegistry.registrations[i].fg;
    ShapeSpring* b = static_cast<ShapeSpring*>(a);

    //check if the cable has snapped
    if (b->getSnapped() == true)
    {
      cableRegistry.remove(a); 
    }
  }
}









