#ifndef __CIRCLE_PARTICLE__
#define __CIRCLE_PARTICLE__

#include "Arduino.h"
#include "Ball.h"

class Ball; //forward declaration

class CircleParticle
{
private:
  bool DEBUG_CP = false;
  // the speed of the expanding shockwave
  float expansionSpeed;
  // a variable to keep track of the variable shockwave radius
  float expandingRadius;
  // this is the maximum available expansion for the radius
  float maxExpansion; 

  // Touched is a data structure to keep track of all the ball the circle particle
  // has come in contact with during its expansion. We need a data structure like
  // this one in order not trigger the same ball again when the circle particle radius,
  // during its expansion phase, is becoming greater than the distance between the
  // emitter adn the receiver ball.
  // This structure contains a single bit for each ball whose value is:
  // + 0, if the ball has not been visited yet;
  // + 1, if the ball has already been visited by the circle particle;
  
  //uint8_t* touched;
  // pay attention: with 6 byte to store status for each ball,
  // we can only have 48 balls!
  uint8_t touched[6] = {0, 0, 0, 0, 0, 0};

    uint8_t nPalline;
  // this is the ball ID the particle belongs to
  uint8_t parentId;
  // this is the ID of this specific circle particle
  // (the particle system of the ball can emit more than once
  // circle particle at the same time)
  uint8_t id;
  // a reference to the ball array
  Ball* pallinaRef; 

  // Circle Particle states
  enum {
    IDLE = 0,
    EXPANSION
  } state;

  void setBall(  uint8_t ballIndex);
  void clearBall(uint8_t ballIndex);
  boolean readBall( uint8_t ballIndex);
  void printTouchedMatrix();
  

public:

  CircleParticle(){};
  void init(uint8_t nPalline, Ball* _pallinaRef, uint8_t _parentId, uint8_t _cpId, float _expansionSpeed, float _maxExpansion);
  void update();
  void explode();
  float getRadius();
  float getStatus();
  void resetTouched();

};
#endif
