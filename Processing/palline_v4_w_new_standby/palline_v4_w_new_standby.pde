// Palline v3
// Each Ball has 5 circle parlicle to be used to create more than
// one shockwave!
// In addition to that each ball now have a sort of standby state where it will start 
// breathing!

Ball palline[];

PImage background;
PFont f;
final int NPALLINE = 12 +12 + 11 + 10;
boolean bEditMode = false;
boolean bShowBackgroundImg = false;


int xs[] = { 47, 40, 34, 29, 17, 11, 16, 28, 40, 28, 40, 35, 23, 10, 22, 29, 17, 10, 16, 28, 34, 47, 51, 41, 46, 58, 58, 70, 70, 64, 81, 88, 88, 82, 76, 64, 70, 58, 52, 64, 64, 81, 88, 76, 88 };
int ys[] = { 16, 9, 21, 10, 16, 27, 38, 33, 38, 44, 49, 55, 50, 55, 60, 68, 72, 78, 89, 89, 84, 90, 78, 72, 61, 55, 67, 66, 72, 83, 84, 89, 67, 61, 50, 44, 38, 33, 27, 21, 9, 15, 21, 27, 38 };





// 2020-09-30
// STANDBY STUFF *******************************************************************/
// internal status
final int STANDBY_MODE = 0;
final int ACTIVE_MODE = 1;
int mode;

long currentTime = 0;
// This represent the time we have to wait before passing
// to the STANDBY_MODE after the user inactivity
long TIME_TO_WAIT_BEFORE_STANDBYING = 10000; // ms
// When in standby, this represent the time between consecutive
// ball triggerings.
long TIME_BETWEEN_STANDBY_TRIGGERING_MIN = 25; // ms
long TIME_BETWEEN_STANDBY_TRIGGERING_MAX = 200; // ms
long TIME_BETWEEN_STANDBY_TRIGGERING; // ms

float ACTIVE_BRIGHTNESS = 1.0;
int ACTIVE_ATK_TIME = 250;
int ACTIVE_RLS_TIME  = 500;

float STANDBY_BRIGHTNESS = 0.4;
int STANDBY_ATK_TIME = 2000;
int STANDBY_RLS_TIME  = 2000;


// SETUP /////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  size(800, 800);
  frameRate(60);

  background = loadImage("wall.jpg");
  //println("image w: " + background.width + ", image h: " + background.height); //3152, 3069

  palline = new Ball[NPALLINE];
  //circlePacking();
  for (int i=0; i<palline.length; i++)
  {
    palline[i] = new Ball(NPALLINE, palline, i, (xs[i]/100.0) * width, (ys[i]/100.0) * height, width, height, STANDBY_BRIGHTNESS, STANDBY_ATK_TIME, STANDBY_RLS_TIME ); // pallinaRadius, //random(0.25, 2), random(2*PI)
  }

  f = loadFont("BPGDejaVuSans2011GNU-GPL-Book-48.vlw");
  textAlign(CENTER, CENTER);
  
    // 2020-09-30: state machine stuff
  currentTime = millis();
  mode = STANDBY_MODE;
  setBallsToStandbyMode();
}



// DRAW //////////////////////////////////////////////////////////////////////////////////////////////////////////
void draw() 
{
  /* 2020-09-30: state machine stuff ***********************************************/
  if( mode == STANDBY_MODE ) {
    if( (millis()-currentTime) >= TIME_BETWEEN_STANDBY_TRIGGERING ) {
      //println("TIME_BETWEEN_STANDBY_TRIGGERING is passed");
      pickRandomBallAndTrigger();
      currentTime = millis();
      TIME_BETWEEN_STANDBY_TRIGGERING = int( random(TIME_BETWEEN_STANDBY_TRIGGERING_MIN, TIME_BETWEEN_STANDBY_TRIGGERING_MAX ) );
    }
  } else if ( mode == ACTIVE_MODE ) {
    if( (millis()-currentTime) >= TIME_TO_WAIT_BEFORE_STANDBYING ) {
      //println(" TIME_TO_WAIT_BEFORE_STANDBYING is passed");
      currentTime = millis();
      mode = STANDBY_MODE;
      println("MAIN: status ---> STANDBY");
      setBallsToStandbyMode();
    }
  }
  
  //if( mode == STANDBY_MODE)
  //  println("STANBY_MODE");
  //else if( mode == ACTIVE_MODE)
  //  println("ACTIVE_MODE");
  
  
  
  background(60);
  
  
  
  

  if ( bShowBackgroundImg )
    image(background, 0, 0, width, height);

  for (int i=0; i<NPALLINE; i++) {
    palline[i].update();
  }

  for (int i=0; i<NPALLINE; i++) {
    palline[i].display();
  }

  pushStyle();
  pushMatrix();
  translate(width, 20);
  if ( bEditMode )
  {
    fill(255);
    textAlign(RIGHT);
    text("Edit Mode", 0, 0);
  }
  popMatrix();
  popStyle();
}

//void printCoords()
//{
//  // X coords
//  print("float xs[] = { ");
//  for(int i=0; i<palline.length; i++) {
//    print( (palline[i].getX()/width) * 100.0 );
//    if(i != palline.length-1)
//      print(", ");
//  }
//  println(" };");

//  // Y coords
//  print("float ys[] = { ");
//  for(int i=0; i<palline.length; i++) {
//    print( (palline[i].getY() / height) * 100.0);
//    if(i != palline.length-1)
//      print(", ");
//  }
//  println(" };");
//}

//void keyPressed()
//{
//  if(key == 'b' || key == 'B')
//  {
//    bShowBackgroundImg = !bShowBackgroundImg;
//    if(!bShowBackgroundImg)
//      printCoords();
//  }
//  else if(key == 'e' || key == 'E')
//  {
//    // enter Edit Mode
//    bEditMode = !bEditMode;
//  }
//  else if(key == ' ')
//  {
//    int chosen = int( random(NPALLINE) );
//    //println(chosen);
//    palline[ chosen ].reached();
//    palline[ chosen ].touched();
//  }
//}


//void mouseDragged()
//{
//  if(bEditMode)
//  {
//    for(int i=0; i<palline.length; i++) {
//      if( palline[i].inside(mouseX, mouseY) )
//      {
//        palline[i].setPos(mouseX, mouseY);
//        break;
//      }
//    }
//  }
//}


void mouseReleased()
{
  if( !bEditMode ) 
  {
    for(int i=0; i<NPALLINE; i++)
    {
      if( palline[i].inside(mouseX, mouseY) )
      {
        //println( "You clicked me!");
        //palline[i].reached();
        palline[i].reset();
        palline[i].touched();
        
        // 2020-09-30: change status to ACTIVE if already not
        currentTime = millis();
        mode = ACTIVE_MODE;
        println("MAIN: status ---> ACTIVE");
        setBallsToActiveMode();
        
        break;
      }
    }
  }
}


// 2020-09-30
/************************************************************************************
 * PICK A RANDOM BALL AND TRIGGER IT
 ***********************************************************************************/
void pickRandomBallAndTrigger() {
  // 1. calculate a random index of a ball
  int index = int( random( NPALLINE ) );
  // change the max brightness of the ball
  //palline[ index ].reached();
  // change the timing of the ball
  //palline[ index ].reached();
  // 2. trigger it
  palline[ index ].reached();
}

void setBallsToActiveMode() {
  for(int i=0; i<NPALLINE; i++) {
    //palline[i];
    palline[i].changeBrightnessAndTimes(ACTIVE_BRIGHTNESS, ACTIVE_ATK_TIME, ACTIVE_RLS_TIME);
  }
}

void setBallsToActiveModeExcept( int avoidBallIndex ) {
  for(int i=0; i<NPALLINE; i++) {
    if(i == avoidBallIndex)
      continue;
    palline[i].changeBrightnessAndTimes(ACTIVE_BRIGHTNESS, ACTIVE_ATK_TIME, ACTIVE_RLS_TIME);
  }
}

void setBallsToStandbyMode() {
  for(int i=0; i<NPALLINE; i++) {
    palline[i].changeBrightnessAndTimes(STANDBY_BRIGHTNESS, STANDBY_ATK_TIME, STANDBY_RLS_TIME);
  }
}
