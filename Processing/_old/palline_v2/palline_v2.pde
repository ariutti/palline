// Palline v2
// Each Ball has 5 circle parlicle to be used to create more than
// one shockwave!

Pallina palline[];

PImage background;
PFont f;
final int NPALLINE = 10;
final int pallinaRadius = 30;
boolean bEditMode = false;
boolean bShowBackgroundImg = false;

void setup()
{
  size(800, 800);
  frameRate(60);
  
  background = loadImage("wall.jpg");
  //println("image w: " + background.width + ", image h: " + background.height); //3152, 3069
  
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
  
  if( bShowBackgroundImg )
    image(background, 0, 0, width, height);
    
  for(int i=0; i<palline.length; i++) {
    palline[i].update();
  }
 
  for(int i=0; i<palline.length; i++) {
    palline[i].display();
  }
  
  pushStyle();
  pushMatrix();
  translate(width, 20);
  if( bEditMode )
  {
    fill(255);
    textAlign(RIGHT);
    text("Edit Mode", 0, 0);
  }
  popMatrix();
  popStyle();
    
}

void printCoords()
{
  // X coords
  print("float xs[] = { ");
  for(int i=0; i<palline.length; i++) {
    print( (palline[i].getX()/width) * 100.0 );
    if(i != palline.length-1)
      print(", ");
  }
  println(" };");
  
  // Y coords
  print("float ys[] = { ");
  for(int i=0; i<palline.length; i++) {
    print( (palline[i].getY() / height) * 100.0);
    if(i != palline.length-1)
      print(", ");
  }
  println(" };");
}

void keyPressed()
{
  if(key == 'b' || key == 'B')
  {
    bShowBackgroundImg = !bShowBackgroundImg;
    if(!bShowBackgroundImg)
      printCoords();
  }
  else if(key == 'e' || key == 'E')
  {
    // enter Edit Mode
    bEditMode = !bEditMode;
  }
  else if(key == ' ')
  {
    int chosen = int( random(NPALLINE) );
    //println(chosen);
    palline[ chosen ].reached();
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
        palline[i].reached();
        palline[i].touched();
        break;
      }
    }
  }
}
