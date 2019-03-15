#include "Pet.h"


Pet::Pet(int _age)
{
  age = _age;
}

void Pet::printAge()
{
  Serial.println( age );
}

void Pet::increaseAge()
{
  age ++;
}
