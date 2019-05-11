#include "AnimLine.h"

// Set attack and release time in millisecond
// default are attack time (t_1) = 10ms and
// release time (t_2) = 500ms
void AnimLine::init( int _t_atk, int _t_rls ) {
	t_atk = _t_atk>0.0?_t_atk:10;
	t_rls = _t_rls>0.0?_t_rls:500;
	state = QUIET;
  y1 = y = 0.0;
	y2 = 1.0; // redundant
}


void AnimLine::update() {
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

 void AnimLine::trigger() {
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

float AnimLine::getY()
{
	return y;
}

int AnimLine::getState()
{
	return state;
}

// remember:
//const int QUIET = -1;
//const int ATTACK  = 0;
//const int RELEASE = 2;
void AnimLine::printState()
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
