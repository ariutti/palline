/*
 * An abstraction of a physical Ball object.
 * The ball is controlling two different aspects:
 * - Animation
 * - LED light
 * - Its own Circle Particle
 * Ball is controlled by the main program according to
 * the MPR121 capacitive sensors status.
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
#include "AnimARv2.h"

#include <Wire.h>
#include "Limulo_MPR121.h"

//#include "Adafruit_DotStar.h"
#include "Adafruit_NeoPixel.h"

// how many circle particle a ball will have?
#define NCP (1)

class CircleParticle;

class Ball
{
private:
  bool DEBUG_BALL = false;

  uint8_t nPalline;
  Ball* pallineRef;
  uint8_t idx;
  
  // GEOMETRY STUFF
  uint8_t planeW, planeH; // these represents the size of the wall (in pixels? in cm?)
  
  // ANIMATION & TIMING STUFF
  // 2020-10-01 changed the type of animator to be used
  //AnimAR* ar;
  AnimARv2* ar;
  
  // DISTRIBUTED BEHAVIOUR STUFF
  //CircleParticle* cp;
  CircleParticle* cp[NCP];
  
  // the speed of the expanding shokwave
  float EXPANSION_SPEED = 1.5; // 0.5
  
  // LED stuff
  //Adafruit_DotStar* strip;
  Adafruit_NeoPixel* strip;
  
  // how many leds are dedicated to the ball
  uint8_t LPB;
  // a variable to save output from Animator class
  float rawAmp;
  // indexes of first/last pixel of the block
  uint16_t head, tail;
  
  // a variable to change overall brightness of the LEDs
  float brightness;
  
  // variable to set the BASECOLOR (ie the color we chose for the LEDs light)
  uint8_t BASECOLOR_R = 255;
  uint8_t BASECOLOR_G = 255;
  uint8_t BASECOLOR_B = 255;
  // this are vars to keep track of changing colors
  uint8_t r;
  uint8_t g;
  uint8_t b;


public:

  //2020-10-01 
  // ANIMATION & TIMING STUFF
  //added some new timing variables to be used
  // in ACTIVE LOOP mode (see main loop)
  static const int ACTIVE_ATK_TIME = 250; // time will take for ligh to reach max brightness
  static const int ACTIVE_RLS_TIME = 500; // time will take for ligh to reach min brightness
  
  //added some new timing variables to be used
  // in STANDBY LOOP mode, the default one (see main loop)
  static const int STANDBY_ATK_TIME = 1000; // time will take for ligh to reach max brightness
  static const int STANDBY_RLS_TIME = 1000; // time will take for ligh to reach min brightness
  // similar thing for the brightness 
  static const float ACTIVE_BRIGHTNESS = 0.9;
  static const float STANDBY_BRIGHTNESS = 0.5;

  uint8_t posX, posY; // position

  Ball() {};
  void init(uint8_t _nPalline, Ball* _pallinaRef,
            uint8_t _idx,
            float _x, float _y, float _w, float _h,
            uint8_t _LPB,
            /*Adafruit_DotStar* _strip*/
            Adafruit_NeoPixel* _strip //,
            //float _brightness,
            //uint16_t _t_atk, uint16_t _t_rls
            );

  int calculateMaxExpansion();
  void update();
  void display();
  boolean inside();
  void setPos(uint8_t _x, uint8_t _y);

  // two methods to do quite the same thing. You will find that the two methods
  // are similar. Both are responsible to trigger the AR animation.
  // "touched" is called by the main loop while "reached" is called on another ball
  // by the expanding circle particle. The main difference is that "reached" will no
  // trigger any circle particle emission by the reached ball.
  void touched();
  void reached();

  // 2020-10-01
  // added a new funciont to be called when exhibit is in STANDBY MODE. 
  // See implementation for more detials and also the main loop.
  void standbyTouch();

  // This method is called by the main loop when there's a release on the corresponding
  // MPR pad. This method does nothing at all for the moment.
  void released() {};
  void printId();

  //2020-09-30
  //void changeBrightnessAndTimes(float _brightness, uint16_t _t_atk, uint16_t _t_rls );
};
#endif
