#include "Carlo.h"

Carlo::Carlo()
{
}

void Carlo::giveMeYourPets(Pet* _p)
{
  pets = _p;
}


void Carlo::talkAbountYourPets(int _nPets)
{
  Pet* p = pets;
  for(int i=0; i<_nPets; p++, i++)
  {
    p->printAge();
  }
}

void Carlo::modifyPets(int _nPets)
{
  Pet* p = pets;
  for(int i=0; i<_nPets; p++, i++)
  {
    p->increaseAge();
  }
}
