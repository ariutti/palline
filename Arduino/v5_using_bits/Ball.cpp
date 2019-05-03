#include "Ball.h"

extern float dist(uint8_t xA, uint8_t yA, uint8_t xB, uint8_t yB);

void Ball::init(int _nPalline, Ball* _ballRef, 
                   uint8_t _idx, 
                   float _x, float _y, float _w, float _h, 
                   uint8_t _LPB,
                   Adafruit_DotStar* _strip)
{
  nPalline = _nPalline;
  idx = _idx;
  posX = _x;
  posY = _y;
  planeW = _w;
  planeH = _h;
  float maxExpansion = calculateMaxExpansion();

  Serial.print("Ball ");
  Serial.print(idx);
  Serial.print(" - NCP: ");
  Serial.print( NCP );
  Serial.print(",  max expansion: ");
  Serial.print(maxExpansion);
  Serial.println(";");

  for(int i=0; i<NCP; i++) {
    cp[i] = new CircleParticle();
    cp[i]->init(_nPalline, _ballRef, idx, i, EXPANSION_SPEED, maxExpansion);
  }
  
  ar = new AnimLine();
  ar->init(ATK_TIME, RLS_TIME);

  strip = _strip;
  LPB = _LPB;
  tail = idx*LPB;
  head = tail + LPB - 1;

  if( DEBUG ) {
    Serial.print("; head: ");
    Serial.print(head);
    Serial.print("; tail: ");
    Serial.print(tail);
    Serial.println(";");
  }
  
  rawAmp = 0.0;

  // TODO: improve these two fucntions
  // they way them are used now in counter intuitive
  r = rawAmp * BASECOLOR_R;
  g = rawAmp * BASECOLOR_G;
  b = rawAmp * BASECOLOR_B;
  //setColor(r, g, b); 
  for(int j=0; j<LPB; j++) {
    strip->setPixelColor(tail+j, r, g, b);
  }
  //strip->show();
  if( DEBUG ) Serial.println();
}

void Ball::update()
{
  for(int i=0; i<NCP; i++)
  {
    cp[i]->update();
  }
  ar->update();
  
  if( DEBUG ) {
    Serial.print("Ball [");
    Serial.print(idx);
    Serial.print("] - ar status = ");
    ar->printState();
    Serial.print(" - rawAmp: ");
  }
  rawAmp = ar->getY();
  
  // better to square the 0.0 - 1.0 value in order to get a smoother transition
  rawAmp = rawAmp * rawAmp * BRIGHTNESS;
  //rawAmp = rawAmp * BRIGHTNESS;
  if( DEBUG ) Serial.print(rawAmp);
  
  r = rawAmp * BASECOLOR_R;
  g = rawAmp * BASECOLOR_G;
  b = rawAmp * BASECOLOR_B;

  if( DEBUG ) {
    Serial.print(" rgb:[");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.print(b);
    Serial.print("]");
  }
 
  if( DEBUG ) Serial.println();
}

void Ball::display()
{
  for(int j=0; j<LPB; j++)
  {
    strip->setPixelColor(tail+j, r, g, b );
  }
  //strip->show();
  //if(DEBUG) Serial.println();
}

void Ball::setPos(uint8_t _x, uint8_t _y)
{
  posX = _x;
  posY = _y;
}

void Ball::reached()
{
  ar->trigger();
}

void Ball::touched()
{
  if( DEBUG ) {
    Serial.print("ball ");
    Serial.print(idx);
    Serial.println(" touched!");
  }

  // show a flash light for the touched ball
  ar->trigger();

  // then make one of its circle particle explode.
  
  // Check if we have at least one circlo particle
  // which is ready to explode
  for(int i=0; i<NCP;i++) { 
    if( cp[i]->getStatus() == 0 ) // the circle particle is in IDLE state
    {
      cp[i]->explode();
      return;
    }
  }
  // if we are here it means every circle particle for the Ball is busy
  // so simply exit the function.
}

void Ball::printId()
{
  Serial.print("my name is ");
  Serial.println(idx);
}

int Ball::calculateMaxExpansion()
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
