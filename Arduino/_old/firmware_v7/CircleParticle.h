#ifndef __CIRCLE_PARTICLE__
#define __CIRCLE_PARTICLE__

#include "Arduino.h"
#include "Ball.h"




class Ball; //forward declaration

class CircleParticle
{
private:
  bool DEBUG_CP = false;
  // the speed of the expanding shokwave
  float expansionSpeed;
  // a variable to keep track of the variable shockwave radius
  float expandingRadius;
  // this is the maximum available expansion for the radius
  float maxExpansion; 
  
  //uint8_t* touched;
  // pay attention: with 6 byte to store status for each ball,
  // we can only have 48 balls!
  uint8_t touched[6] = {0, 0, 0, 0, 0, 0};

  
  // this is the ball ID the particle belongs to
  uint8_t nPalline;
  uint8_t parentId;
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
