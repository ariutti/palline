// A class to create linear transition between 0.0 and 1.0 and back.
// The version 2 of the Attack-Release-Animator, is capable of beeing retriggered
// multiple times in a row, restarting its attack phase from the beginning but w/o 
// creating abruptly transition of its output value "y".
// Every time the Animator is triggered, phase will switch to ATTACK but, the 
// current value of "y" will be maintained so not to create abruptly transition.
// In order for "y" to reach the maximum "y2" value will however require the same
// amount of time "t1", but, "y" will start from its current value.

class Animator_ARv2
{
  // x axis represents time
  int t;    // utility var: time to reach target value (it can be either atk_time or rls_time)
  int x;     // utility var: keeps track of the passing time
  float y;   // utility var: interpolated value between y1 and y2
  
  int t_atk; // attack time 
  int t_rls; // release time
  int x1;    // start time
  int x2;    // final time
  float y1;  // initial value
  float y2;  // target value

  // states
  final int QUIET = -1;
  final int ATTACK  = 0;
  final int RELEASE  = 1;
  int state;

  Animator_ARv2 ( int _t_atk, int _t_rls ) {
    t_atk = _t_atk>0.0?_t_atk:250;
    t_rls = _t_rls>0.0?_t_rls:500;
    state = QUIET;
    y1 = y = 0.0;
  }

  void update() {
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

  void trigger() {
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
  
  
  void trigger( int _t_atk, int _t_rls ) {
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

  float getY() {
    return y;
  }
  
  void printStates()
  {
    // ATTACK
    // RELEASE
    // QUITE
    if(state == ATTACK)
    {
      print("ATTACK ");
    }
    else if(state == RELEASE)
    {
      print("RELEASE");
    }
    else
    {
      print("QUITE  ");
    } 
  }
}
