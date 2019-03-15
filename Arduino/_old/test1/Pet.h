#ifndef __PETS__
#define __PETS__

#include "Arduino.h"

class Pet {
  
private:
  int age;
  int legs;

public:

  Pet(int age);
  void printAge();
  void increaseAge();
};


#endif
