#include "Pallina.h"
#include "Carlo.h"

#define NPALLINE 4

Pallina palline[ NPALLINE ];
/*= {
Pallina(NPALLINE, 3),
Pallina(NPALLINE, 13),
Pallina(NPALLINE, 9),
Pallina(NPALLINE, 37)
};
*/

Carlo* carlo = new Carlo();

void setup()
{
  Serial.begin(9600);
  for(int i=0; i<NPALLINE; i++)
  {
    //palline[i] = new Pallina();
    palline[i].init( NPALLINE, i*3 );
  }
  
  
  //Serial.print("dimensione in byte di Pet: ");
  //Serial.println( sizeof(Pet) );

  Serial.println("SETUP: set reference for palline");
  for(int i=0; i<NPALLINE; i++)
  {
    palline[i].setPallineRef( palline );
  }

  for(int i=0; i<NPALLINE; i++)
  {
    Serial.print("I'm am the Pallina number ");
    Serial.print(i);
    Serial.println(";\n");
    palline[i].printMySistersList();
    if(i == 2)
      palline[i].increaseAge();
    Serial.println();
  }
  
  

  //carlo->giveMeYourPets( palline );
  //carlo->talkAbountYourPets( 4 );
  //carlo->modifyPets( 4 );
  //carlo->talkAbountYourPets( 4 );
  /*
  for(int i=0; i<NPALLINE; i++)
  {
    palline[i].printAge();
  }
  */
}

void loop()
{
  // do nothing
}
