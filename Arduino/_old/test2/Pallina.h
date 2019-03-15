#ifndef __PALLINA__
#define __PALLINA__

#include "Arduino.h"

class Pallina {
  
private:
  int age;
  // a reference to the Palline array
  Pallina* p;
  int nPalline;

public:

  Pallina() {};
  void init(int nPalline, int age);
  void setPallineRef(Pallina* p);
  void printAge();
  void increaseAge();
  void printMySistersList();
};


#endif
