#include "CircleParticle.h"

extern float dist(uint8_t xA, uint8_t yA, uint8_t xB, uint8_t yB);

void CircleParticle::init(uint8_t _nPalline, Ball* _pallinaRef, uint8_t _parentId, uint8_t _cpId, float _expansionSpeed, float _maxExpansion)
{
  nPalline = _nPalline;
  pallinaRef = _pallinaRef;
  
  //touched = new uint8_t[ 6 ];
  resetTouched();
  
  expansionSpeed = _expansionSpeed;
  expandingRadius = 0;
  parentId = _parentId;
  id = _cpId;
  maxExpansion = _maxExpansion;
  state = IDLE;
  if(DEBUG_CP) {
    Serial.print("cp: ");
    Serial.print( id );
    Serial.println(";");
  }
}

void CircleParticle::update()
{
  if( state == EXPANSION )
  {
    // if we are here it meas particle is expanding
    expandingRadius = expandingRadius + expansionSpeed;

    if(expandingRadius >= maxExpansion)
    {
      // if expansione has reached its maximum
      // expansion radius returns to zero and the particle
      // go back to its IDLE status
      //Serial.print("pallina ");
      //Serial.print( parentId );
      //Serial.println(", state: IDLE");
      state = IDLE;
      expandingRadius = 0.0;
      resetTouched();
      if(DEBUG_CP) { printTouchedMatrix(); };
    }
    else
    {
      // if we are here it means the particle is expanding but in
      // hasn't reached its maximum yet. So we must check if
      // the "shockwave" is colliding with some pallina
      float emitterX = pallinaRef[ parentId ].posX;
      float emitterY = pallinaRef[ parentId ].posY;
      for(int j=0; j<nPalline; j++)
      {
        if( j != parentId )
        {
          float receiverX = pallinaRef[j].posX;
          float receiverY = pallinaRef[j].posY;

          float distance = dist(emitterX, emitterY, receiverX, receiverY);

          if(DEBUG_CP) {
            Serial.print("CP ");
            Serial.print(parentId);
            Serial.print(" distance to ball #");
            Serial.print(j);
            Serial.print(": ");
            Serial.print(distance);
            Serial.println(";");
            printTouchedMatrix();            
          }

          // We reach a ball when shokwave is hitting the ball center.
          // We want to trigger light from the ball only if it wasn't previously
          // reached by the same particle shockwave
          //if( expandingRadius>=distance && touched[j]==0 )
          if( expandingRadius>=distance && !readBall(j) )
          {
            // set 1 on the touched array
            //touched[j] = 1;
            
            if( DEBUG_CP ) {
              Serial.print("CP ");
              Serial.print(parentId);
              Serial.print(" reached ball #");
              Serial.print(j);
              Serial.println(";");
            }
            setBall(j);
            pallinaRef[j].reached();
          }
        }
        else
          continue;
      }
    }
  }
  else
  {
    // particle is on IDLE state so do nothing
  }
  
  if( DEBUG_CP ) {
    Serial.print("\tCP ");
    Serial.print(parentId);
    Serial.print(" status: ");
    switch(state) {
      case 0:
        Serial.print("IDLE");
        break; 
      case 1:
        Serial.print("EXPANSION");
        break;
      default:
        // do nothing
        break;
    }
    Serial.print(", radius: ");
    Serial.print( expandingRadius);
    Serial.println(";");
  }
}

void CircleParticle::explode()
{
  if(DEBUG_CP) {
    Serial.print("CircleParticle ");
    Serial.print(parentId);
    Serial.println(" exploding!");
  }
  state = EXPANSION;
}

float CircleParticle::getRadius()
{
  return expandingRadius;
}

float CircleParticle::getStatus()
{
  return state;
}

//void CircleParticle::resetTouched()
//{
//  for(uint8_t i=0; i<nPalline; i++) {
//    uint8_t j = i / 8;
//    uint8_t k = i % 8;
//    touched[j] = touched[j] & _BV(k);
//  }  
//}


void CircleParticle::resetTouched()
{
  for(uint8_t i=0; i<6; i++)
    touched[i] = 0; // reset 8 balls at once!
}

// given the "bit index" (expressed as a value between 0-NBALLS)
// the function is capable to set that specific bit (set it to 1)
void CircleParticle::setBall( uint8_t ballIndex )
{
  //Serial.println("\n--- set pallina ---");
  int byteIndex = ballIndex / 8;
  int bitIndex  = ballIndex % 8;
  touched[ byteIndex ] = touched[ byteIndex ] | (1<<bitIndex);
}

// given the "bit index" (expressed as a value between 0-NBALLS)
// the function is capable to clear that specific bit (set it to 0)
void CircleParticle::clearBall( uint8_t ballIndex )
{
  //Serial.println("\n--- clear pallina ---");
  int byteIndex = ballIndex / 8;
  int bitIndex  = ballIndex % 8;
  bitClear(touched[ byteIndex ], bitIndex);
}

// given the "bit index" (expressed as a value between 0-NBALLS)
// the function returns the bit status
boolean CircleParticle::readBall( uint8_t ballIndex )
{
  //Serial.println("\n--- read pallina ---");
  int byteIndex = ballIndex / 8;
  int bitIndex  = ballIndex % 8;
  return bitRead(touched[ byteIndex ], bitIndex); 
}

void CircleParticle::printTouchedMatrix()
{
  Serial.println("\n--- trouched matrix ---");
  for(int i=0; i<6; i++) {
    Serial.println( touched[i], BIN );
  }

  /*
  for(int i=0; i<NBALLS; i++) {
    int byteIndex = i / 8;
    int bitIndex  = i % 8;
    if( bitRead(touched[ byteIndex ], bitIndex) == 1) {
      Serial.print( "la pallina " );
      Serial.print( i );
      Serial.println( " e' settata." );  
    }  
  }
  */
}
