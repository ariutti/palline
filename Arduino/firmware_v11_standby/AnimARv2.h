
#ifndef __ARV2_ANIMATOR__
#define __ARV2_ANIMATOR__
#include "Arduino.h"

extern bool DEBUG;

class AnimARv2
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
  
  // 2020-09-30: a couple of variables used to keep track of
  // evntually changed time preference by the user.
  //int next_t_atk;
  //int next_t_rls;

  // states
  enum {
    QUIET = -1,
    ATTACK,
    RELEASE
  } state;


public:
	AnimARv2() {};

	void init( int t_atk, int t_rls );
  void update();
  //void trigger();
  void trigger( int t_atk, int t_rls );
  float getY();
	int getState();
  void printState();

  //2020-09-30: a new instance method added to make possibile
  // to change attack and release phase time changable by the user.
  //void setTimes(uint16_t t_atk, uint16_t t_rls);
  
};
#endif
