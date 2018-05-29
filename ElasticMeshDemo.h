#ifndef _ElasticMeshDemo_
#define _ElasticMeshDemo_


#include "Demo.h"
#include <math.h>
#include <vector>
#include "Vector2D.h"
#include "Contacts.h"
#include "InputHandler.h"
#include "World.h"
#include "ForceGenerator.h"
#include "SDL2/SDL2_gfxPrimitives.h" 
#include "ShapeLinks.h"
#include "Circle.h"

/**
 * The mesh is designed to rip apart through the middle as the rods at either end move in
 * opposing directions. As the rods do not deform, they will continue moving whilst the elastic
 * cables holding the nodes together stretch and snap.
 **/


class ElasticMeshDemo : public Demo
{
  //array holding all nodes that are joined by cables to form a mesh
  Circle *meshNodes;
  //number of nodes the mesh is created from
  int meshNodesCount; //(must be an EVEN square number (e.g. 4,16,36,56,100,144))
  //the size at which a node is drawn on-screen - does not affect the simulation
  int nodeRadius;

  float verticalDistanceBetweenNodes;
  float horizontalDistanceBetweenNodes;

  
  //array holding cables that connect the nodes together to form the mesh
  ShapeCable *cables;
  //total number of cables in cables array
  int cablesCount;

  ShapeRod *rods;
  int rodsCount;
  
  //holds all nodes and the connections between them, and updates objs from the contacts applied to
  //the nodes and cables
  World world;

public:
    ElasticMeshDemo(); //constr
    virtual ~ElasticMeshDemo(); //destr

    //draws the nodes and cables on screen
    virtual void draw();

    //applies physics to nodes
    virtual void update();

    //deletes all objects for cleanup upon closing demo
    virtual void clean();

    //restarts the scene
    virtual void reset();

    //collects keyboard/mouse input and applies appropriate actions
    virtual void handleInput();


private:
    //calculates and places nodes in position to form a mesh
    //returns distanceBetweenNodes, the distance between each node, and is used in connectNodesWithCables to
    //establish the maximum stretched length of a cable
    void placeNodes();

    void connectNodesWithRods();

    //conects all nodes with cables to form the mesh
    //distanceBetweenNodes is multiplied by 1.2 to give the max stretch length between nodes
    void connectNodesWithCables();
};

#endif
