#include "Carlo.h"

Carlo::Carlo()
{
}

void Carlo::giveMeYourPets(Pallina* _p)
{
  pallineDiCarlo = _p;
}


void Carlo::talkAbountYourPets(int _nPets)
{
  Pallina* p = pallineDiCarlo;
  for(int i=0; i<_nPets; p++, i++)
  {
    p->printAge();
  }
}

void Carlo::modifyPets(int _nPets)
{
  Pallina* p = pallineDiCarlo;
  for(int i=0; i<_nPets; p++, i++)
  {
    p->increaseAge();
  }
}
