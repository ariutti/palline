#include "Ball.h"

// see main file to look at the implementation
extern float dist(uint8_t xA, uint8_t yA, uint8_t xB, uint8_t yB);

// BALL INIT
// Each Ball needs:
// * the total number of balls of the exhibit;
// * a reference to this very same balls array (in order for the ball to talk to other balls);
// * the index of the ball;
// * the x and y coordinates of the ball inside the wall;
// * the overall wall dimensions;
// * the number of LEDs the ball is using (this is a constant);
// * a reference to the LED strip;
void Ball::init(int _nPalline, Ball* _ballRef,
                   uint8_t _idx,
                   float _x, float _y, float _w, float _h,
                   uint8_t _LPB,
                   /*Adafruit_DotStar* _strip*/
                   Adafruit_NeoPixel* _strip)
{
  nPalline = _nPalline;
  idx = _idx;
  posX = _x;
  posY = _y;
  planeW = _w;
  planeH = _h;
  float maxExpansion = calculateMaxExpansion();

  if( DEBUG_BALL ) {
    Serial.print("Ball ");
    Serial.print(idx);
    Serial.print(" - NCP: ");
    Serial.print( NCP );
    Serial.print(",  max expansion: ");
    Serial.print(maxExpansion);
    Serial.println(";");
  }

  // once we know the max expansion for the ball, we can create the particle system
  // which will emit the expanding circle particle @ every touch by the user.
  for(int i=0; i<NCP; i++) {
    cp[i] = new CircleParticle();
    cp[i]->init(_nPalline, _ballRef, idx, i, EXPANSION_SPEED, maxExpansion);
  }

  // Each ball uses an support Animator class in order to make some timed brightness
  // adjustment of its LEDs according to 2 different phases with two separate timing.
  // * attack time;
  // * release time;
  ar = new AnimLine();
  ar->init(STANDBY_ATK_TIME, STANDBY_RLS_TIME);

  // Keep internally a reference to the LED strip
  // and calculate the index of the first an last LEDs of this
  // which the ball is in charge of.
  strip = _strip;
  LPB = _LPB;
  tail = idx*LPB;
  head = tail + LPB - 1;

  if( DEBUG_BALL ) {
    Serial.print("; head: ");
    Serial.print(head);
    Serial.print("; tail: ");
    Serial.print(tail);
    Serial.println(";");
  }

  rawAmp = 0.0;

  brightness = STANDBY_BRIGHTNESS;

  // TODO: improve these two functions
  // they way them are used now in counter intuitive
  r = rawAmp * BASECOLOR_R;
  g = rawAmp * BASECOLOR_G;
  b = rawAmp * BASECOLOR_B;
  //setColor(r, g, b);
  for(int j=0; j<LPB; j++) {
    strip->setPixelColor(tail+j, r, g, b);
  }
  //strip->show();
  if( DEBUG_BALL ) Serial.println();
}


// BALL UPDATE
// Every time we update the Ball we are:
// * updating its animator and, according to the animator output, we are calculating
//   a new brightness value to be used later for the ball LEDs (see "display" method);
// * we are also updating all the Ball's circle particles in order for them to
//   expand and do their stuff;

void Ball::update()
{
  ar->update();

  if( DEBUG_BALL ) {
    Serial.print("Ball [");
    Serial.print(idx);
    Serial.print("] - ar status = ");
    ar->printState();
    Serial.print(" - rawAmp: ");
  }

  rawAmp = ar->getY();

  // better to square the 0.0 - 1.0 value in order to get a smoother transition
  //rawAmp = rawAmp * rawAmp * BRIGHTNESS;
  // think we can use an MAIN overall variable for brigthness
  rawAmp = rawAmp * rawAmp * brightness;

  if( DEBUG_BALL ) Serial.print(rawAmp);

  r = rawAmp * BASECOLOR_R;
  g = rawAmp * BASECOLOR_G;
  b = rawAmp * BASECOLOR_B;

  if( DEBUG_BALL ) {
    Serial.print(" rgb:[");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.print(b);
    Serial.print("]");
  }

  if( DEBUG_BALL ) Serial.println();

  // update all the ball circle particles
  for(int i=0; i<NCP; i++)
  {
    cp[i]->update();
  }
}


// BALL DISPLAY
// The ball display function simply apply the brightness calculation from the
// "update" to the actual ball LEDs.
void Ball::display()
{
  for(int j=0; j<LPB; j++) {
    strip->setPixelColor(tail+j, r, g, b );
  }
}



void Ball::setPos(uint8_t _x, uint8_t _y)
{
  posX = _x;
  posY = _y;
}

// BALL REACHED
// This method is called by the Circle Particle when its expanding radius reaches
// the ball.
void Ball::reached()
{
  ar->trigger();
}



// BALL TOUCHED
// When the user touches the ball, this method is called by the main loop.
// This method is responsible for:
// * triggering the ATTACK phase of the internal animator for the ball;
// * explode a new circle particle (if there is one of them which is in IDLE status);
void Ball::touched()
{
  if( DEBUG_BALL ) {
    Serial.print("ball ");
    Serial.print(idx);
    Serial.println(" touched!");
  }

  // 2020-09-29:
  setActiveBrightnessAndTimes();

  // show a flash light for the touched ball
  ar->trigger();

  // then make one of its circle particle explode.

  // Check if we have at least one circle particle
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



// Max expansion is a distance we will use to decide when to destroying the circle Particle
// once it has expandend more than needed. In order to calcultate how large this circle
// particle can be, we first need to find which is the maximum distance among the four
// distances we have from the ball and the wall corners.
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

// 2020-09-29
// Two new functions to change the brightness and AR timing for the balls
void Ball::setActiveBrightnessAndTimes() {
  // change the brightness to the ACTIVE BRIGHTNESS
  brightness = ACTIVE_BRIGHTNESS;
  // 2020-09-29:
  // use active times instead of stanby times
  ar->setTimes( ACTIVE_ATK_TIME, ACTIVE_RLS_TIME );
}

void Ball::setStandbyBrightnessAndTimes() {
  // change the brightness to the STANDBY BRIGHTNESS
  brightness = STANDBY_BRIGHTNESS;
  // 2020-09-29:
  // use standby times instead of active times
  ar->setTimes( STANDBY_ATK_TIME, STANDBY_RLS_TIME );
}
