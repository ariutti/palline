
#ifndef __LINE_ANIMATOR__
#define __LINE_ANIMATOR__
#include "Arduino.h"

extern bool DEBUG;

class AnimLine
{

private:

  // NOTE: that time variables must be
  // long in order to store great values.
  
  // x axis represents time
  int t;     // utility var: time to reach target value (it can be either atk_time or rls_time)
  long x;    // utility var: keeps track of the passing time
  float y;   // utility var: interpolated value between y1 and y2
  
  int t_atk;  // t attack,
  int t_rls;  // t release
  long x1;    // start time
  long x2;    // end time
  float y1;   // start value
  float y2;   // end value
  //long dt;     // delta time
  //long st;     // start time

  // states
  enum {
    QUIET = -1,
    ATTACK,
    RELEASE
  } state;


public:
	AnimLine() {};

	void init( int t_atk, int t_rls );
  void update();
  void trigger();
  float getY();
	int getState();
  void printState();
};
#endif
