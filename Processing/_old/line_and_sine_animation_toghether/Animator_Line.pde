class Animator_Line
{
  // time represent x axis
  int t;  // ramp time
  int x;  // delta time
  int x1; // start time
  int x2; // final time
  float y1, y2;

  // states
  final int QUIET = -1;
  final int RAMPUP  = 0;
  final int RAMPDOWN  = 1;
  int state;

  float y;

  Animator_Line ( int rampTime ) {
    t = rampTime>0.0?rampTime:1000;
    state = QUIET;
    y1 = y = 0.0;
  }

  void update() {
    if( state == RAMPUP ) {
      x = millis();
      y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
      if( y > y2 ) {
        y = y1 = y2;
        state = QUIET;
      }
    } else if( state == RAMPDOWN ) {
      x = millis();
      y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
      if( y < y2 ) {
        y = y1 = y2;
        state = QUIET;
      }
    }
  }

  void trigger(float target, int rampTime) {
    //println("target: " + target + "; rampTime: " + rampTime + ";");
    y2 = target;
    t  = rampTime;
    x1 = millis();
    x2 = x1 + t;
    y1 = y;
    if( y2 >= y1 )
      state = RAMPUP;
    else
      state = RAMPDOWN;
  }
  
  void fromTo(float start, float end, int rampTime) {
    y2 = end;
    t  = rampTime;
    x1 = millis();
    x2 = x1 + t;
    y1 = start;
    if( y2 >= y1 )
      state = RAMPUP;
    else
      state = RAMPDOWN;
  }

  float getY() {
    return y;
  }
}
