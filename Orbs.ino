
/*
  Osmos
*/
#include <Arduboy2.h>

Arduboy2 alib;
Arduboy2Base base;
Arduboy2Core core;

// Game configurable parameters
#define MaxOrbs 20
#define InitialOrbs 6
#define MY_INIT_RADIUS 5
#define MAX_RADIUS 10
#define MinInitialRadius 2
#define MaxInitialRadius 5
#define DIM_BLINK 30 // Speed at whch Blue LED shows missiles firing
#define SplitJump 4  // How far away orb jumps when split in half
#define SLOWEST_SPEED 10
#define INVINCIBLE_DELAY 5  //How long my orb is invcincible after hit
#define WIN_LOSE_BLINK 10
#define COUNT_DOWN_COUNT 5  // Initial count down seconds

// ------------------------
#define FPS 60
#define MaxX 127
#define MaxY 63
#define COUNTDOWN_X 0      // Where to place a double size 5 x 7 digit during countdown mode
#define COUNTDOWN_Y 0
enum NESW{
  dN=0, dNE, dE, dSE, dS, dSW, dW, dNW
};
#define UP_button 1
#define DN_button 2
#define LFT_button 4
#define RGT_button 8
#define TRUE 1
#define FALSE 0
#define ON 1
#define OFF 0

int x, y, i, j;
int wins=0, losses=0;
unsigned char color;
char fps = FPS;
struct orb{
  bool active, fill;
  int x,y, orbRadius;
  char orbDirection;
  char curSpeedCounter, speedReset;
};
orb orbs[MaxOrbs+1]; // Array of screen orbs
orb *me;
orb missile; 
char orbD;
int orbX, orbY, orbR, winLoseX;
long nextFrameCount=0, totalSec, lastSec, lastMs;
bool shooterMode, blueBlink, invincible, countDown;
char winFlag, loseFlag;
char blueBlinkCounter;
char countDownCount;

void soundTest();
void newGme();
void renderOrbs();
bool orbsTouch(int x0, int y0, int r0, int x1, int y1, int r1);
void absorbOrb(int larger, int smaller);
char WinLose[]="W:00 L:00 O:00";
char WinLoseOrbCuntPtr = 12;
char WinLoseWinPtr = 2;
char WinLoseLossesPtr = 7;
char winRadius;
float cos45 = 0.7071;   // cos 45 dgrees. radius*cos 45 degree is offset to point on circle at 45 degrees

// ORBS Logo
// Created with PixelArt drawign tool to create logo.png then using
// http://fuopy.github.io/arduboy-image-converter/ Convert PNS to Arduboy2 bitmap format
//

// Each byte is one column 8-pixels high with the low order bit being the upper top picel of that column.
// Each group is the 128 columns. 8 groups make up the 64 bit height of the image.

const PROGMEM unsigned char logo[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
0xc0, 0xc0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0,
0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
0xc0, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xfe, 0xfe,
0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff,
0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0x7f,
0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x7e, 0xfe, 0xfe, 0xff, 0xff,
0xfc, 0xf8, 0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xfc, 0xfe, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0x03, 0x01, 0x01, 0x00, 0x00, 0xf8, 0xf8, 0xf8, 0xf8, 0x00, 0x00, 0x01, 0x01, 0x03, 0xff,
0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0xc1, 0xc1, 0xe3, 0xff, 0x00,
0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x41, 0x41, 0xe3, 0xff, 0xe3, 0xc1,
0xc1, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x08, 0x18, 0x78, 0x78, 0xfc, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 0xf0, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0f, 0x1f,
0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xf8, 0xf0, 0xf0, 0xc0, 0x80, 0x83, 0x83, 0x83, 0x83, 0x80, 0xc0, 0xf0, 0xf0, 0xf8, 0xff,
0xc0, 0x80, 0x80, 0x80, 0xc0, 0xfe, 0xf8, 0xf0, 0xf0, 0xc0, 0x80, 0x83, 0x83, 0xc7, 0xff, 0xc0,
0x80, 0x80, 0x80, 0x80, 0x82, 0x82, 0x82, 0x82, 0x80, 0xc0, 0xf0, 0xf0, 0xf8, 0xff, 0xc7, 0x83,
0x83, 0x83, 0x82, 0x82, 0x82, 0x82, 0x82, 0x80, 0xc0, 0xf0, 0xf0, 0xf8, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x0f, 0x07, 0x01, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x03, 0x07, 0x07, 0x0f, 0x0f, 0x0f, 0x0f,
0x1f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
0x7f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 0x07,
0x07, 0x03, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  alib.begin();
  Serial.begin(9600); // Only do this one time
  alib.setFrameRate(fps);   // Frames per second    
  alib.initRandomSeed();     // Start random generator
  alib.clear();
  totalSec = 0; 
  
  // Display logo to start - 3 sedonds
  alib.drawBitmap(0,0,logo,MaxX+1,MaxY+1,color=INVERT);
  alib.display();
  delay (3000);

  countDown = 1;    // Cound down before starting first game - time to see display
  newGame();
  
}

void newGame(){
  Serial.println("Start Osmos");
  for (i=0; i<MaxOrbs; i++) orbs[i].active = FALSE;
  for (i=0; i<InitialOrbs; i++){
    orbs[i].active = TRUE;
    orbs[i].orbRadius = orbR = random(MinInitialRadius,MaxInitialRadius);
    orbs[i].x = random(orbR,MaxX-orbs[i].orbRadius);
    orbs[i].y = random(orbR,MaxY-orbs[i].orbRadius);    
    orbs[i].orbDirection = dNE; // random(0,7);  // N,NE, E, SE, S, SW, W, NW
    orbs[i].speedReset = orbs[i].curSpeedCounter = random(1,SLOWEST_SPEED); // Higher number slower speed
    };
  me = &orbs[MaxOrbs];      // me orbs is just beyond the other orbs
  me->x=63;
  me->y=31;
  me->orbDirection=dN;
  me->orbRadius = MY_INIT_RADIUS;
  missile.active = FALSE;
  missile.orbRadius = 1;
  missile.orbDirection = dE;
  winLoseX = ((MaxX+1)/2)- 3*strlen(WinLose);   // x pixel of Win/Lose message - 6x8 WxH
  shooterMode = FALSE;
  blueBlink = OFF;
  invincible = TRUE;
  winFlag = 0;
  loseFlag = 0;
  lastSec = totalSec;
  lastMs = millis();
  if (countDown){
    countDownCount = COUNT_DOWN_COUNT;
  }

}

// the loop function runs over and over again forever
void loop() {
  if (!alib.nextFrame()){
    nextFrameCount++;
    return;  // Waiting for the next frame to start
  }
  alib.clear();   // clear display
  renderOrbs();   // Put orbs in display buffer
  alib.display(); // Display the buffer to the screen

  // Check if win or lose
  j = 0;
  for (i=0;i<MaxOrbs;i++){
    if (orbs[i].active) j++;    // how many active orbs
  }

  // If won the game - flash that we won then start a new game
  // Make the winning orb big then back to the winning size - a few times
  if (!j && !winFlag){
    // I won - no more active orbs
    if (++wins > 999) wins = 999;
    winFlag = WIN_LOSE_BLINK;
    winRadius = me->orbRadius;
    me->orbRadius = 2*MAX_RADIUS;
    // Prepare to start a new gamew
  }
  if (winFlag){
    if (--me->orbRadius < winRadius) {
      me->orbRadius = 2*MAX_RADIUS;
      if (!--winFlag) {
        // start new game
        newGame();      // restart game
      }
    }
  }

  // if lost the game - flash where you lost then start a new game
  // Show where you lost a few times with a ring around it
  if (me->orbRadius == 1 && !loseFlag){
    if (++losses > 999) losses = 999;
    loseFlag = WIN_LOSE_BLINK;            // Blink the wining orb a few times
    me->orbRadius = 2*MAX_RADIUS;
  }
  if (loseFlag){
    if (!--me->orbRadius) {
      me->orbRadius = 2*MAX_RADIUS;
      if (!--loseFlag) {
        // start new game
        newGame();      // restart game
      }
    }
  }

  //
  // Process once per second
  //
  if(millis()>=lastMs+1000){
    lastMs=millis();
    nextFrameCount=0;
    if (++totalSec - lastSec  > INVINCIBLE_DELAY) invincible = FALSE;    // My orb is invinceable for the 1st 5 seconds
    core.digitalWriteRGB(GREEN_LED, RGB_OFF);
    if (countDown) {
      if (!--countDownCount) countDown = FALSE;  // Counted downt to zero
    }
    Serial.print("-->");
    Serial.print((int)countDownCount);
    Serial.print(",");
    Serial.println((int)countDown);

  }

  // If game won or lost, just indicate that for a bit and dont move orbs 
  if (winFlag || loseFlag || countDown) return;  //Skip all other game processing
      

  // Blink blue LED for dim blue so not too distracting
  if (shooterMode){
    if (blueBlinkCounter) blueBlinkCounter--;
    else {
      if (blueBlink) {
        blueBlink = OFF;
        core.digitalWriteRGB(BLUE_LED, RGB_OFF);
        blueBlinkCounter = DIM_BLINK;
      }
      else {
        blueBlink = ON;
        core.digitalWriteRGB(BLUE_LED, RGB_ON);
        blueBlinkCounter = DIM_BLINK;
      }
    }
  }


  // Determine if two orbs have collided - do they touch
  for (i=0;i<MaxOrbs;i++){
    if (!orbs[i].active) continue;  // Only examine active orbs
    for (j=i+1;j<MaxOrbs;j++){
      if (!orbs[j].active) continue;  // Only examine active orbs
      if (orbsTouch(orbs[i].x,orbs[i].y,orbs[i].orbRadius,
                    orbs[j].x,orbs[j].y,orbs[j].orbRadius)){
         // Collision has occureed
         core.digitalWriteRGB(GREEN_LED, RGB_ON); 
         // One orb is abut to consume the other orb 
         // Make the larger one larger - eliminate the smaller one
         if(orbs[i].orbRadius > orbs[j].orbRadius){
           absorbOrb(i,j);      // i absords j
         }
         else absorbOrb(j,i);   // j abrobs i
      } 
    }
  }

  // Determine if my Orb collided with another orb
  // If so, if mine is the larger, absrod the smaller
  // If mine was smaller, reduce my size and make the other larger. 
  // My orb is invincible for the first few seconds during startup to give the
  // user time to move the orb around and also when it is shooting missiles.
  if (!invincible && !shooterMode){            // my ord is safe during the startup period
    for (i=0;i<MaxOrbs;i++){
      if (!orbs[i].active) continue;  // Only examine active orbs
      // Has my orb collided with any other orbs
      if (orbsTouch(orbs[i].x,orbs[i].y,orbs[i].orbRadius,
                    me->x,me->y,me->orbRadius)){
         // Collision has occureed
         core.digitalWriteRGB(GREEN_LED, RGB_ON); 
         // One orb is abut to consume the other orb 
         // Make the larger one larger - eliminate the smaller one
         if(orbs[i].orbRadius > me->orbRadius){
           // I have neen absorbed somewhat
           absorbOrb(i,MaxOrbs);     // my Orb partially absorded 
           invincible = TRUE;       // invinceable for a few seconds
           lastSec = totalSec;      // invincible period
         }
         else absorbOrb(MaxOrbs,i);  // my Orb absorbs smaller or same size orb
      }  
    }
  }

  // Determine if a missile hit an orb
  if (missile.active){
    for (i=0;i<MaxOrbs;i++){
      if (!orbs[i].active) continue; // Only examine active orbs
      if (orbsTouch(orbs[i].x,orbs[i].y,orbs[i].orbRadius,
                    missile.x,missile.y,missile.orbRadius)){ 
        // missile has hit an orb - split orb in two
        missile.active = FALSE;   // missile exploded
        if (orbs[i].orbRadius > 2) orbs[i].orbRadius/= 2;    // reduce radius in half
        if (orbs[i].orbRadius < 2) orbs[i].orbRadius = 2;
        if (orbs[i].speedReset > 1) orbs[i].speedReset--;    // speed up from missible hit
        // move first split peiece of orb in general direcion of missile and away from the impact point
        switch (missile.orbDirection) {
          case dNE:
          case dE:
          case dSE:  // ORb moves SE ++
            orbs[i].x += (SplitJump*orbs[i].orbRadius);
            orbs[i].y == (SplitJump*orbs[i].orbRadius);
            break;
         case dNW:
         case dW:
         case dSW:   // Orb moves SW +-
            orbs[i].x += (SplitJump*orbs[i].orbRadius);
            orbs[i].y -= (SplitJump*orbs[i].orbRadius);
            break;
         case dN:    // Orb moves NE -+
            orbs[i].x -= (SplitJump*orbs[i].orbRadius);
            orbs[i].y += (SplitJump*orbs[i].orbRadius);
            break;         
         case dS:    // Orb moves SW --
            orbs[i].x -= (SplitJump*orbs[i].orbRadius);
            orbs[i].y -= (SplitJump*orbs[i].orbRadius);
            break;       
        }
        if (orbs[i].x - orbs[i].orbRadius < 0) orbs[i].x = orbs[i].orbRadius;
        if (orbs[i].y - orbs[i].orbRadius < 0) orbs[i].y = orbs[i].orbRadius;  
        if (orbs[i].x + orbs[i].orbRadius  > MaxX) orbs[i].x = MaxX - orbs[i].orbRadius;  
        if (orbs[i].y + orbs[i].orbRadius  > MaxY) orbs[i].y = MaxY - orbs[i].orbRadius; 
            
        orbs[i].orbDirection = (random(0,3)+orbs[i].orbDirection)%8; // Change direction of the orb that was hit

        // create a new orb movng in the opposite direction
        
        for (j=0;j<MaxOrbs;j++){
          if (orbs[j].active) continue; // Need inactive orb slot to create one
          orbs[j].active = TRUE;
          orbs[j].orbDirection = (orbs[i].orbDirection + 4)%8;// opposite direction
          orbs[j].orbRadius = orbs[i].orbRadius;    // same radius as other
          orbs[j].x = orbs[i].x; // Create new location based on location
          orbs[j].y = orbs[i].y; // of the one that split awau 
          
          // 2nd piece of split at opposite location in the universe
          switch (missile.orbDirection) {
          case dNE:
          case dE:
          case dSE:  // ORb moves SE ++
            orbs[i].x -= (2*SplitJump*orbs[i].orbRadius);
            orbs[i].y -= (2*SplitJump*orbs[i].orbRadius);
            break;
         case dNW:
         case dW:
         case dSW:   // Orb moves SW +-
            orbs[i].x -= (2*SplitJump*orbs[i].orbRadius);
            orbs[i].y += (2*SplitJump*orbs[i].orbRadius);
            break;
         case dN:    // Orb moves NE -+
            orbs[i].x += (2*SplitJump*orbs[i].orbRadius);
            orbs[i].y -= (2*SplitJump*orbs[i].orbRadius);
            break;         
         case dS:    // Orb moves SW --
            orbs[i].x += (2*SplitJump*orbs[i].orbRadius);
            orbs[i].y += (2*SplitJump*orbs[i].orbRadius);
            break;       
          }
          // Ensure orb within universe
          if (orbs[i].x - orbs[i].orbRadius < 0) orbs[i].x = orbs[i].orbRadius;
          if (orbs[i].y - orbs[i].orbRadius < 0) orbs[i].y = orbs[i].orbRadius;  
          if (orbs[i].x + orbs[i].orbRadius  > MaxX) orbs[i].x = MaxX - orbs[i].orbRadius;  
          if (orbs[i].y + orbs[i].orbRadius  > MaxY) orbs[i].y = MaxY - orbs[i].orbRadius; 

          orbs[j].speedReset = orbs[j].curSpeedCounter = orbs[i].speedReset; // Same speed as other half
          break;
          }
        break;
      }
    }
  }

  if (alib.pressed(A_BUTTON)){
    shooterMode = FALSE;
    missile.active = FALSE;
    core.digitalWriteRGB(BLUE_LED, RGB_OFF);
    }

  if (alib.pressed(B_BUTTON)){
    if (!shooterMode){  // skip if already in shooterMode
      shooterMode = TRUE;
      blueBlink = OFF;
      blueBlinkCounter = 0;
      }
    } 
  
  // Move the missile
  if (shooterMode && !missile.active){    
    missile.active = TRUE;
    // Place the missile at the starting position along the outside edge of my orbs
    switch (missile.orbDirection) {
      case dN:
        missile.x = me->x;
        missile.y = me->y - me->orbRadius;
        break;       
      case dNE:
        missile.x = me->x + (int)(cos45*me->orbRadius);
        missile.y = me->y + (int)(cos45*me->orbRadius);    
      case dE:
        missile.x = me->x + me->orbRadius;
        missile.y = me->y;
        break;
      case dSE:
        missile.x = me->x + (int)(cos45*me->orbRadius);
        missile.y = me->y - (int)(cos45*me->orbRadius);     
        break; 
      case dS:
        missile.x = me->x;
        missile.y = me->y + me->orbRadius;    
        break;
      case dSW:
        missile.x = me->x - (int)(cos45*me->orbRadius);
        missile.y = me->y + (int)(cos45*me->orbRadius); 
        break;     
      case dW:
        missile.x = me->x - me->orbRadius;
        missile.y = me->y;   
        break;
      case dNW:
        missile.x = me->x - (int)(cos45*me->orbRadius);
        missile.y = me->y - (int)(cos45*me->orbRadius);      
        break;     
      }
    }
  else {
    // Move the missle in the correct direction
    switch (missile.orbDirection) {
      case dN:
        if (--missile.y + missile.orbRadius <= 0) missile.active = FALSE;
        break;       
      case dNE:
        if (++missile.x + missile.orbRadius >= MaxX) missile.active = FALSE;
        if (--missile.y + missile.orbRadius <= 0) missile.active = FALSE;
        break;          
      case dE:
        if (++missile.x + missile.orbRadius >= MaxX) missile.active = FALSE;
        break;
      case dSE:
        if (++missile.x + missile.orbRadius >= MaxX) missile.active = FALSE;
        if (++missile.y + missile.orbRadius >= MaxY) missile.active = FALSE;      
        break; 
      case dS:
        if (++missile.y + missile.orbRadius >= MaxY) missile.active = FALSE;          
        break;
      case dSW:
        if (--missile.x + missile.orbRadius <= 0) missile.active = FALSE;
        if (++missile.y + missile.orbRadius >= MaxY) missile.active = FALSE;  
        break;     
      case dW:
        if (--missile.x + missile.orbRadius <= 0) missile.active = FALSE;
        break;
      case dNW:
        if (--missile.x + missile.orbRadius <= 0) missile.active = FALSE;
        if (--missile.y + missile.orbRadius <= 0) missile.active = FALSE;  
        break;     
      }
    }
 

  // Move my orb around
  if (!shooterMode){
    if (alib.pressed(UP_BUTTON)){
      if (--me->y-me->orbRadius<0) me->y=me->orbRadius;
    }
    if (alib.pressed(DOWN_BUTTON)){
      if (++me->y+me->orbRadius > MaxY) me->y=MaxY-me->orbRadius;
    }
    if (alib.pressed(LEFT_BUTTON)){
      if (--me->x-me->orbRadius<0) me->x=me->orbRadius;
    }
    if (alib.pressed(RIGHT_BUTTON)){
      if (++me->x+me->orbRadius > MaxX) me->x=MaxX-me->orbRadius;
    }
  }
  else{
    // Shooter mode 
    i = 0;
    if (alib.pressed(UP_BUTTON))    i |= UP_button;
    if (alib.pressed(DOWN_BUTTON))  i |= DN_button;
    if (alib.pressed(LEFT_BUTTON))  i |= LFT_button;
    if (alib.pressed(RIGHT_BUTTON)) i |= RGT_button;
    
    // determine if missile direction is to change
    switch (i) {
      case 0:                                                 // 0
        break;    // No button pressed - no change
      case UP_button:                                         // 1
        missile.orbDirection = dN;
        break;
      case DN_button:                                         // 2
        missile.orbDirection = dS;
        break;
      case DN_button | UP_button:                             // 3
        missile.orbDirection = dS;
        break;
      case LFT_button:                                        // 4
        missile.orbDirection = dW;
        break;
      case LFT_button | UP_button:                            // 5
        missile.orbDirection = dNW;
        break;
      case LFT_button | DN_button:                            // 6
        missile.orbDirection = dSW;
        break;
      case LFT_button | DN_button | UP_button:                // 7
        missile.orbDirection = dSW;
        break;
      case RGT_button:                                        // 8
        missile.orbDirection = dE;
        break;
      case RGT_button | UP_button:                            // 9
        missile.orbDirection = dNE;
        break;
      case RGT_button | DN_button:                            // 10
        missile.orbDirection = dSE;
        break;
      case RGT_button | DN_button | UP_button:                // 11
        missile.orbDirection = dSE;
        break;
      case RGT_button | LFT_button:                           // 12
        missile.orbDirection = dE;
        break;
      case RGT_button | LFT_button | UP_button:               // 13
        missile.orbDirection = dNE;
        break;
      case RGT_button | LFT_button | DN_button:               // 14
        missile.orbDirection = dSE;
        break;
      case RGT_button | LFT_button | DN_button | UP_button:   // 15
        missile.orbDirection = dSE;
        break;
    }
    // If any button was pressed, stop the current missile and start a new one in the proper direction
    if (i) {
      Serial.print(i);
      missile.active = FALSE;   // start a new missile in a new direction   
    }
  }

  // Bounce off side walls
  for(i=0; i<MaxOrbs;i++){
    if (orbs[i].active && !--orbs[i].curSpeedCounter ){
      orbs[i].curSpeedCounter = orbs[i].speedReset;
      orbD=orbs[i].orbDirection;
      orbX=orbs[i].x;
      orbY=orbs[i].y;
      orbR=orbs[i].orbRadius;
      switch (orbD) {
        case dN:
          orbY--;   
          if((orbY-orbR) < 0){
            orbY=orbR;
            orbD=dS;
            if ((random(0,9)%2)) orbD=(orbD+random(1,7))%8;  // random new direction 50% of the time 
          }
          break;
        case dNE:
          orbY--;
          if(orbY-orbR<0){
            orbY=1+orbR;
            orbD=dSE;
          }
          orbX++;
          if(orbX+orbR>MaxX){
            orbX=MaxX-1-orbR;
            orbD=dNW;
          }
          break;
        case dE:
          orbX++;
          if((orbX+orbR) > MaxX){
            orbX = MaxX-orbR-1;
            orbD=dW;
            if ((random(0,9)%2)) orbD=(orbD+random(1,7))%8;  // random new direction 50% of the time 
          }
          break;
        case dSE:
          orbY++;
          if(orbY+orbR>MaxY){
            orbY=MaxY-1-orbR;
            orbD=dNE;
          }
          orbX++;
          if(orbX+orbR>MaxX){
            orbX=MaxX-1-orbR;
            orbD=dSW;            
          }          
          break;
        case dS:
          orbY++;
          if(orbY+orbR > MaxY){
            orbY=MaxY-orbR-1;
            orbD=dN;
            if ((random(0,9)%2)) orbD=(orbD+random(1,7))%8;  // random new direction 50% of the time 
          }
          break;
        case dSW:
          orbY++;
          if(orbY+orbR>MaxY){
            orbY=MaxY-1-orbR;
            orbD=dNW;
          }
          orbX--;
          if(orbX-orbR <0){
            orbX=1+orbR;
            orbD=dSE;
          }
          break;
        case dW:
          orbX--;
          if((orbX-orbR)<0){
            orbX=1+orbR;
            orbD=dE;
            if ((random(0,9)%2)) orbD=(orbD+random(1,7))%8;  // random new direction 50% of the time 
          }
          break;
        case dNW:
          orbY--;
          if(orbY-orbR<0){
            orbY=1+orbR;
            orbD=dSW;
          }
          orbX--;
          if(orbX-orbR <0){
            orbX=1+orbR;
            orbD=dNE;            
          }          
          break;
      }
      orbs[i].orbDirection = orbD;
      orbs[i].x=orbX;
      orbs[i].y=orbY;
    }
  }
}

void renderOrbs(){
  // Display Wins and Losses first - orbs overwrite them
  char buf[4];
  
  j = 0;
  for (i=0; i<MaxOrbs; i++) if (orbs[i].active ) j++;
  sprintf(buf,"%02d",j); // Number of orbs remaining 
  WinLose[WinLoseOrbCuntPtr]=buf[0];
  WinLose[WinLoseOrbCuntPtr+1]=buf[1];

  if (wins < 100) sprintf(buf,"%02d",wins);
  else sprintf(buf,"%3d",wins);
  WinLose[WinLoseWinPtr] = buf[0];
  WinLose[WinLoseWinPtr+1] = buf[1];
  if (wins >= 100) WinLose[WinLoseWinPtr+2] = buf[1];

  if (losses < 100) sprintf(buf,"%02d",losses);  
  else sprintf(buf,"%3d",losses);
  WinLose[WinLoseLossesPtr] = buf[0];
  WinLose[WinLoseLossesPtr+1] = buf[1];
  if (losses >= 100) WinLose[WinLoseLossesPtr+2] = buf[2];
 
  alib.setCursor(winLoseX,MaxY-7);  
  alib.write(WinLose);
  for (i=0; i<MaxOrbs;i++){
    if (orbs[i].active){
      if (me->orbRadius<orbs[i].orbRadius){
        base.fillCircle(orbs[i].x, orbs[i].y, orbs[i].orbRadius, WHITE);  
      }
     else base.drawCircle(orbs[i].x, orbs[i].y, orbs[i].orbRadius, WHITE);
    }
  } 
  if (invincible || shooterMode){
    base.drawCircle(me->x, me->y, me->orbRadius+1, WHITE);
  }
  base.drawCircle(me->x, me->y, me->orbRadius, WHITE);
  for (i=0;i<5;i++){
    // Make the inside of my Orb sparkle
    x=random(1,me->orbRadius-1);
    y=random(1,me->orbRadius-1);
    if (random(0,99)%2) x=-x;         // select +/- delta x
    if (random(0,99)%2) y=-y;
    alib.drawPixel(me->x+x,me->y+y,WHITE);  
  }
  // Draw missile
  if (missile.active) base.drawCircle(missile.x, missile.y, missile.orbRadius, WHITE); 

  // Display count down count at start of first game
  if (countDown){
    alib.drawChar(COUNTDOWN_X, COUNTDOWN_Y, countDownCount+'0', WHITE, BLACK, 2);
  }
}

bool orbsTouch(int x0, int y0, int r0, int x1, int y1, int r1){
  // d1 =sqrt(delta x^2 + delta y^)
  // d2 = r1-r0
  // d2^2 <= d1^2 then the circles touch
  
  if ( ( (x1-x0)*(x1-x0)) + ( (y1-y0)*(y1-y0) ) <= ( (r1+r0)*(r1+r0) ) ){
    char buf[120];
    return TRUE;
  }
  else {
    return FALSE;
  }
}

void absorbOrb(int larger, int smaller){
   if (orbs[larger].orbRadius < MAX_RADIUS) orbs[larger].orbRadius+=(orbs[smaller].orbRadius/2);
   if (orbs[larger].orbRadius > MAX_RADIUS) orbs[larger].orbRadius = MAX_RADIUS;
   if (orbs[larger].x - orbs[larger].orbRadius < 0) orbs[larger].x = 2*orbs[larger].orbRadius;
   if (orbs[larger].y - orbs[larger].orbRadius < 0) orbs[larger].x = 2*orbs[larger].orbRadius;  
   if (orbs[larger].x + orbs[larger].orbRadius  > MaxX) orbs[larger].x -= 2*orbs[larger].orbRadius;  
   if (orbs[larger].y + orbs[larger].orbRadius  > MaxY) orbs[larger].x -= orbs[larger].orbRadius;      
   if (++orbs[larger].speedReset > SLOWEST_SPEED) orbs[larger].speedReset = SLOWEST_SPEED; // slow down by 1 step
   if (smaller == MaxOrbs){   // my orb has been partially aborbed
     me->orbRadius--;
     if (!me->orbRadius) me->orbRadius = MY_INIT_RADIUS;  // Go back to start when 0 radius
   } else orbs[smaller].active=FALSE;               
}
