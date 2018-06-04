#ifndef _ElasticMeshDemo_
#define _ElasticMeshDemo_

#include "Demo.h"
#include <math.h>  //sqrt
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
   ElasticMeshDemo README
   
   The mesh is designed to rip apart through the middle as the rods (drawn in red)  at either end move
   in opposing directions. As the rods do not deform, they will continue moving whilst the elastic
   cables (drawn in black) holding the nodes together stretch and snap.

   Following Hook's Law, spring force is used to replicate the properties of elastic, defining the
   stiffness (spring constant) of each connection in the mesh, the length at which a spring will
   contact back together, and the length whereby it has stretched too far and will snap.
 **/

class ElasticMeshDemo : public Demo
{
  //array holding all nodes that are joined by cables to form a mesh
  Circle *meshNodes;
  //number of nodes the mesh is created from
  int meshNodesCount; //must be an EVEN square number from given choices [ 16, 36, 64, 144 ]
  //the size at which a node is drawn on-screen - does not affect the simulation
  int nodeRadius;

  //press N key to turn on/off. if on nodes between cables are drawn
  bool drawMeshNodes;
  
  //the number of pixels between nodes upon spawing. Calculated from int percentageBorderWidth
  float verticalDistanceBetweenNodes;
  float horizontalDistanceBetweenNodes;

  //holds all the spring forces that connect the nodes by 'cables'
  //updateForces() is called on the registry to apply the spring forces
  ForceRegistry cableRegistry;
  //total number of cables in cables array - NOT REQUIRED
  //int cablesCount;

  //array holding non-flexible rods that connect front most nodes together
  ShapeRod *rods;
  //total number of rods in rods arrayx
  int rodsCount;
  
  //holds all nodes and the connections between them, and updates objs from the contacts applied to
  //the nodes and cables
  World simulation;


private:
  //variables set according to meshNodesCount to enable simulations using different number of nodes

  /**
     Changes the border size, scaling the size at which nodes are drawn in the center of the screen.
   **/
  int percentageBorderWidth;
  
  /**
     rodAcceleration defines the constant x and y movement that is applied to the rods that are at the front
     of the mesh. These values are set to pull the mesh apart.
   **/
  Vector2D rodAcceleration;

  /**
     The amount of velocity that is retained between frames. An artifical form of friction.
   **/
  float nodeDamping;
  
  /**
     The stiffness/strength of the spring. Higher the value the stiffer, and stronger the push back.
   **/
  float cableSpringConstant;

  /**
     When the distance between the two nodes forming a cable surpasses the rest length a force will be
     generated to pull them together is created.
     The rest length is at default set to the distance between the nodes, as calculated from the dimensions
     of the screen.
  **/
  float cableRestLengthMultipler;

  /**
     When the distance between the two nodes forming a cable surpasses the snap length the cable will snap
     and be removed from the force registry.
     The rest length is at default set to twice the distance of the rest length.
  **/
  float cableSnapLengthMultipler;
  

public:
    ElasticMeshDemo(int p_numOfMeshNodes = 36); //constr
    ~ElasticMeshDemo() {} //destr

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

    //if any cables have snapped under the forces applied to them they are removed in this method
    void checkForSnappedCables();
    
private:

    //sets up the demo according to how many nodes have been assigned to meshNodesCount in the constructor
    void setupDemo(int p_numOfMeshNodes = 64);
    
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
