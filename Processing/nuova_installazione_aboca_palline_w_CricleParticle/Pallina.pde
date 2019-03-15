class Pallina
{
  PVector pos;
  PVector planeSize; // this represents the width and height of the wall
  int idx;
  float radius;
  Animator_AR ar;
  float brightness;
  CircleParticle cp;
  Pallina pallineRef[];
  
  Pallina(Pallina[] _pallinaRef, int _idx, float _radius, float _x, float _y, float _w, float _h)
  {
    idx = _idx;
    pos = new PVector(_x, _y);
    planeSize = new PVector(_w, _h);
    radius = _radius;
    float maxExpansion = calculateMaxExpansion();
    println("pallina " + idx + " max expansion: " + maxExpansion);
    cp = new CircleParticle(_pallinaRef, idx, 6, maxExpansion);
    // TODO: give this information to the Circle Particle
    
    ar = new Animator_AR(500, 250);
    brightness = 0.0;
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
    cp.update();
    ar.update();
    brightness = ar.getY();
    brightness = brightness * brightness * 255.0;
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
    //noFill();
    //stroke(120);
    //ellipse(0,0, cp.getRadius()*2, cp.getRadius()*2);
    popStyle();
    popMatrix();
  }
  
  boolean inside(float x, float y)
  {
    if( dist(x, y, pos.x, pos.y)< radius)
      return true;
    return false;
  } 
  
  void setPos( float _x, float _y)
  {
    pos.set(_x, _y);
  }
  
  void reached()
  {
    //cp.explode();
    ar.trigger();
  }
  
  void touched()
  {
    println("pallina " + idx + " touched!");
    cp.explode();
  }
  void printId()
  {
    println("my name is " + idx);
  }
}
