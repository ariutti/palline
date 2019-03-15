/*
 * An abstraction of a physical Ball object.
 * The ball is controlling two different aspects:
 * - Animation
 * - LED light
 * - Its own Circle Particle
 * Ball is controlled itself by the main program according to
 * the MPR121 capacitive sensors stataus.
 * 
 * Each touch on a ball will trigger:
 * - child Animator to make its attack/release animation;
 * - child CircleParticle to explode;
 * 
 * You can control: 
 * - ATTACK and RELEASE timing for the child Animator
 * - child CircleParticle EXAPNASION_SPEED
 * - overall light BRIGHTNESS
 * - light BASECOLOR red/green/blue
 */

#ifndef __PALLINA__
#define __PALLINA__
#include "Arduino.h"
#include "CircleParticle.h"
#include "AnimAR.h"

#include <Wire.h>
#include "Limulo_MPR121.h"

#include "Adafruit_DotStar.h"
extern bool DEBUG;

class CircleParticle;

class Ball
{
private:
  int nPalline;
  Ball* pallineRef;
  int idx;
  // GEOMETRY STUFF
  int planeW, planeH; // these represents the size of the wall (in pixels? in cm?)
  // ANIMATION & TIMING STUFF
  int ATK_TIME = 250; // time will take for ligh to reach max brightness
  int RLS_TIME = 500; // time will take for ligh to reach min brightness
  AnimAR* ar;
  // DISTRIBUTED BEHAVIOUR STUFF
  CircleParticle* cp;
  // the speed of the expanding shokwave
  float EXPANSION_SPEED = 0.5;
  // LED stuff
  Adafruit_DotStar* strip;
  // how many leds are dedicated to the ball
  int LPB;
  // a variable to save output from Animator class
  float rawAmp;
  // indexes of first/last pixel of the block
  uint8_t head, tail; 
  // a variable to change overall brightness
  float BRIGHTNESS = 0.5;
  // variable to set the BASECOLOR (ie the color we chose for the LEDs light)
  uint8_t BASECOLOR_R = 255;
  uint8_t BASECOLOR_G = 0;
  uint8_t BASECOLOR_B = 0;
  // this are vars to keep track of changing colors
  uint8_t r;
  uint8_t g;
  uint8_t b;


public:
  int posX, posY; // position

  Ball() {};
  void init(int _nPalline, Ball* _pallinaRef, 
            int _idx, 
            float _x, float _y, float _w, float _h,
            int _LPB,
            Adafruit_DotStar* _strip);
  int calculateMaxExpansion();
  void update();
  void display();
  boolean inside();
  void setPos(int _x, int _y);
  void reached();
  void touched();
  void released() {};
  void printId();
};


#endif
