#include "AnimARv2.h"

// Set attack and release time in millisecond
// default are attack time (t_1) = 10ms and
// release time (t_2) = 500ms
void AnimARv2::init( int _t_atk, int _t_rls ) {
	t_atk = _t_atk>0.0?_t_atk:10;
	t_rls = _t_rls>0.0?_t_rls:500;
	state = QUIET;
  y1 = y = 0.0;
	y2 = 1.0; // redundant
}


void AnimARv2::update() {
	 if( state == ATTACK ) {
		 x = millis();
		 y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
		 if( y > y2 ) {
			 y = y1 = y2;
			 // set the state machine to begin relase phase
			 y2 = 0.0;
			 t  = t_rls;
			 x1 = millis();
			 x2 = x1 + t;
			 y1 = y;
			 state = RELEASE;
		 }
	 } else if( state == RELEASE ) {
		 x = millis();
		 y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
		 if( y < y2 ) {
			 y = y1 = y2;
			 state = QUIET;
		 }
	 }
 }


/*
 void AnimARv2::trigger() {
  //println("target: " + target + "; rampTime: " + rampTime + ";");
  if(y == 1.0 )
    return;
  y2 = 1.0;
  t  = t_atk;
  x1 = millis();
  x2 = x1 + t;
  y1 = y;
  state = ATTACK;
}
*/


 void AnimARv2::trigger( int _t_atk, int _t_rls ) {
  t_atk = _t_atk;
  t_rls = _t_rls; 
  //println("target: " + target + "; rampTime: " + rampTime + ";");
  if(y == 1.0 )
    return;
  y2 = 1.0;
  t  = t_atk;
  x1 = millis();
  x2 = x1 + t;
  y1 = y;
  state = ATTACK;
}

float AnimARv2::getY() {
	return y;
}

int AnimARv2::getState() {
	return state;
}

// remember:
//const int QUIET = -1;
//const int ATTACK  = 0;
//const int RELEASE = 2;
void AnimARv2::printState()
{
  switch(state) {
    case QUIET:
      Serial.print("QUIET");
      break;
    case ATTACK:
      Serial.print("ATTACK");
      break;
    case RELEASE:
      Serial.print("RELEASE");
      break;
    default:
      break;
  }
  //Serial.println();
}


/*
//2020-09-30: 
// A new instance method added to make possibile
// to change attack and release phase time changable by the user.
// I want to prevent times to be changed while the animator is in its "active" states.
// So I've created thwo new variable to keep track of the user preferences for the new timings
// I will change times when animator will pass from active to QUIET state.
void AnimARv2::setTimes(uint16_t _t_atk, uint16_t _t_rls) {
  next_t_atk = _t_atk>0.0?_t_atk:10;
  next_t_rls = _t_rls>0.0?_t_rls:500;

  // check if we already are in the QUIET status
  // is so, update the timing right now. Alternatively
  // let the update method do the job when passing
  // to the QUIET status
  if( state == QUIET ) {
    if( next_t_atk != t_atk) {
      t_atk = next_t_atk;
    }
    if( next_t_rls != t_rls) {
      t_rls = next_t_rls;
    }
  }
}
*/
