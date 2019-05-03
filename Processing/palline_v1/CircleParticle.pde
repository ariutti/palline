final int IDLE = 0;
final int EXPANSION = 1;

class CircleParticle
{
  float expandingRadius;
  float expansionSpeed;
  float maxExpansion; // this is the maximum available expansion for the radius
  int touched[];
  int parentId; // this is the pallina ID the particle belongs to
  Pallina pallinaRef[]; // a reference to the palline array

  int state;
  
  CircleParticle(Pallina[] _pallinaRef, int _parentId, float _speed, float _maxExpansion)
  {
    pallinaRef = _pallinaRef;
    touched = new int[ pallinaRef.length ];
    resetTouched();
    expandingRadius = 0;
    parentId = _parentId;
    expansionSpeed = _speed;
    maxExpansion = _maxExpansion;
    state = IDLE;
  }
  
  void update()
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
        println("pallina " + parentId + ", state: IDLE");
        state = IDLE;
        expandingRadius = 0.0;
        resetTouched();
      }
      else
      {
        // if we are here it means the particle is expanding but in 
        // hasn't reached its maximum yet. So we must check if 
        // the "shockwave" is colliding with some pallina
        float emitterX = pallinaRef[ parentId ].pos.x;
        float emitterY = pallinaRef[ parentId ].pos.y;
        for(int j=0; j<palline.length; j++)
        {
          if( j != parentId ) 
          {
            float receiverX = pallinaRef[j].pos.x;
            float receiverY = pallinaRef[j].pos.y;
            
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
  
  void explode()
  {
    println("pallina " + parentId + " exploding!");
    //pallinaRef[ parentId ].printId();
    state = EXPANSION;
    //pallinaRef[ parentId ].ar.trigger();
  }
  
  float getRadius()
  {
    return expandingRadius;
  }
  
  int getStatus()
  {
    return state;
  }
  
  void resetTouched()
  {
    for(int i=0; i<touched.length; i++)
      touched[i] = 0;
  } 
}
