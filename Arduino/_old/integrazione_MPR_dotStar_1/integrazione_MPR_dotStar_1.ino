/*
* This sketch is meant to work with the
* "capacitive_plotter_3" sketch for Processing.
* 
* Mind that Arduino micro doesn't have SerialEvent method.
*/

#include <Wire.h>
#include "Limulo_MPR121.h"

#define FIRST_MPR_ADDR 0x5A
const int NMPR = 1;
const int NPADS[] = {1};// {1, 1, 3, 2};



struct mpr121
{
  Limulo_MPR121 cap;
  uint8_t addr;
  // Save an history of the pads that have been touched/released
  uint16_t lasttouched = 0;
  uint16_t currtouched = 0;
  boolean bPadState[12];
};


// an array of mpr121! You can have up to 4 on the same i2c bus
mpr121 mpr[NMPR];

// A boolean utility variable to print via serial 
// only when something is changed
// If data are going to the plotter they will be not sent 
// in the form of 0000100;
// and viceversa.


// LEDs /////////////////////////////////////////////////
#include "Adafruit_DotStar.h"
#include <SPI.h>

// Here's how to control the LEDs from any two pins:
#define DATAPIN    5
#define CLOCKPIN   4
// How many LEDs we have along the strip
const int NUMLEDS = 3;
// Leds Per Ball: how many LEDs ar dedicated to each ball
const int LPB = 3;
// in Adafruit DotStar library colors are ordered like this: GREEN, RED, BLUE
Adafruit_DotStar strip = Adafruit_DotStar(NUMLEDS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);


uint8_t color_r = 0, color_g = 0, color_b = 0;


// some booolean variables
boolean bDebug = true;
boolean bToV4 = true;
// the Serial plotter boolean will be set via Processing
boolean bToPlotter;
// used to share messages via serial communication
String temp; 

int filt;
int base;
byte b;
int delayTime = 10;

/************************************************************************************
 * SETUP
 ***********************************************************************************/
void setup()
{
  
  Serial.begin(9600, SERIAL_8N1);
  while(!Serial);
  if(bDebug) Serial.println("Serial ready!");
  if(bDebug) Serial.println("Ricerca MPR iniziata");

  
  // cycle through all the MPR
  for(int i=0; i<NMPR; i++)
  {
    mpr[i].cap = Limulo_MPR121();
    // mpr address can be one of these: 0x5A, 0x5B, 0x5C o 0x5D
    mpr[i].addr = FIRST_MPR_ADDR + i;

    // Look for the MPR121 chip on the I2C bus
    if ( !mpr[i].cap.begin( mpr[i].addr ) )
    {
      if(bDebug) Serial.print("MPR121 ");
      if(bDebug) Serial.print(i);
      if(bDebug) Serial.println(" not found, check wiring?");
      
      while (1);
    }
    if(bDebug) Serial.print("MPR121 ");
    if(bDebug) Serial.print(i);
    if(bDebug) Serial.println(" ready!");

    // initialize the array of booleans
    for(int j=0; j<12; j++)
    {
      mpr[i].bPadState[j] = false;
    }

    mpr[i].cap.setUSL(201);
    mpr[i].cap.setTL(180);
    mpr[i].cap.setLSL(130);

    // possibly initialize the mpr with some initial settings
    // mhd, nhd, ncl, fdl
    mpr[i].cap.setFalling(1, 1, 2, 1);
    //mpr[i].cap.setFalling(4, 2, 16, 2);
    mpr[i].cap.setRising( 1, 8, 1, 0);
    mpr[i].cap.setTouched( 0, 0, 0);
    
    mpr[i].cap.setThresholds( 24, 5 );
    //mpr.setDebounces(1, 1);
  }

  if( bDebug ) Serial.println("LED initialization");
  strip.begin(); // Initialize pins for output
  strip.show();
  
 
  bToPlotter = false;
  if(bDebug) Serial.println("fine setup");
}

/************************************************************************************
 * LOOP
 ***********************************************************************************/
void loop()
{
  // get serial information
  getSerialData(); 

  // cycle through all the MPR
  for(int i=0; i<NMPR; i++)
  {
    mpr[i].currtouched = mpr[i].cap.touched(); // Get the currently touched pads
    // cycle through all the electrodes
    for(int j=0; j<NPADS[i]; j++)
    {
      if (( mpr[i].currtouched & _BV(j)) && !(mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'i' has been touched
        //Serial.println("Pad 0:\ttouched!");
        mpr[i].bPadState[j] = true;

        if( bToV4 ) printAllSensors(); // Send serial data to VVVV
        if( i == 0 )
          changeColor( 1 );
        
      }
      if (!(mpr[i].currtouched & _BV(j)) && (mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'i' has been released
        //Serial.println("Pad 0 :\treleased!");
        mpr[i].bPadState[j] = false;
        
        if( bToV4 ) printAllSensors(); // Send serial data to VVVV
        if( i == 0 )
          changeColor( 0 );
      }
      
    }
    // reset our state
    mpr[i].lasttouched = mpr[i].currtouched;

    
    // Send Serial data ////////////////////////////////////////////////////////////
    if(bToPlotter)
    {
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
    

  } //  for(int i=0; i<NMPR; i++)

  for(int i=0; i<LPB;i++) {
    strip.setPixelColor(i, color_r, color_g, color_b );
  }
  strip.show();
  
  
  delay(delayTime); // put a delay so it isn't overwhelming
}

void changeColor( int colorType ) 
{
  if(colorType == 1)
  {
    color_r = 255;
    color_g = 255;
    color_b = 255;
  }
  else
  {
    color_r = 0;
    color_g = 0;
    color_b = 0;
  }

}


/************************************************************************************
 * SERIAL EVENT
 ***********************************************************************************
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
  // V4 can open or close the serial communication with Arduino
  else if( 'a' ) {
    bToPlotter = false;
    bToV4 = true;
  }
  else if( 'b' ) {
    bToV4 = false;
  }
  else if (c == 'r')
  {
    // reset all the MPR
    for(int i=0; i<NMPR; i++)
      mpr[i].cap.reset();
  }
}
*/

/************************************************************************************
 * SERIAL DATA
 ***********************************************************************************/
void getSerialData()
{
  while (Serial.available() > 0) 
  {
    char c = Serial.read();
    // the message from VVVV is finished
    if (c == '\n')
    {
      //Serial.print( temp );
      //Serial.print( "(" );
      //Serial.print( temp.length() );
      //Serial.println( ")" );

      // the lenght of 2 is comprehensive of the letter 
      // the new line char, which must be appended in order for the firmware to
      // know when had received a complete message, is not included.
      if( temp.length() == 1 )
      {
        if (temp[0] == 'o' or temp[0] == 'O') {
          //Serial.println("bToPlotter = TRUE");
          bToPlotter = true;
        }
        else if (temp[0] == 'c' or temp[0] == 'C') {
          //Serial.println("bToPlotter = FALSE");
          bToPlotter = false;
        }
        // V4 can open or close the serial communication with Arduino
        else if( temp[0] == 'a' ) {
          bToPlotter = false;
          bToV4 = true;
        }
        else if( temp[0] == 'b' ) {
          bToV4 = false;
        }
        else if ( temp[0] == 'r')
        {
          // reset all the MPR
          Serial.println("RESET all");
          for(int i=0; i<NMPR; i++)
            mpr[i].cap.reset();
        }
        temp = "";
      }
    } 
    else
    {
      // if the message from VVVV is not finished yet,
      // append character to 'temp' string
      temp += c;
    }
  }
}


/************************************************************************************
 * PRINT ALL SENSORS
 ***********************************************************************************/
void printAllSensors()
{
  // cycle through all the mpr
  for(int i=0; i<NMPR; i++)
  {
    // cycle through all the electrodes
    for(int j=0; j<NPADS[i]; j++)
    {
      int state = (mpr[i].currtouched & _BV(j)) >> j;
      Serial.print( state );
    }
  }
  Serial.println(";");
}
