/*  Territory control w/ time(Game)
 *  
 *  Setup: 2 player game. Tiles die slowly over time. Quicker when surrounded by enemies, and slower when surrounded by family. Moving a tile boosts its life.
 *    
 *  Technical Details:
 *    A long press on the tile changes the color of the tile for prototyping (switch state 1 or 2)
 *    
 *    Tiles resets to wait mode when double press occurs (pulse white)
 *    
 *    Game starts when single press in wait mode (enter player 1 & player 2 states)
 *    Shares the start message with neighbors (hold on green for 3 pulses...)
 *    
 *    States for game piece
 *    0 = no piece (never in state 0)
 *    1 = player 1 alive (glow red)
 *    2 = player 2 alive (glow blue)
 *    3 = player 1/2 dead (pulse white)
 *    4 = wait mode (pulse white)
 *    5 = start mode (green starting lights pattern)
 * 
 *  Game devopment by: Nick Bently, Jonathan Bobrow
 *
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  -------------------------------------------------------------------------------------------------- 
 *     
 *  by Jonathan Bobrow
 *  10.07.2016
 */
enum gameStates {
  alive,
  dead,
  waiting,
  start
};

uint8_t colors[5][3] = {{255,0,0},      // red   (player 1)
                    {0,0,255},      // blue  (player 2)
                    {255,255,255},  // white (dead state)
                    {255,255,255},  // white (wait state)
                    {0,255,0}};     // green (start state)

int brightness[60] = {
  64,71,77,84,90,96,102,107,
  112,116,119,122,125,127,128,128,
  128,127,125,122,119,116,112,107,
  102,96,90,84,77,71,64,57,
  51,44,38,32,26,21,16,12,
  9,6,3,1,0,0,0,1,
  3,6,9,12,16,21,26,32,
  38,44,51,57};

int displayColor[3];

uint8_t neighbors[6];

int team = 0;

uint32_t lastPressTime = 0;
uint8_t bDidLongPress = 0;

int gameStartTime = 0;
int lifeSpan = 15000;        // 15 second lifespan
int health = lifeSpan;       // starting life
int moveBoost = 5000;        // 5 second boost when moved


void setup() {
  // put your setup code here, to run once:
  setButtonCallback(button);
  setLongButtonCallback(longPress, 1000);
  setState(4);  //waiting
  setColor(colors[3]);  //waiting
  setMicOff();
  setTimeout(300);  // 2 minute
}

void loop() {
  // put your main code here, to run repeatedly:

  // check surroundings
  getNeighborStates(neighbors);

  if(getState() == 1 || getState() == 2) {
    // check how much life is remaining
    health = lifeSpan - (getTimer() - gameStartTime);
  
    if(health <= 0) {
      // we are dead, show dead state
      setState(3);
    }
    else {
      // deal with a living tile here

      int friends = 0;
      int enemies = 0;
      int numNeighbors = 0;
      
      // count friends and enemies
      for(int i=0; i<6; i++) {
        
        // count total number of neighbors
        if(neighbors[i] != 0) {
          numNeighbors++;
        }
        
        // check team red first
        if(team == 0) {
          if(neighbors[i] == 1) {
            friends++;
          }
          else if(neighbors[i] == 2) {
            enemies++;
          }
        }
        // check team blue next
        else if(team == 1) {
          if(neighbors[i] == 2) {
            friends++;
          }
          else if(neighbors[i] == 1) {
            enemies++;
          }
        }
      }

      // determine rate of life based on surroundings (sad but true)
      if(friends > enemies) {
        // don't die quickly
      }
      else if (friends == enemies) {
        // normal rate
      }
      else { // implied friends < enemies
        // surrounded by enemies, this is gonna be rough
      }

      
    }
  }
  
  // display your state based on the team you are on
  if(team == 0) {
    // display state using red
    setColor(colors[0]);
  } else if(team == 1){
    // display state using blue
    setColor(colors[1]);
  } else {
    // how did we get here?
  }

}


/*
 * Button Press
 * starts a game if waiting to play
 */
void button() {
  if(getState() == 4) {
    // start the game
    setState(5);
    setColor(colors[3]);
  }
  lastPressTime = getTimer();
  bDidLongPress = 0;
}


/*
 * Button Long Press (>1 second)
 * changes which player we are playing as
 */
void longPress() {
  if(bDidLongPress == 0) {
    // ensure we handle long press only once
    bDidLongPress = 1;
    
    // if player 1, change to player 2
    team = (team + 1) % 2;  // two player game
    
    // set the current state to alive as that color
    setState(team + 1);
  }
}

