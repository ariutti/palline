
#include "Pallina.h"
#define NPALLINE 4
#define PALLINARADIUS 10

Pallina palline[NPALLINE];

float dist(int xA, int yA, int xB, int yB)
{
  int A = xB - xA;
  int B = yB - yA;
  return sqrt( A*A + B*B );
}


void setup()
{
  for(int i=0; i<NPALLINE; i++)
  {
    palline[i].init(NPALLINE, palline, i, PALLINARADIUS, random(100), random(100), 100, 100 );
  }

  
}

void loop()
{
  for(int i=0; i<NPALLINE; i++)
  {
    palline[i].update();
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
