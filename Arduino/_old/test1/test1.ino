#include "Pet.h"
#include "Carlo.h"

#define NOBJ 4

Pet pets[ NOBJ ] = {
Pet(3),
Pet(13),
Pet(9),
Pet(37)
};

Carlo* carlo = new Carlo();

void setup()
{
  Serial.begin(9600);
  
  //Serial.print("dimensione in byte di Pet: ");
  //Serial.println( sizeof(Pet) );

  carlo->giveMeYourPets( pets );
  carlo->talkAbountYourPets( 4 );
  carlo->modifyPets( 4 );
  carlo->talkAbountYourPets( 4 );

  for(int i=0; i<4; i++)
  {
    pets[i].printAge();
  }
}

void loop()
{
  // do nothing
}
