#ifndef __PALLINA__
#define __PALLINA__
#include "Arduino.h"
#include "CircleParticle.h"
#include "AnimAR.h"

class CircleParticle;

class Pallina
{
private:
  
  int planeW, planeH; // these represents the size of the wall (in pixels??)
  int radius;
  int idx;
  int brightness;
  AnimAR* ar;
  CircleParticle* cp;
  int nPalline;
  Pallina* pallineRef;

public:

  int posX, posY; // position

  Pallina() {};
  void init(int _nPalline, Pallina* _pallinaRef, int _idx, float _radius, float _x, float _y, float _w, float _h);
  int calculateMaxExpansion();
  void update();
  void display();
  boolean inside();
  void setPos(int _x, int _y);
  void reached();
  void touched();
  void printId();
};


#endif
