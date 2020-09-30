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
  //Animator_Sine sine;
  //Animator_Line line;
  Animator_AR ar;
  
  PVector pos;
  float rawAmp;
  float brightness;
  float next_brightness;
  
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
  // float _radius, float _freq, float _iPhase
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
    
    ar = new Animator_AR(idx, 250, 500);
    brightness = _brightness;
    next_brightness = brightness;
    rawAmp = 0.0;
    
    r = int(rawAmp * BASECOLOR_R);
    g = int(rawAmp * BASECOLOR_G);
    b = int(rawAmp * BASECOLOR_B);
    
    //sine = new Animator_Sine( _freq, _iPhase );
    //line = new Animator_Line(2000);
    //prevTime = millis();
    
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
    //line.update();    
    //sine.update(); 
    ar.update();
    if( ar.getState() == -1 ) {
      if(next_brightness != brightness) {
        brightness = next_brightness;
      }
    }
    
    
    rawAmp = ar.getY();
    //print(rawAmp); print(", "); ar.printState(); println();
    rawAmp = rawAmp * rawAmp * brightness;
    r = int(BASECOLOR_R * rawAmp);
    g = int(BASECOLOR_G * rawAmp);
    b = int(BASECOLOR_B * rawAmp);
    //print(r); print(",");print(g); print(",");print(b); print(",");
    
    
    for(int i=0; i<cp.length;i++) { 
      cp[i].update();
    }
    
    //float y = 0;
    //if(status == IDLE && millis()-prevTime<= TIMETOWAIT) {
    //  y = ar.getY();
    //}
    //else if( status == IDLE && millis()-prevTime > TIMETOWAIT )
    //{
    //  // triggera l'animazione della linea 
    //  // la cui y moltiplica la y del seno
    //  prevTime = millis();
    //  status = BREATHING;
    //  //resetLine();
    //  //y = line.getY() * sine.getY();
    //} else if( status == BREATHING )
    //{
    //  y = line.getY() * sine.getY();
    //  y = map(y, 0.0, 1.0, 0.2, 0.7);
    //}
    
    //brightness = y * y * 255.0;
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
    //println("Ball touched");
    ar.trigger();
    
    
    // Check if we have at least one circlo particle
    // which is ready to explode
    for(int i=0; i<NCP;i++) { 
      if( cp[i].getStatus() == 0 ) // the circle particle is in IDLE state
      {
        cp[i].explode();
        return;
      }
    }
    //println("pallina " + idx + " touched!");
    //prevTime = millis();
    //status = IDLE;
    
    //// if we are here it means every cp for the Ball is busy
    //// so simply exit the function
  }
  
  // REACHED ///////////////////////////////////////////////////////////////////
  void reached()
  {
    //println("Ball reached");
    //cp.explode();
    ar.trigger();
    
    //// start the timer to switch to IDLE / BREATHING statuses
    //prevTime = millis();
    //status = IDLE;
  }
  
  void released() {}
  
  
  void printId()
  {
    println("my name is " + idx);
  }
  
  //void resetLine() {
  //  line.fromTo(0, 1, 2000);
  //}
  
  void changeBrightnessAndTimes(float _brightness, int _t_atk, int _t_rls ) 
  {
    next_brightness = _brightness;
    ar.setTimes( _t_atk, _t_rls );
  }  
  
  void reset() {
    println("Ball " + idx + " resetted!");
    ar.reset();
  }
  

//  float getX() 
//  {
//    return pos.x;
//  }
  
//  float getY()
//  {
//    return pos.y;
//  }
}
