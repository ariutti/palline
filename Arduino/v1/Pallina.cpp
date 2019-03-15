#include "Pallina.h"

extern float dist(int xA, int yA, int xB, int yB);

void Pallina::init(int _nPalline, Pallina* _pallinaRef, int _idx, float _radius, float _x, float _y, float _w, float _h)
{
  nPalline = _nPalline;
  idx = _idx;
  posX = _x;
  posY = _y;
  planeW = _w;
  planeH = _h;
  radius = _radius;
  float maxExpansion = calculateMaxExpansion();
  Serial.print("pallina ");
  Serial.print(idx);
  Serial.print(" max expansion: ");
  Serial.print(maxExpansion);
  Serial.println(";");
  cp = new CircleParticle();
  cp->init(_nPalline, _pallinaRef, idx, 6, maxExpansion);
  ar = new AnimAR();
  ar->init(500, 250);
  brightness = 0.0;
}

int Pallina::calculateMaxExpansion()
{
  float d1 = dist(posX, posY, 0, 0);
  float d2 = dist(posX, posY, planeW, 0);
  float d3 = dist(posX, posY, planeW, planeH);
  float d4 = dist(posX, posY, 0, planeH);
  float winner = max(d1, d2);
  winner = max(winner, d3);
  winner = max(winner, d4);
  return winner;
}

void Pallina::update()
{
  cp->update();
  ar->update();
  brightness = ar->getY();
  brightness = brightness * brightness * 255.0;
}

void Pallina::display()
{
  //pushMatrix();
  //pushStyle();
  //translate(pos.x, pos.y);
  //stroke(30);
  //fill(brightness);
  //ellipse(0,0, radius*2, radius*2);
  //fill(255);
  //text(idx, radius, radius);
  //noFill();
  //stroke(120);
  //ellipse(0,0, cp.getRadius()*2, cp.getRadius()*2);
  //popStyle();
  //popMatrix();
}

//boolean Pallina::inside()
//{
//  if( dist(x, y, posX, posY)< radius)
//    return true;
//  return false;
//}

void Pallina::setPos(int _x, int _y)
{
  posX = _x;
  posY = _y;
}

void Pallina::reached()
{
  ar->trigger();
}

void Pallina::touched()
{
  Serial.print("pallina ");
  Serial.print(idx);
  Serial.println(" touched!");
  
  cp->explode();
}

void Pallina::printId()
{
  Serial.print("my name is ");
  Serial.println(idx);
}
