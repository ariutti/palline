Pallina palline[];

PFont f;

void setup()
{
  size(400, 400);
  frameRate(60);
  palline = new Pallina[100];
  for(int i=0; i<palline.length; i++)
  {
    palline[i] = new Pallina( i, random(width), random(height), width, height );
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
  
  for(int i=0; i<palline.length; i++)
  {
    float emitterX = palline[i].pos.x;
    float emitterY = palline[i].pos.y;
    float emitterRadius = palline[i].getExpandingRadius();
    for(int j=0; j<palline.length; j++)
    {
      if( i != j ) 
      {
        float receiverX = palline[j].pos.x;
        float receiverY = palline[j].pos.y;
        
        float distance = dist(emitterX, emitterY, receiverX, receiverY);
        
        if( palline[j].getStatus() == IDLE && emitterRadius>=distance)
          palline[j].reached( i );
      }
      else
        continue;
    }
  }
  
  
  for(int i=0; i<palline.length; i++) {
    palline[i].display();
  }
}

void mouseReleased()
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
