#ifndef __CARLO__
#define __CARLO__

#include "Arduino.h"
#include "Pet.h"

class Carlo {
  
private:

  Pet* pets;

public:

  Carlo();
  void giveMeYourPets( Pet* p );
  void talkAbountYourPets(int _n);
  void modifyPets(int _n);
};


#endif
