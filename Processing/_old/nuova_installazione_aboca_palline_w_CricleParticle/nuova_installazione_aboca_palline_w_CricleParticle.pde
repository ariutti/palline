Pallina palline[];

PFont f;
final int NPALLINE = 100;
final int pallinaRadius = 10;
boolean bEditMode = false;

void setup()
{
  size(800, 600);
  frameRate(60);
  palline = new Pallina[NPALLINE];
  //circlePacking();
  for(int i=0; i<palline.length; i++)
  {
    palline[i] = new Pallina(palline, i, pallinaRadius, random(width), random(height), width, height );
  }
  
  f = loadFont("BPGDejaVuSans2011GNU-GPL-Book-48.vlw");
  textAlign(CENTER, CENTER);
}

void draw() 
{
  background(0);
  for(int i=0; i<palline.length; i++) {
    palline[i].update();
  }
 
  for(int i=0; i<palline.length; i++) {
    palline[i].display();
  }
}

void keyPressed()
{
  if(key == 'e' || key == 'E')
  {
    // enter Edit Mode
    bEditMode = !bEditMode;
  }
  else
  {
    int chosen = int( random(NPALLINE) );
    //println(chosen);
    palline[ chosen ].touched();
  }
}


void mouseDragged()
{
  if(bEditMode)
  {
    for(int i=0; i<palline.length; i++) {
      if( palline[i].inside(mouseX, mouseY) )
      {
        palline[i].setPos(mouseX, mouseY);
        break;
      }
    }
  }
}


void mouseReleased()
{
  if( !bEditMode ) 
  {
    for(int i=0; i<palline.length; i++)
    {
      if( palline[i].inside(mouseX, mouseY) )
      {
        palline[i].touched();
        break;
      }
    }
  }
}
