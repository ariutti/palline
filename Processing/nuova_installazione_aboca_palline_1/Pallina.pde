final int IDLE = 0;
final int EXPANSION = 1;

class Pallina
{
  PVector pos;
  PVector planeSize; // this represents the width and height of the wall
  int idx;
  float radius, movingRadius;
  float maxExpansion; // this is the maximum available expansion for the radius
  Animator_AR ar;
  float brightness;
  int state;
  float expansionSpeed;
  int lastTimeTouchedBy = -1;
  
  Pallina(int _idx, float _x, float _y, float _w, float _h)
  {
    idx = _idx;
    pos = new PVector(_x, _y);
    planeSize = new PVector(_w, _h);
    radius = 10.0;
    movingRadius = 0.0;
    maxExpansion = calculateMaxExpansion();
    println("pallina " + idx + " max expansion: " + maxExpansion);
    ar = new Animator_AR(250, 250);
    brightness = 0.0;
    expansionSpeed = 10;
    state = IDLE;
    
  }
  
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
  
  void update()
  {
    
    println("pallina " + idx + " update! status= " + state + "; moving Radius: " + movingRadius);
    ar.update();
    if( ar.getStatus() == -1)
    {
      lastTimeTouchedBy = -1;
    }
    brightness = ar.getY();
    brightness = brightness * brightness * 255.0;
    
    if( state == EXPANSION ) {
      movingRadius = movingRadius + expansionSpeed;
      if(movingRadius >= maxExpansion) 
      {
        println("pallina " + idx + ", state: IDLE");
        state = IDLE;
        movingRadius = 0.0;
        
      }
    }
  }
  
  float getExpandingRadius()
  {
    return movingRadius;
  }
  
  int getStatus()
  {
    return state;
  }
  
  void display()
  {
    pushMatrix();
    pushStyle();
    translate(pos.x, pos.y);
    stroke(30);
    fill(brightness);
    ellipse(0,0, radius*2, radius*2);
    //fill(255);
    //text(idx, radius, radius);
    noFill();
    stroke(120);
    ellipse(0,0, movingRadius*2, movingRadius*2);
    popStyle();
    popMatrix();
  }
  
  boolean inside(float x, float y)
  {
    if( dist(x, y, pos.x, pos.y)< radius)
      return true;
    return false;
  } 
  
  void reached(int _emitterId)
  {
    println("pallina " + idx + " reached by " + _emitterId + " while last time was touched by " + lastTimeTouchedBy);
    if(_emitterId != lastTimeTouchedBy) 
    {
      println("pallina " + idx + " triggered!");
      ar.trigger();
      lastTimeTouchedBy = _emitterId;
    }
  }
  
  void touched()
  {
    println("pallina " + idx + " touched!");
    println("pallina " + idx + ", state: EXPANSION");
    state = EXPANSION;
  }
}
