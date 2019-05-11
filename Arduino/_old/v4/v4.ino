// PAY ATTENTION: This code is bugged
// for some reason we are unable to have more than one CP for Balls

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
// balls coords
uint8_t xs[] = { 0, 1, 2, 3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9 };
uint8_t ys[] = { 0, 1, 2, 3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9 };

//float xs[] = { 90.625, 58.25, 19.25, 11.25, 49.625, 91.5, 67.375, 36.625, 52.625, 76.5, 39.625, 19.75 };
//float ys[] = { 9.75, 18.125, 15.5, 40.875, 35.0, 49.125, 52.875, 65.125, 68.375, 85.0, 91.375, 75.75 };
#define WALL_W (100)
#define WALL_H (100)


/* CAPACITIVE STUFF ****************************************************************/

#include <Wire.h>
#include "Limulo_MPR121.h"

const uint8_t NMPR = 1;
const uint8_t NPADS[] = {1}; // NPALLINE
#define FIRST_MPR_ADDR 0x5A

struct mpr121
{
  Limulo_MPR121 cap;
  uint8_t addr;
  // Save an history of the pads that have been touched/released
  uint16_t lasttouched = 0;
  uint16_t currtouched = 0;
  uint16_t oor=0;
};

// an array of mpr121! You can have up to 4 on the same i2c bus
// mpr address can be one of these: 0x5A, 0x5B, 0x5C o 0x5D
mpr121 mpr[NMPR];

// LEDs /////////////////////////////////////////////////
#include "Adafruit_DotStar.h"
#include <SPI.h>

// Here's how to control the LEDs from any two pins:
#define DATAPIN    5
#define CLOCKPIN   4
// How many LEDs we have along the strip
const uint8_t NUMLEDS = 24;
// Leds Per Ball: how many LEDs ar dedicated to each ball
const uint8_t LPB = 2;
// in Adafruit DotStar library colors are ordered like this: GREEN, RED, BLUE
Adafruit_DotStar strip = Adafruit_DotStar(NUMLEDS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);

/* UTILITIES ***********************************************************************/
// have a pause between loops so it will be not overwelming
const int DELAY_TIME = 10;
// utility variables to dataviz values readed from MPR
boolean DEBUG = false;
boolean bDebug = true;
boolean bToPlotter = false;
boolean bToVVVV = true;
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
  Serial.println("\n\nSerial ready!");

  // LED stuff
  strip.begin(); // Initialize pins for output
  
  // BALL stuff
  for(uint8_t i=0; i<NPALLINE; i++)
  {
    Serial.print("Inizializzazione pallina ");
    Serial.print(i);
    Serial.println(";");
    palline[i].init(NPALLINE, palline, i, xs[i], ys[i], WALL_W, WALL_H, LPB, &strip);
  }

  // Turn all LEDs off ASAP
  strip.show(); 

  // CAPACITIVE STUFF **************************************************************/
  Serial.println("Looking for MPRs!");
  // cycle through all the MPR
  for(uint8_t i=0; i<NMPR; i++)
  {
    mpr[i].cap = Limulo_MPR121();
    mpr[i].addr = FIRST_MPR_ADDR + i;

    // Look for the MPR121 chip on the I2C bus
    if ( !mpr[i].cap.begin( mpr[i].addr ) )
    {
      if( bDebug ) Serial.println("MPR121 not found, check wiring?");
      while (1);
    }
    if( bDebug ) Serial.print("MPR ");
    if( bDebug ) Serial.print(i); 
    if( bDebug ) Serial.println(" found!");

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
    mpr[i].cap.setThresholds( 8, 3 );
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
        //Serial.print("Pad:"); Serial.print(j); Serial.println("touched!"); 
        palline[j].touched();
        
        if( bToVVVV ) printAllSensors(); // Send serial data to VVVV
      }
      if (!(mpr[i].currtouched & _BV(j)) && (mpr[i].lasttouched & _BV(j)) )
      {
        // pad 'i' has been released
        //Serial.print("Pad:"); Serial.print(j); Serial.println("released!");
        palline[j].released();
        
        if( bToVVVV ) printAllSensors(); // Send serial data to VVVV
      }
    }
    // reset our state
    mpr[i].lasttouched = mpr[i].currtouched;

    mpr[i].oor = mpr[i].cap.getOOR();

    
    if(bToPlotter)
    {
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
    }
    //mpr[i].cap.printOOR(); // added for debug purposes
  } // go through all the MPRs

  // PALLINE STUFF
  for(uint8_t i=0; i<NPALLINE; i++) {
    palline[i].update();
  }
  for(uint8_t i=0; i<NPALLINE; i++) {
    palline[i].display();
  }
  strip.show();

  delay(DELAY_TIME); // put a delay so it isn't overwhelming
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
      if( bDebug ) Serial.print( state );
    }
    if( bDebug ) Serial.println(";");
  }
}




/*
void keyPressed()
{
  if(key == 'e' || key == 'E')
  {
    // enter Edit Mode
    bEditMode = !bEditMode;
  }
  else
  {
    int chosen = int( random(NPALLINE) );
    //println(chosen);
    palline[ chosen ].touched();
  }
}

 */
