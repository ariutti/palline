#ifndef __CARLO__
#define __CARLO__

#include "Arduino.h"
#include "Pallina.h"

class Carlo {
  
private:

  Pallina* pallineDiCarlo;

public:

  Carlo();
  void giveMeYourPets( Pallina* p );
  void talkAbountYourPets(int _n);
  void modifyPets(int _n);
};


#endif
