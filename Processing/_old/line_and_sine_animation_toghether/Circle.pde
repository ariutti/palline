class Circle
{
  PVector pos;
  int diameter;
  color c = color(255, 150, 50);
  Animator_Sine sine;
  Animator_Line line;
  float y_sine, y_line;

  Circle(float _x, float _y, int _d, float _freq, float _iPhase )
  {
    pos = new PVector(_x, _y);
    diameter = _d;
    sine = new Animator_Sine( _freq, _iPhase );
    line = new Animator_Line(2000);
    y_sine = 0.0;
    y_line = 0.0;
  }

  void update()
  {    
    sine.update();    
    y_sine = sine.getY();
    line.update();
    y_line = line.getY();
  }

  void draw()
  {
    //c = color(255*y_line, 150*y_line, 50*y_line);
    pushStyle();
    fill( c );
    noStroke();
    ellipse(pos.x, pos.y,
            diameter*(1+y_sine*y_line),
            diameter*(1+y_sine*y_line)
            );
    popStyle();
  }

  void reset() {
    line.fromTo(0, 1, 2000);
  }
}
