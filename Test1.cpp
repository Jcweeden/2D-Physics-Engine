#include "Contacts.h"
#include "ForceGenerator.h"
#include "ShapeBody.h"
#include "Vector2D.h"
#include "World.h"
#include <vector>
#include <math.h>
#include <random>
#include <iostream>
#include "Random.h"
#include "Game.h"

#define BLOB_COUNT 5
#define PLATFORM_COUNT 10
#define BLOB_RADIUS 0.4f


class Platform : public ShapeContactGenerator
{
 public:
  Vector2D start;
  Vector2D end;

  /**
   * Holds a pointer to the particles we're checking for collisions with.
   */
  ShapeBody *plaformShapes;

  Platform()
      : start(0,0), end(0,0)
  {}
  
  virtual unsigned addContact(ShapeContact *contact, unsigned limit) const;
};

unsigned Platform::addContact(ShapeContact *contact, unsigned limit) const
{
  const static float restitution = 0.0f;

  unsigned used = 0;
  for (unsigned i = 0; i < BLOB_COUNT; i++)
  {
    if (used >= limit) break;

    // Check for penetration
    Vector2D toParticle = plaformShapes[i].getPosition() - start;
    Vector2D lineDirection = end - start;
    float projected = toParticle * lineDirection;
    float platformSqLength = lineDirection.squareMagnitude();
    if (projected <= 0)
    {
      // The blob is nearest to the start point
      if (toParticle.squareMagnitude() < BLOB_RADIUS*BLOB_RADIUS)
      {
        // We have a collision
        contact->contactNormal = toParticle.unit();
        //contact->contactNormal.z = 0;
        contact->restitution = restitution;
        contact->shapesInContact[0] = plaformShapes + i;
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = BLOB_RADIUS - toParticle.magnitude();
        used ++;
        contact ++;
      }

    }
    else if (projected >= platformSqLength)
    {
      // The blob is nearest to the end point
      toParticle = plaformShapes[i].getPosition() - end;
      if (toParticle.squareMagnitude() < BLOB_RADIUS*BLOB_RADIUS)
      {
        // We have a collision
        contact->contactNormal = toParticle.unit();
        // contact->contactNormal.z = 0;
        contact->restitution = restitution;
        contact->shapesInContact[0] = plaformShapes + i;
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = BLOB_RADIUS - toParticle.magnitude();
        used ++;
        contact ++;
      }
    }
    else
    {
      // the blob is nearest to the middle.
      float distanceToPlatform =
          toParticle.squareMagnitude() -
          projected*projected / platformSqLength;
      if (distanceToPlatform < BLOB_RADIUS*BLOB_RADIUS)
      {
        // We have a collision
        Vector2D closestPoint =
            start + lineDirection*(projected/platformSqLength);

        contact->contactNormal = (plaformShapes[i].getPosition()-closestPoint).unit();
        // contact->contactNormal.z = 0;
        contact->restitution = restitution;
        contact->shapesInContact[0] = plaformShapes + i;
        contact->shapesInContact[1] = 0;
        contact->penetrationDepth = BLOB_RADIUS - sqrt(distanceToPlatform);
        used ++;
        contact ++;
      }
    }
  }
  return used;
}

class BlobForceGenerator : public ForceGenerator
{
 public:
  /**
   * Holds a pointer to the particles we might be attracting.
   */
  ShapeBody *shapes;

  /**
   * The maximum force used to push the particles apart.
   */
  float maxReplusion;

  /**
   * The maximum force used to pull particles together.
   */
  float maxAttraction;

  /**
   * The separation between particles where there is no force.
   */
  float minNaturalDistance, maxNaturalDistance;

  /**
   * The force with which to float the head particle, if it is
   * joined to others.
   */
  float floatHead;

  /**
   * The maximum number of particles in the blob before the head
   * is floated at maximum force.
   */
  unsigned maxFloat;

  /**
   * The separation between particles after which they 'break' apart and
   * there is no force.
   */
  float  maxDistance;

  virtual void updateForce(
      ShapeBody *particle,
      float duration
                           );
};

void BlobForceGenerator::updateForce(ShapeBody *shape, float duration)
{
  unsigned joinCount = 0;
  for (unsigned i = 0; i < BLOB_COUNT; i++)
  {
    // Don't attract yourself
    if (shapes + i == shape) continue;

    // Work out the separation distance
    Vector2D separation =
        shapes[i].getPosition() - shape->getPosition();
    //separation.z = 0.0f;
    float distance = separation.magnitude();

    if (distance < minNaturalDistance)
    {
      // Use a repulsion force.
      distance = 1.0f - distance / minNaturalDistance;
      shape->addForce(
          separation.unit() * (1.0f - distance) * maxReplusion * -1.0f
                         );
      joinCount++;
    }
    else if (distance > maxNaturalDistance && distance < maxDistance)
    {
      // Use an attraction force.
      distance =
          (distance - maxNaturalDistance) /
          (maxDistance - maxNaturalDistance);
      shape->addForce(
          separation.unit() * distance * maxAttraction
                         );
      joinCount++;
    }
  }

  // If the particle is the head, and we've got a join count, then float it.
  if (shape == shapes && joinCount > 0 && maxFloat > 0)
  {
    float force = float(joinCount / maxFloat) * floatHead;
    if (force > floatHead) force = floatHead;
    shape->addForce(Vector2D(0, force));
  }

}

/**
 * The main demo class definition.
 */
class BlobDemo /*: public Application*/
{
    ShapeBody *blobs;

    Platform *platforms;

    World world;

    BlobForceGenerator blobForceGenerator;

    /* The control for the x-axis. */
    float xAxis;

    /* The control for the y-axis. */
    float yAxis;

    void reset();

public:
    /** Creates a new demo object. */
    BlobDemo();
    virtual ~BlobDemo();

    /** Returns the window title for the demo. */
  //virtual const char* getTitle();

    /** Display the particles. */
    virtual void display();

    /** Update the particle positions. */
    virtual void update();

    /** Handle a key press. */
    virtual void key(unsigned char key);

};

// Method definitions
BlobDemo::BlobDemo()
:
xAxis(0), yAxis(0),
world(PLATFORM_COUNT+BLOB_COUNT, PLATFORM_COUNT)
{
    // Create the blob storage
    blobs = new ShapeBody[BLOB_COUNT];
    Random r;

    // Create the force generator
    blobForceGenerator.shapes = blobs;
    blobForceGenerator.maxAttraction = 20.0f;
    blobForceGenerator.maxReplusion = 10.0f;
    blobForceGenerator.minNaturalDistance = BLOB_RADIUS*0.75f;
    blobForceGenerator.maxNaturalDistance = BLOB_RADIUS*1.5f;
    blobForceGenerator.maxDistance = BLOB_RADIUS * 2.5f;
    blobForceGenerator.maxFloat = 2;
    blobForceGenerator.floatHead = 8.0f;

    // Create the platforms
    platforms = new Platform;
    for (unsigned i = 0; i < PLATFORM_COUNT; i++)
    {
        platforms[i].start = Vector2D(
            float(i%2)*10.0f - 5.0f,
            float(i)*4.0f + ((i%2)?0.0f:2.0f));
        platforms[i].start.m_x += r.randomBinomial(2.0f);
        platforms[i].start.m_y += r.randomBinomial(2.0f);

        platforms[i].end = Vector2D(
            float(i%2)*10.0f + 5.0f,
            float(i)*4.0f + ((i%2)?2.0f:0.0f));
        platforms[i].end.m_x += r.randomBinomial(2.0f);
        platforms[i].end.m_y += r.randomBinomial(2.0f);

        // Make sure the platform knows which particles it
        // should collide with.
        platforms[i].plaformShapes = blobs;
        world.getContactGenerators().push_back(platforms + i);
    }

    // Create the blobs.
    Platform *p = platforms + (PLATFORM_COUNT-2);
    float fraction = (float)1.0 / BLOB_COUNT;
    Vector2D delta = p->end - p->start;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        unsigned me = (i+BLOB_COUNT/2) % BLOB_COUNT;
        blobs[i].setPosition(
            p->start + delta * (float(me)*0.8f*fraction+0.1f) +
            Vector2D(0, 1.0f+r.randomReal()));

        blobs[i].setVelocity(0,0);
        blobs[i].setDamping(0.2f);
        blobs[i].setAcceleration(0.0f, 1.0f); //gravity
        blobs[i].setMass(1.0f);
        blobs[i].clearAccumForces();

        world.getShapes().push_back(blobs + i);
        world.getForceRegistry().add(blobs + i, &blobForceGenerator);
    }
}

void BlobDemo::reset()
{
    Random r;
    Platform *p = platforms + (PLATFORM_COUNT-2);
    float fraction = (float)1.0 / BLOB_COUNT;
    Vector2D delta = p->end - p->start;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        unsigned me = (i+BLOB_COUNT/2) % BLOB_COUNT;
        blobs[i].setPosition(
            p->start + delta * (float(me)*0.8f*fraction+0.1f) +
            Vector2D(0, 1.0f+r.randomReal()));
        blobs[i].setVelocity(0,0);
        blobs[i].clearAccumForces();
    }
}

BlobDemo::~BlobDemo()
{
    delete blobs;
}

void BlobDemo::display()
{
  /*
    Vector2D pos = blobs[0].getPosition();

    // Clear the view port and set the camera direction
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(pos.m_x, pos.m_y, 6.0,  pos.m_x, pos.m_y, 0.0,  0.0, 1.0, 0.0);

    glColor3f(0,0/*,0*//*);

/*
    glBegin(GL_LINES);
    glColor3f(0,0/*,1*//*);/*
    for (unsigned i = 0; i < PLATFORM_COUNT; i++)
    {
        const Vector2D &p0 = platforms[i].start;
        const Vector2D &p1 = platforms[i].end;
        glVertex3f(p0.m_x, p0.m_y/*, p0.m_z*//*);*/
/*  glVertex3f(p1.m_x, p1.m_y/*, p1.m_z*//*);*/
/* }/*
    glEnd();

    glColor3f(1,0,0);
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        const Vector2D &p = blobs[i].getPosition();
        glPushMatrix();
        glTranslatef(p.x, p.y, p.z);
        glutSolidSphere(BLOB_RADIUS, 12, 12);
        glPopMatrix();
    }

    Vector2D p = blobs[0].getPosition();
    Vector2D v = blobs[0].getVelocity() * 0.05f;
    v.trim(BLOB_RADIUS*0.5f);
    p = p + v;
    glPushMatrix();
    glTranslatef(p.x-BLOB_RADIUS*0.2f, p.y, BLOB_RADIUS);
    glColor3f(1,1,1);
    glutSolidSphere(BLOB_RADIUS*0.2f, 8, 8);
    glTranslatef(0,0,BLOB_RADIUS*0.2f);
    glColor3f(0,0,0);
    glutSolidSphere(BLOB_RADIUS*0.1f, 8, 8);
    glTranslatef(BLOB_RADIUS*0.4f, 0, -BLOB_RADIUS*0.2f);
    glColor3f(1,1,1);
    glutSolidSphere(BLOB_RADIUS*0.2f, 8, 8);
    glTranslatef(0,0,BLOB_RADIUS*0.2f);
    glColor3f(0,0,0);
    glutSolidSphere(BLOB_RADIUS*0.1f, 8, 8);
    glPopMatrix();
*/
}

void BlobDemo::update()
{
    // Clear accumulators
    world.startFrame();

    // Find the duration of the last frame in seconds
    float duration = TheGame::Instance()->getFrameTime() * 0.01f;
    //float duration = (float)TimingData::get().lastFrameDuration * 0.001f;
    if (duration <= 0.0f) return;

    // Recenter the axes
    xAxis *= pow(0.1f, duration);
    yAxis *= pow(0.1f, duration);

    // Move the controlled blob
    blobs[0].addForce(Vector2D(xAxis, yAxis)*10.0f);

    // Run the simulation
    world.runPhysics(duration);

    // Bring all the particles back to 2d
    /* Vector2D position;
    for (unsigned i = 0; i < BLOB_COUNT; i++)
    {
        blobs[i].getPosition(&position);
        position.z = 0.0f;
        blobs[i].setPosition(position);
    }*/

    //Application::update();
}

/*const char* BlobDemo::getTitle()
{
    return "Cyclone > Blob Demo";
    }*/

void BlobDemo::key(unsigned char key)
{
    switch(key)
    {
    case 'w': case 'W':
        yAxis = 1.0;
        break;
    case 's': case 'S':
        yAxis = -1.0;
        break;
    case 'a': case 'A':
        xAxis = -1.0f;
        break;
    case 'd': case 'D':
        xAxis = 1.0f;
        break;
    case 'r': case 'R':
        reset();
        break;
    }
}
