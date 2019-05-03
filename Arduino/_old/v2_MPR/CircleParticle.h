#ifndef __CIRCLE_PARTICLE__
#define __CIRCLE_PARTICLE__

#include "Arduino.h"
#include "Ball.h"

extern bool DEBUG;

class Ball; //forward declaration

class CircleParticle
{
private:
  // the speed of the expanding shokwave
  float expansionSpeed;
  // a variable to keep track of the variable shockwave radius
  float expandingRadius;
  // this is the maximum available expansion for the radius
  float maxExpansion; 
  int* touched;
  // this is the ball ID the particle belongs to
  int nPalline;
  int parentId;
  // a reference to the ball array
  Ball* pallinaRef; 

  // Circle Particle states
  enum {
    IDLE = 0,
    EXPANSION
  } state;

public:

  CircleParticle(){};
  void init(int nPalline, Ball* _pallinaRef, int _parentId, float _expansionSpeed, float _maxExpansion);
  void update();
  void explode();
  float getRadius();
  float getStatus();
  void resetTouched();

};
#endif
