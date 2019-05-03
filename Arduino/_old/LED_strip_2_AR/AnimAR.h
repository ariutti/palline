
/*
* Animator.h - a library to create smooth linear variation
* on a value according to a customizable AR envelope.
* Created by Nicola Ariutti, March 01, 2019
* TODO: license
*/
#ifndef __AR_ANIMATOR__
#define __AR_ANIMATOR__
#include "Arduino.h"

class AnimAR
{
  
private:

  // note that time variables must be
  // long in order to store great values.
  int t1, t2;  // t attack, t release
  long dt;     // delta time
  long st;     // start time

  // states
  enum {
    QUIET = -1,
    ATK,
    RLS
  } state;

  float y;
  
public:
	AnimAR() {};

	void init( int t_1, int t_2 );
  void update();
  void trigger();
  float getY();
	int getState();
  void printState();
};
#endif
