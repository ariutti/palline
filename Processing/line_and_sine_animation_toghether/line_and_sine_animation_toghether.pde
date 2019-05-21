// a test sketch which is creating some circle each with two animators
// 1) a Line animator which is multiplying the amplitude of
// 2) a Sine animator
// You can press 'space' to reset thesline animation;

final int NCIRCLES = 5;
Circle[] c = new Circle[NCIRCLES];

void setup()
{
  size(300, 300);
  for(int i=0;i<NCIRCLES;i++) {
    c[i] = new Circle(random(width), random(height), 40, random(0.25, 2), random(2*PI));
  }
}

void draw()
{
  background( 120 );
  for(int i=0;i<NCIRCLES;i++) {
    c[i].update();
    c[i].draw();
  }
}

void keyPressed() {
  if(key == ' ') {
    for(int i=0;i<NCIRCLES;i++) {
      c[i].reset();
    }
  }
}
