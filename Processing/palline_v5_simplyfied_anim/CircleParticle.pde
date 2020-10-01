class CircleParticle
{
  float expansionSpeed;
  float expandingRadius;
  float maxExpansion; // this is the maximum available expansion for the radius
  
  int nPalline;
  int touched[];
  
  int parentId; // this is the pallina ID the particle belongs to
  int id;
  
  Ball pallinaRef[]; // a reference to the palline array

  // status machine
  final int IDLE = 0;
  final int EXPANSION = 1;
  int state;
  
  // CONSTRUCTOR ///////////////////////////////////////////////////////////////
  CircleParticle(int _nPalline, Ball[] _pallinaRef, int _parentId, int _cpId, float _expansionSpeed, float _maxExpansion)
  {
    nPalline = _nPalline;
    pallinaRef = _pallinaRef;
    
    parentId = _parentId;
    id = _cpId;
    
    touched = new int[ pallinaRef.length ];
    resetTouched();
    
    expansionSpeed = _expansionSpeed;
    maxExpansion = _maxExpansion;
    expandingRadius = 0;
    
    state = IDLE;
  }
  
  // UPDATE ////////////////////////////////////////////////////////////////////
  void update()
  {
    if( state == EXPANSION ) 
    {
      // if we are here it meas particle is expanding
      expandingRadius = expandingRadius + expansionSpeed;
      //println("expanding");
      
      if(expandingRadius >= maxExpansion) 
      {
        // if expansione has reached its maximum
        // expansion radius returns to zero and the particle 
        // go back to its IDLE status
        //println("pallina " + parentId + ", state: IDLE");
        state = IDLE;
        expandingRadius = 0.0;
        resetTouched();
        //print("reached my max expansion");
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
            //println(j);
            if( expandingRadius>=distance && readBall(j)==0 )
            {
              //println("reaced a ball with index " + j +  ";");
              // set 1 on the touched array
              setBall(j);
              pallinaRef[j].reached();
            }
          }
          else
            continue;
        } // end of for(int j=0; j<nPalline; j++)
      } // end of if(expandingRadius >= maxExpansion)
    } // end of if( state == EXPANSION )
    else
    {
      // particle is on IDLE state so do nothing
    }
  }
  
  
  // EXPLODE ///////////////////////////////////////////////////////////////////
  void explode()
  {
    //println("pallina " + parentId + " exploding!");
    //pallinaRef[ parentId ].printId();
    state = EXPANSION;
    //pallinaRef[ parentId ].ar.trigger();
  }
  
  // GET RADIUS ////////////////////////////////////////////////////////////////
  float getRadius()
  {
    return expandingRadius;
  }
  
  // GET STATUS ////////////////////////////////////////////////////////////////
  int getStatus()
  {
    return state;
  }
  
  // RESET TOUCHED /////////////////////////////////////////////////////////////
  void resetTouched()
  {
    for(int i=0; i<touched.length; i++)
      touched[i] = 0;
  } 

  // SET BALL //////////////////////////////////////////////////////////////////
  void setBall( int ballIndex) 
  {
    touched[ ballIndex ] = 1;
  }
  
  
  // CLEAR BALL ////////////////////////////////////////////////////////////////
  void clearBall( int ballIndex) 
  {
    touched[ ballIndex ] = 0;
  }

  // READ BALL /////////////////////////////////////////////////////////////////
  // CIRCLE PARTICLE READ BALL
  // Read the status of the examined ball from the bytes data structure "touched".
  // Given the "bit index" (expressed as a value between 0-NBALLS)
  // the function returns the bit status
  int readBall( int ballIndex )
  {
    return touched[ ballIndex ]  ;
  }

  // PRINT TOUCHED MATRIX //////////////////////////////////////////////////////
  void printTouchedMatrix() {}
  
}
