class Animator_AR
{
  // x axis represents time
  int t;     // utility var: time to reach target value (it can be either atk_time or rls_time)
  long x;    // utility var: keeps track of the passing time
  float y;   // utility var: interpolated value between y1 and y2

  int idx;
  int t_atk;  // t attack,
  int t_rls;  // t release
  long x1;    // start time
  long x2;    // end time
  float y1;   // start value
  float y2;   // end value

  // 2020-09-30: a couple of variables used to keep track of
  // evntually changed time preference by the user.
  int next_t_atk;
  int next_t_rls;

  // states
  final int AR_QUIET = -1;
  final int AR_ATTACK = 0;
  final int AR_RELEASE= 1;
  int state;

  // CONSTRUCTOR ///////////////////////////////////////////////////////////////
  Animator_AR ( int _idx, int _t_atk, int _t_rls ) {
    idx = _idx;
    t_atk = _t_atk>0.0?_t_atk:10;
    t_rls = _t_rls>0.0?_t_rls:500;
    reset();
    //state = CP_QUIET;
    //y1 = y = 0.0;
    //y2 = 1.0; // redundant
  }


  // UPDATE ////////////////////////////////////////////////////////////////////
  void update() {
    if ( state == AR_ATTACK ) {
      x = millis();
      y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
      if ( y > y2 ) {
        y = y1 = y2;
        // set the state machine to begin relase phase
        y2 = 0.0;
        t  = t_rls;
        x1 = millis();
        x2 = x1 + t;
        y1 = y;
        state = AR_RELEASE;
      }
    } else if ( state == AR_RELEASE ) {
      x = millis();
      y = (x - x1) * ( (y2 - y1)/((x2 - x1)*1.0) ) + y1;
      if ( y < y2 ) {
        y = y1 = y2;
        state = AR_QUIET;
        // when we change to the QUIET status, better to do a check 
        // if we have to change times to the desired timing.
        if ( next_t_atk != t_atk || next_t_rls != t_rls) {
        println("AR " + idx +": reached only now the QUIET status, is time to change atk and rls");
        if ( next_t_atk != t_atk )
          t_atk = next_t_atk;
        if ( next_t_rls != t_rls)
          t_rls = next_t_rls;
      }
      }
    }
  }
  
  // TRIGGER ///////////////////////////////////////////////////////////////////
  void trigger() {
    println("AR " + idx + ", I've been triggered");
    //println("target: " + target + "; rampTime: " + rampTime + ";");
    if(y == 1.0)
      return;
    y2 = 1.0;
    t  = t_atk;
    x1 = millis();
    x2 = x1 + t;
    y1 = y;
    state = AR_ATTACK;
  }
  
  void reset() {
    println("AR " + idx +": I've been resetted");
    state = AR_QUIET;
    y1 = y = 0.0;
    y2 = 1.0; // redundant
  }
  
  
  // GET Y /////////////////////////////////////////////////////////////////////
  float getY() {
    return y;
  }

  // GET STATUS ////////////////////////////////////////////////////////////////
  int getState()
  {
    return state;
  }


  // SET TIMES /////////////////////////////////////////////////////////////////
  //2020-09-30: 
  // A new instance method added to make possibile
  // to change attack and release phase time changable by the user.
  // I want to prevent times to be changed while the animator is in its "active" states.
  // So I've created thwo new variable to keep track of the user preferences for the new timings
  // I will change times when animator will pass from active to QUIET state.
  void setTimes(int _t_atk, int _t_rls) {
    next_t_atk = _t_atk>0.0?_t_atk:10;
    next_t_rls = _t_rls>0.0?_t_rls:500;

    // check if we already are in the QUIET status
    // is so, update the timing right now. Alternatively
    // let the update method do the job when passing
    // to the QUIET status (see update)
    if ( state == AR_QUIET ) {
      
      if ( next_t_atk != t_atk || next_t_rls != t_rls) {
        println("AR " + idx +": change time immediately");
        if ( next_t_atk != t_atk )
          t_atk = next_t_atk;
        if ( next_t_rls != t_rls)
          t_rls = next_t_rls;
      }
    }
  }
  
  // remember:
  //const int AR_QUIET = -1;
  //const int AR_ATTACK  = 0;
  //const int AR_RELEASE = 2;
  void printState()
  {
    switch(state) {
      case AR_QUIET:
        print("QUIET");
        break;
      case AR_ATTACK:
        print("ATTACK");
        break;
      case AR_RELEASE:
        print("RELEASE");
        break;
      default:
        break;
    }
    //Serial.println();
  }
}
