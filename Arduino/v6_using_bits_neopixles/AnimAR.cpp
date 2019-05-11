#include "AnimAR.h"


// Set attack and release time in millisecond
// default are attack time (t_1) = 10ms and
// release time (t_2) = 500ms
void AnimAR::init( int t_1, int t_2 ) {
	t1 = t_1>0.0?t_1:10;
	t2 = t_2>0.0?t_2:500;
	state = QUIET;
  y = 0.0;
}

void AnimAR::update()
{
	if(state == ATK)
	{
		dt = millis() - st;
    // TODO: insert a check here in order to prevent
    // cases where 'dt' become grater than t1.
    // Update st accordingly.
    //if( dt < 0 )
    //  Serial.println("ERROR");
		y =  (1.0*dt)/t1;
		if(y > 1.0)
		{
      
			y = 1.0;
      st = millis();
			state = RLS;
      if (DEBUG) {
        Serial.print("change from ATTACK to RELEASE -> ");
        Serial.println(state);
      }
		}
	}
	else if( state == RLS )
	{
		dt = millis() - st;
    // TODO: insert a check here in order to prevent
    // cases where 'dt' become grater than t2.
    // Update st accordingly.
		y =  1.0 - ( (1.0*dt)/t2 );
    if(y < 0.0)
		{
      if(DEBUG) {
        Serial.print("change from RELEASE to QUIET ->");
        Serial.println(state);
      }
			y = 0.0;
			state = QUIET;
		}
	}
}

void AnimAR::trigger()
{
  st = millis();
  state = ATK;
}

float AnimAR::getY()
{
	return y;
}

int AnimAR::getState()
{
	return state;
}


// remember:
//const int QUIET = -1;
//const int ATTACK  = 0;
//const int RELEASE = 2;
void AnimAR::printState()
{
  switch(state) {
    case QUIET:
      Serial.print("QUIET");
      break;
    case ATK:
      Serial.print("ATTACK");
      break;
    case RLS:
      Serial.print("RELEASE");
      break;
    default:
      break;
  }
  //Serial.println();
}
