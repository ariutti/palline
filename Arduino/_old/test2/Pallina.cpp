#include "Pallina.h"


void Pallina::init(int _nPalline, int _age)
{
  nPalline = _nPalline;
  age = _age;
}

void Pallina::printAge()
{
  Serial.println( age );
}

void Pallina::increaseAge()
{
  age ++;
}

void Pallina::setPallineRef(Pallina* _p)
{
  p = _p;
}

void Pallina::printMySistersList()
{
  // initialize a temporary pointer
  Pallina* tmp = p;
  for(int i=0; i<nPalline; tmp++, i++)
  {
    tmp->printAge();
  }
}
