class Animator_Sine
{
  float freq;
  float phase;
  float t, t0, dt;
  float y;

  Animator_Sine ( float _freq, float _phase )
  {
    freq = _freq>0.0?_freq:1;
    phase = abs(_phase)%(2*PI);
    // At the beginning 't0' and 't' are equal.
    t0 = t = (millis() * 0.001);
    dt = t - t0;
  }

  void update()
  {
    t = (millis() * 0.001);
    dt = t - t0;
    y = sin( phase + 2*PI*freq*dt );
    y = (y+1)*0.5;
  }

  void changeFreq( float _freq )
  {
    // calculate the phase for the
    // upcoming sinusoid
    t = (millis() * 0.001);
    dt = t - t0;
    phase = phase + 2*PI*freq*dt;
    phase = phase % (2*PI);
    // define the new 't0' which is the
    // new reference for counting time
    t0 = t;
    // Finally, set the new frequency
    freq = _freq;
  }

  float getY() {
    return y;
  }
}
