class Ball
{
  
  // state machine
  final int IDLE = 0;
  final int BREATHING = 1;
  int status = IDLE;
  
  int nPalline;
  Ball ballRef[];
  int idx;
  
  PVector planeSize; // this represents the width and height of the wall
  //Animator_AR ar;
  Animator_ARv2 ar;
  
  PVector pos;
  float rawAmp;
  float brightness;
  
  int NCP = 1;
  CircleParticle cp[];
  
  float EXPANSION_SPEED = 4.5; //1.5 on Arduino
  
  int BASECOLOR_R = 255;
  int BASECOLOR_G = 255;
  int BASECOLOR_B = 255;
  int r,g,b;
 
  int prevTime;
  int TIMETOWAIT = 5000;
  float radius = 15;

  
  // CONSTRUCTOR ///////////////////////////////////////////////////////////////
  Ball(int _nPalline, Ball[] _ballRef, int _idx, float _x, float _y, float _w, float _h, float _brightness, int _t_atk, int _t_rls)  
  {
    nPalline = _nPalline;
    idx = _idx;
    pos = new PVector(_x, _y);
    planeSize = new PVector(_w, _h);
    float maxExpansion = calculateMaxExpansion();
    println("Ball " + idx + " initialization");
    //println("pallina " + idx + " max expansion: " + maxExpansion);
    
    //radius = _radius;
    
    cp = new CircleParticle[NCP];
    for(int i=0; i<cp.length;i++) { 
      cp[i] = new CircleParticle(_nPalline, _ballRef, idx, i, EXPANSION_SPEED, maxExpansion);
    }
    
    //ar = new Animator_AR(ACTIVE_ATK_TIME, ACTIVE_RLS_TIME);
    ar = new Animator_ARv2(ACTIVE_ATK_TIME, ACTIVE_RLS_TIME);
    brightness = _brightness;
    rawAmp = 0.0;
    
    r = int(rawAmp * BASECOLOR_R);
    g = int(rawAmp * BASECOLOR_G);
    b = int(rawAmp * BASECOLOR_B);    
  }
  

  // CALCULATE MAX EXPANSION ///////////////////////////////////////////////////
  float calculateMaxExpansion()
  {
    float d1 = dist(pos.x, pos.y, 0, 0);
    float d2 = dist(pos.x, pos.y, planeSize.x, 0);
    float d3 = dist(pos.x, pos.y, planeSize.x, planeSize.y);
    float d4 = dist(pos.x, pos.y, 0, planeSize.y);
    float winner = max(d1, d2);
    winner = max(winner, d3);
    winner = max(winner, d4);
    return winner;   
  }
  
  // UPDATE ////////////////////////////////////////////////////////////////////
  void update()
  {
    ar.update();
   
    rawAmp = ar.getY();
    rawAmp = rawAmp * rawAmp * brightness;
    r = int(BASECOLOR_R * rawAmp);
    g = int(BASECOLOR_G * rawAmp);
    b = int(BASECOLOR_B * rawAmp);
   
    for(int i=0; i<cp.length;i++) { 
      cp[i].update();
    }
  }
  
  // DISPLAY ///////////////////////////////////////////////////////////////////
  void display()
  {
    pushMatrix();
    pushStyle();
    translate(pos.x, pos.y);
    // Main Circle
    stroke(30);
    fill(r, g, b);
    ellipse(0,0, radius*2, radius*2);
    // ID
    fill(255);
    text(idx, 0, 0);
    
    // Circle particle
    noFill();
    stroke(120);
    for(int i=0; i<cp.length;i++) { 
      ellipse(0,0, cp[i].getRadius()*2, cp[i].getRadius()*2);
    }
    
    popStyle();
    popMatrix();
  }
  
  
  // INSIDE ////////////////////////////////////////////////////////////////////
  // used only in the Processing version because we are using mouse to simulate touch
  boolean inside(float x, float y)
  {
    if( dist(x, y, pos.x, pos.y)< radius)
      return true;
    return false;
  } 
  
  // SET POS ///////////////////////////////////////////////////////////////////
  void setPos( float _x, float _y)
  {
    pos.set(_x, _y);
  }
  
  // TOUCHED ///////////////////////////////////////////////////////////////////
  void touched()
  {
    brightness = ACTIVE_BRIGHTNESS;
    //println("Ball touched");
    ar.trigger(ACTIVE_ATK_TIME, ACTIVE_RLS_TIME);
    
    // Check if we have at least one circlo particle
    // which is ready to explode
    for(int i=0; i<NCP;i++) { 
      if( cp[i].getStatus() == 0 ) // the circle particle is in IDLE state
      {
        cp[i].explode();
        return;
      }
    }
  }
  
  // STANDBY TOUCHED ///////////////////////////////////////////////////////////
  // This is a different versione of the touched function which will be called
  // when the exhibit is in standby mode
  void standbyTouch() {
    brightness = STANDBY_BRIGHTNESS;
    ar.trigger( STANDBY_ATK_TIME, STANDBY_RLS_TIME );
  }
  
  // REACHED ///////////////////////////////////////////////////////////////////
  void reached()
  {
    brightness = ACTIVE_BRIGHTNESS;
    ar.trigger( ACTIVE_ATK_TIME, ACTIVE_RLS_TIME );
  }
  
  void released() {}
  
  void printId() {
    println("my name is " + idx);
  }
}
