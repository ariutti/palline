#include "CircleParticle.h"

extern float dist(int xA, int yA, int xB, int yB);

void CircleParticle::init(int _nPalline, Pallina* _pallinaRef, int _parentId, float _speed, float _maxExpansion)
{
  nPalline = _nPalline;
  pallinaRef = _pallinaRef;
  touched = new int[ nPalline ];
  resetTouched();
  expandingRadius = 0;
  parentId = _parentId;
  expansionSpeed = _speed;
  maxExpansion = _maxExpansion;
  state = IDLE;
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
      Serial.print("pallina ");
      Serial.print( parentId );
      Serial.println(", state: IDLE");
      state = IDLE;
      expandingRadius = 0.0;
      resetTouched();
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

          // we want to trigger light from the pallina only if it wasn't previously
          // reached by the same particle shockwave
          if( expandingRadius>=distance && touched[j]==0 )
          {
            // set 1 on the touched array
            touched[j] = 1;
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
}

void CircleParticle::explode()
{
  Serial.print("pallina ");
  Serial.print(parentId);
  Serial.println(" exploding!");
  //pallinaRef[ parentId ].printId();
  state = EXPANSION;
  //pallinaRef[ parentId ].ar.trigger();
}

float CircleParticle::getRadius()
{
  return expandingRadius;
}

float CircleParticle::getStatus()
{
  return state;
}

void CircleParticle::resetTouched()
{
  for(int i=0; i<nPalline; i++)
    touched[i] = 0;
}
