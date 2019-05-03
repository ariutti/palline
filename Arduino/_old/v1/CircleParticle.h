#ifndef __CIRCLE_PARTICLE__
#define __CIRCLE_PARTICLE__

#include "Arduino.h"
#include "Pallina.h"

class Pallina; //forward declaration

class CircleParticle
{
private:

  float expandingRadius;
  float expansionSpeed;
  float maxExpansion; // this is the maximum available expansion for the radius
  int* touched;
  int parentId; // this is the pallina ID the particle belongs to
  int nPalline;
  Pallina* pallinaRef; // a reference to the palline array

  // states
  enum {
    IDLE = 0,
    EXPANSION
  } state;

public:

  CircleParticle(){};
  void init(int nPalline, Pallina* _pallinaRef, int _parentId, float _speed, float _maxExpansion);
  void update();
  void explode();
  float getRadius();
  float getStatus();
  void resetTouched();

};
#endif
