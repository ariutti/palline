/* 
 * 2019-03-14
 * first integration test using an Arduino Micro.
 * 
 * This sketch is using 5 different classes:
 * - AdaFruit DotStar: to menage LED strip;
 * - Limulo_MPR121: Adafruit_MPR121 modification to menage an MPR121 board;
 * - Ball: main abstraction of the physical Ball object;
 * - CircleParticle: each Ball has it's own Circle particle which will expand when the ball is touched to reach neighbour balls;
 * - AnimAR: an utility class to animate the light;
 * 
 * Each ball is placed inside an imaginary plane of WALL_W x WALL_H dimension and has
 * its own coordinates (xs, ys). Coordinates will be useful to calculate what ball
 * will be lit and when according to the expanding CircleParticle from a touched ball.
 * 
 * We have as many MPR121 capacitive pads as the number of Balls because 
 * each ball is made of capacitive stuff and will act as a pad itself.
 */


#include "Ball.h"
#define NPALLINE 48
Ball palline[NPALLINE];

// Leds Per Ball: how many LEDs ar dedicated to each ball
const uint8_t LPB = 7;

// this are the wall dimensions (in virtual units the don't correspond to centimeters)
#define WALL_W (100)
#define WALL_H (100)
// balls coords (created using a sepcial processing sketch)

float xs[] = {0.0,8.33,16.67,25.0,33.33,41.67,50.0,58.33,66.67,75.0,83.33,91.67,91.67,83.33,75.0,66.67,58.33,50.0,41.67,33.33,25.0,16.67,8.33,0.0,0.0,8.33,16.67,25.0,33.33,41.67,50.0,58.33,66.67,75.0,83.33,91.67,91.67,83.33,75.0,66.67,58.33,50.0,41.67,33.33,25.0,16.67,8.33,0.0,};
float ys[] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,7.5,7.5,7.5,7.5,7.5,7.5,7.5,7.5,7.5,7.5,7.5,7.5,15.0,15.0,15.0,15.0,15.0,15.0,15.0,15.0,15.0,15.0,15.0,15.0,22.5,22.5,22.5,22.5,22.5,22.5,22.5,22.5,22.5,22.5,22.5,22.5};

/* CAPACITIVE STUFF ****************************************************************/

#include <Wire.h>
#include "Limulo_MPR121.h"

const uint8_t NMPR = 4;
const uint8_t NPADS[] = {12, 12, 12, 12}; // NPALLINE
#define FIRST_MPR_ADDR 0x5A

struct mpr121
{
  Limulo_MPR121 cap;
  uint8_t addr;
  // Save an history of the pads that have been touched/released
  uint16_t lasttouched = 0;
  uint16_t currtouched = 0;
  uint16_t oor=0;
  boolean bPadState[12];
};

// an array of mpr121! You can have up to 4 on the same i2c bus
// mpr address can be one of these: 0x5A, 0x5B, 0x5C o 0x5D
mpr121 mpr[NMPR];

// LEDs /////////////////////////////////////////////////
// A basic everyday NeoPixel strip test program.

// NEOPIXEL BEST PRACTICES for most reliable operation:
// - Add 1000 uF CAPACITOR between NeoPixel strip's + and - connections.
// - MINIMIZE WIRING LENGTH between microcontroller board and first pixel.
// - NeoPixel strip's DATA-IN should pass through a 300-500 OHM RESISTOR.
// - AVOID connecting NeoPixels on a LIVE CIRCUIT. If you must, ALWAYS
//   connect GROUND (-) first, then +, then data.
// - When using a 3.3V microcontroller with a 5V-powered NeoPixel strip,
//   a LOGIC-LEVEL CONVERTER on the data line is STRONGLY RECOMMENDED.
// (Skipping these may work OK on your workbench but can fail in the field)

#include "Adafruit_NeoPixel.h"
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define DATAPIN 6   

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT (LPB*NPALLINE)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, DATAPIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)




/*
#include "Adafruit_DotStar.h"
#include <SPI.h>

// Here's how to control the LEDs from any two pins:
#define DATAPIN    5
#define CLOCKPIN   4
// How many LEDs we have along the strip
const uint8_t NUMLEDS = 24;

// in Adafruit DotStar library colors are ordered like this: GREEN, RED, BLUE
Adafruit_DotStar strip = Adafruit_DotStar(NUMLEDS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
*/


/* UTILITIES ***********************************************************************/
// have a pause between loops so it will be not overwelming
const int DELAY_TIME = 10;
// utility variables to dataviz values readed from MPR
boolean DEBUG_MAIN = true;
boolean bToPlotter = false;
boolean bToVVVV = false;
byte b;
int filt;
int base;


float dist(uint8_t xA, uint8_t yA, uint8_t xB, uint8_t yB)
{
  int A = xB - xA;
  int B = yB - yA;
  return sqrt( A*A + B*B );
}


/* SETUP ***************************************************************************/
void setup()
{
  Serial.begin(9600, SERIAL_8N1);
  //while(!Serial);
  if(DEBUG_MAIN) Serial.println("\n\nSerial ready!");

  // LED stuff
  strip.begin(); // Initialize pins for output
  
  // BALL stuff
  for(uint8_t i=0; i<NPALLINE; i++)
  {
    if(DEBUG_MAIN) {
      Serial.print("Inizializzazione pallina ");
      Serial.print(i);
      Serial.println(";");
    }
    palline[i].init(NPALLINE, palline, i, xs[i], ys[i], WALL_W, WALL_H, LPB, &strip);
  }

  
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
  // Turn all LEDs off ASAP
  strip.show(); 

  // CAPACITIVE STUFF **************************************************************/
  if(DEBUG_MAIN) Serial.println("Looking for MPRs!");
  // cycle through all the MPR
  for(uint8_t i=0; i<NMPR; i++)
  {
    mpr[i].cap = Limulo_MPR121();
    mpr[i].addr = FIRST_MPR_ADDR + i;

    // Look for the MPR121 chip on the I2C bus
    if ( !mpr[i].cap.begin( mpr[i].addr ) )
    {
      if( DEBUG_MAIN ) Serial.println("MPR121 not found, check wiring?");
      while (1);
    }
    if( DEBUG_MAIN ) Serial.print("MPR ");
    if( DEBUG_MAIN ) Serial.print(i); 
    if( DEBUG_MAIN ) Serial.println(" found!");

    // initialize the array of booleans
    for(int j=0; j<12; j++) {
      mpr[i].bPadState[j] = false;
    }

    // possibly initialize the mpr with some initial settings
    mpr[i].cap.setUSL(201);
    mpr[i].cap.setTL(180);
    mpr[i].cap.setLSL(130);
    
    // First Filter Iteration
    // Second Filter Iteration
    // Electrode Sample Interval
    // NOTE: the system seems to behave 
    // better if these value are more than 0
    mpr[i].cap.setFFI_SFI_ESI(1, 1, 1);  // See AN3890

    // MHD, NHD, NCL, FDL
    mpr[i].cap.setFalling( 1, 1, 2, 1 );
    mpr[i].cap.setRising( 1, 8, 1, 1 );
    mpr[i].cap.setTouched( 1, 1, 1 );
    mpr[i].cap.setThresholds( 24, 10 );
    mpr[i].cap.setDebounces(2, 2);
  }
}


/* LOOP ****************************************************************************/
void loop()
{
  //getSerialData();
  
  // CAPACITIVE STUFF **************************************************************/
  // cycle through all the MPRs
  for(uint8_t i=0; i<NMPR; i++)
  {
    // Get the currently touched pads
    mpr[i].currtouched = mpr[i].cap.touched(); 
    
    // cycle through all the electrodes
    for(uint8_t j=0; j<NPADS[i]; j++)
    {
      if (( mpr[i].currtouched & _BV(j)) && !(mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'j' has been touched 
        mpr[i].bPadState[j] = true;
        uint8_t index = composeIndex(i, j);
        palline[ index ].touched();
        
        if( bToVVVV ) printAllSensors(); // Send serial data to VVVV
      }
      if (!(mpr[i].currtouched & _BV(j)) && (mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'i' has been released
        mpr[i].bPadState[j] = false;
        uint8_t index = composeIndex(i, j);
        palline[ index ].released();
        
        if( bToVVVV ) printAllSensors(); // Send serial data to VVVV
      }
    }
    // reset our state
    mpr[i].lasttouched = mpr[i].currtouched;

    mpr[i].oor = mpr[i].cap.getOOR();

    
    if(bToPlotter)
    {
      /*
      // ### NEW COMMUNICATION PROTOCOL (19-02-2018) ###
      //
      // Send data via serial:
      // 1. 'Status Byte': first we send a byte containing the address of the mpr.
      //    The most significant bit of the byte is 1 (value of the byte is > 127).
      //    This is a convention in order for the receiver program to be able to recognize it;
      // 2. Then we send 'Data Bytes'. The most significant bit of these bytes is 
      //    always 0 in order to differenciate them from the status byte.
      //    We can send as many data bytes as we want. The only thing to keep in mind
      //    is that we must be coherent the receiver side in order not to create confusion
      //    in receiving the data.
      //
      //    For instance we can send pais of byte containing the 'baseline' and 'filtered'
      //    data for each mpr pad. 
      //
      //    We can also use data bytes for sending information as:
      //    * 'touched' register;
      //    * 'oor' register;


      // 1. write the status byte containing the mpr addr
      b = (1<<7) | i;
      Serial.write(b);
      // 2. write 'touched' register
      b = mpr[i].currtouched & 0x7F; 
      Serial.write(b); //touch status: pad 0 - 6
      b = (mpr[i].currtouched>>7) & 0x7F;
      Serial.write(b); //touch status: pad 7 - 12 (eleprox)
      // 3. write 'oor' register
      b = mpr[i].oor & 0x7F; 
      Serial.write(b); //oor status: pad 0 - 6
      b = (mpr[i].oor>>7) & 0x7F;
      Serial.write(b); //oor status: pad 7 - 12 (eleprox)

      // Cycle all the electrodes and send pairs of
      // 'baseline' and 'filtered' data. Mind the bit shifting!
      for(uint8_t j=0; j<NPADS; j++)
      {
        base = mpr[i].cap.baselineData(j); 
        filt = mpr[i].cap.filteredData(j);
        Serial.write(base>>3); // baseline is a 10 bit value
        Serial.write(filt>>3); // sfiltered is a 10 bit value
      }
      */
      // Send data via serial:
      // 1. First send a byte containing the address of the mpr + the address of the pad +
      //    the 'touched' status of the pad; This byte has a value greater than 127 by convention;
      // 2. Then send two more bytes for 'baseline' and 'filtered' respectively. 
      //    Because these values are 10bit values and we must send them
      //    inside a 7bit packet, we must made a 3 times bit shift to the right (/8).
      //    This way we will loose some precision but this is not important.
      //    This two other bytes have values lesser than 127 by convention.

      // cycle all the electrodes
      for(int j=0; j<NPADS[i]; j++)
      {
        filt = mpr[i].cap.filteredData(j);
        base = mpr[i].cap.baselineData(j);
        b = (1<<7) | (i<<5) | (j<<1) | mpr[i].bPadState[j];
        Serial.write(b); // send address & touched
        Serial.write(base / 8); // send base value
        Serial.write(filt / 8); // send filtered value
      }
            
    } // if(bToPlotter)
    //mpr[i].cap.printOOR(); // added for debug purposes
  } //   for(int i=0; i<NMPR; i++)

  // PALLINE STUFF
  for(uint8_t i=0; i<NPALLINE; i++) {
    palline[i].update();
  }
  for(uint8_t i=0; i<NPALLINE; i++) {
    palline[i].display();
  }
  strip.show();

  // put a delay so it isn't overwhelming
  delay(DELAY_TIME); 
}

// GET SERIAL DATA

/*
void getSerialData()
{
  while (Serial.available() > 0) 
  {
    char c = Serial.read();
    // the message from VVVV is finished
    if (c == '\n') //;
    {
      //Serial.print( temp );
      //Serial.print( " - " );
      //Serial.println( temp.length() );
      if( temp.length() == 1 )
      {
        if (temp[0] == 'o')
        {
          bToPlotter = true;
          temp = "";
        }
        else if (temp[0] == 'c')
        {
          bToPlotter = false;
          temp = "";
        }
        else if( 'a' ) {
          bToPlotter = false;
          bToVVVV = true;
        }
        else if( 'b' ) {
          bToVVVV = false;
        }
        else if (c == 'r')
        {
          // reset all the MPR
          for(int i=0; i<NMPR; i++)
            mpr[i].cap.reset();
        }
        else
        {
          temp = "";
        } 
      }
      else
      {
        temp = "";
      }
    } 
    else
    {
      // if the message from Processing is not finished yet,
      // append character to 'temp' string
      temp += c;
    }
  }

 



  
  while (Serial.available() > 0) 
  {
    byte c = Serial.read();
    // Processing can ask Arduino to send some raw
    // data in order to get a plotted version of them
    if (c == 'o') {
      bToPlotter = true;
    }
    else if (c == 'c') {
      bToPlotter = false;
    }
    // V4 can open or close the serial communication with Arduino
    else if( 'a' ) {
      bToPlotter = false;
      bToVVVV = true;
    }
    else if( 'b' ) {
      bToVVVV = false;
    }
    else if (c == 'r')
    {
      // reset all the MPR
      for(int i=0; i<NMPR; i++)
        mpr[i].cap.reset();
    }
  }
}
*/


/************************************************************************************
 * COMPOSE INDEX
 ***********************************************************************************/
int composeIndex(int mprIndex, int padIndex) {
  int acc = 0;
  for(int i=0; i<mprIndex; i++) {
    acc += NPADS[i];
  }
  return acc + padIndex;  
}


/************************************************************************************
 * SERIAL EVENT
 ***********************************************************************************/
void serialEvent()
{
  byte c = Serial.read();
  // Processing can ask Arduino to send some raw
  // data in order to get a plotted version of them
  if (c == 'o') {
    bToPlotter = true;
  }
  else if (c == 'c') {
    bToPlotter = false;
  }
  /*
  // V4 can open or close the serial communication with Arduino
  else if( 'a' ) {
    bToPlotter = false;
    bToV4 = true;
  }
  else if( 'b' ) {
    bToV4 = false;
  }
  */
  else if (c == 'r')
  {
    // reset all the MPR
    for(int i=0; i<NMPR; i++)
      mpr[i].cap.reset();
  }
}


/************************************************************************************
 * PRINT ALL SENSORS
 ***********************************************************************************/
void printAllSensors()
{
  // cycle through all the mpr
  for(uint8_t i=0; i<NMPR; i++)
  {
    // cycle through all the electrodes
    for(uint8_t j=0; j<NPADS[i]; j++)
    {
      int state = (mpr[i].currtouched & _BV(j)) >> j;
      if( DEBUG_MAIN ) Serial.print( state );
    }
    if( DEBUG_MAIN ) Serial.println(";");
  }
}
