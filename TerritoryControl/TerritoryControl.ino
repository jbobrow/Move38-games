/*  Territory control w/ time(Game)
 *  
 *  Setup: 2 player game. Tiles die slowly over time. Quicker when surrounded by enemies, and slower when surrounded by family. Moving a tile boosts its life.
 *    
 *  When a tile is moved, it regains 20% health  
 *  When a tile hits an opponents tile, the opponents tile takes a 20% damage
 *    
 *  Technical Details:
 *    A long press on the tile changes the color of the tile for prototyping (switch state 1 or 2)
 *    
 *    Tiles resets health to full when triple press occurs
 *    
 *    ToDo: Show when tile gets boost
 *    ToDo: Verify alone (5 times testing positive as alone means it is alone...)
 *    
 *    ToDo: Game starts when single press in wait mode (enter player 1 & player 2 states)
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

uint8_t colors[3][3] = {{255,0,0},     // red   (player 1)
                       {0,0,255},      // blue  (player 2)
                       {255,255,255}}; // white (dead state)

uint8_t brightness[60] = {
  64,71,77,84,90,96,102,107,
  112,116,119,122,125,127,128,128,
  128,127,125,122,119,116,112,107,
  102,96,90,84,77,71,64,57,
  51,44,38,32,26,21,16,12,
  9,6,3,1,0,0,0,1,
  3,6,9,12,16,21,26,32,
  38,44,51,57};

uint8_t displayColor[3];

uint8_t neighbors[6];
uint8_t prevNeighbors[6];
uint8_t bAlone = 0;
uint8_t aloneCount = 0;

uint8_t team = 0; // which team are we part of blue or red (player 1 or player 2)

uint32_t lastPressTime = 0;
uint8_t bDidLongPress = 0;

uint32_t gameStartTime = 0;   // to know how far into the game we are
float health = 100.0;         // 100% health for start of life
float damageRate = 2.0;       // how much health is lost / second
float moveBoost = 20.0;       // 20% health boost when moved

// helpers for the pulse rate
uint32_t timePassedBuffer = 0;
uint8_t prevIdx = 0;

uint32_t lastPressTimes[2] = {0,0};
uint32_t lastUpdateTime = 0;
uint32_t updateFrequency = 20;  //milliseconds

void setup() {
  // put your setup code here, to run once:
  setButtonCallback(button);
  setLongButtonCallback(longPress, 2500); // 2.5 seconds to change color
  setState(1);  // test starting as player 1
  setMicOff();
  setTimeout(180);  // 3 minute
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t curTime = getTimer();

  // check surroundings
  getNeighborStates(neighbors);

  // if we are alive...
  if(getState() == 1 || getState() == 2) {
    // check how much life is remaining
    health -= damageRate * ((curTime - lastUpdateTime) / 1000.0);
    // same as health = health - ....;
    
    if(health <= 1.0) {
      // we are dead, show dead state
      health = 0.0;
      setState(3);
    }
    else {
      // deal with a living tile here

      uint8_t friends = 0;
      uint8_t enemies = 0;
      uint8_t numNeighbors = 0;

      // assume we are alone
      if(aloneCount < 254)
        aloneCount++;
      
      // count friends and enemies
      for(uint8_t i=0; i<6; i++) {
        
        // count total number of neighbors
        if(neighbors[i] != 0) {
          numNeighbors++;
          aloneCount = 0;
        }
        
        // check team red first
        if(team == 0) {                 // if on red team
          if(neighbors[i] == 1) {       // if red neighbor
            friends++;                  // count them as a friend
          }
          else if(neighbors[i] == 2) {  // if blue neighbor
            enemies++;                  // count them as an enemy
          }
        }
        // check team blue next
        else if(team == 1) {            // if on blue team
          if(neighbors[i] == 2) {       // if blue neighbor
            friends++;                  // count them as a friend
          }
          else if(neighbors[i] == 1) {  // if red neighbor
            enemies++;                  // count them as an enemy
          }
        }
      }

      // determine rate of life based on surroundings (sad but true)
      if(numNeighbors != 0) {
        // we are not alone
        // if we were, give a move boost
        if(bAlone == 1) {
          health += moveBoost;
          bAlone = 0;
        }
        
        if(friends > enemies) {
          // don't die quickly
          damageRate = 2.0;
        }
        else if (friends == enemies) {
          // normal rate
          damageRate = 3.0;
        }
        else { // implied friends < enemies
          // surrounded by enemies, this is gonna be rough
          damageRate = 4.0;
        }
      }
      else {
        // we are lonely or being moved. stay at the current damage rate

        // make sure we register as alone for 10 reads
        if(aloneCount > 10) {
          bAlone = 1;
        }
      }
    }
  }

  // determine period
  uint32_t period = health * 30.0; // sqrt(health)
  if(getState() == 3) {
    period = 4000;
  }
  uint32_t timePassed = curTime - lastUpdateTime;
  uint8_t idx = getNextPosition(60, prevIdx, timePassed, period);
  prevIdx = idx;
  
  // display your state based on the team you are on
  if(getState() == 3) {
    // display state using white (dead white)
    displayColor[0] = colors[2][0];
    displayColor[1] = colors[2][1];
    displayColor[2] = colors[2][2];
  }
  else if(team == 0) {
    // display state using red
    displayColor[0] = colors[0][0];
    displayColor[1] = colors[0][1];
    displayColor[2] = colors[0][2];
  } else if(team == 1){
    // display state using blue
    displayColor[0] = colors[1][0];
    displayColor[1] = colors[1][1];
    displayColor[2] = colors[1][2];
  } else {
    // how did we get here?
  }
  
  uint8_t r = (displayColor[0] * (32 + brightness[idx]))/255.0;
  uint8_t g = (displayColor[1] * (32 + brightness[idx]))/255.0;
  uint8_t b = (displayColor[2] * (32 + brightness[idx]))/255.0;
  
  displayColor[0] = r; 
  displayColor[1] = g; 
  displayColor[2] = b;
  
  setColor(displayColor); 

  lastUpdateTime = curTime;
}


/*
 * Button Press
 * starts a game if waiting to play
 */
void button() {
  if(getTimer() - lastPressTimes[0] < 1000 && getTimer() - lastPressTimes[1] < 2000) {
    // triple press
    // reset health
    health = 100.0;
    // set state based on team(color)
    switch(team) {
      case 0: setState(1); break;
      case 1: setState(2); break;
      default: setState(1); break;
    }
    lastPressTime = getTimer();
    lastUpdateTime = getTimer();
    bDidLongPress = 0;

  }
  else {
    // single press
    bDidLongPress = 0;
  }
  lastPressTimes[1] = lastPressTimes[0];
  lastPressTimes[0] = getTimer();
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

/*
 * Returns the next position given a previous position and the amount of time passed
 * The position is relative to the period provided
 * 
 * numSteps <uint8_t> number of steps in the cycle, values returned will fall within this range [0,numSteps]
 * currentPosition <uint8_t> value between 0 and numSteps for location in pattern
 * timePassed <uint32_t> millis since last updated position
 * period <uint16_T> length of a single cycle in millis 
 */
uint8_t getNextPosition(uint8_t numSteps, uint8_t currentPosition, uint32_t timePassed, uint32_t period) {
  
  //get normalized value of current position
  float pos = currentPosition / (float)numSteps;
  
  //calculate increment based on how much time passed over this period
  float increment = (timePassed + timePassedBuffer) / (float)period;
  
  // increment our position based on time passed
  float nextPos = pos + increment;

  // if we incremented past completion, start the cycle over
  if(nextPos >= 1.0) {
    nextPos -= 1.0;
  }

  // return the position in terms of steps along the total path
  uint8_t nextIndex = nextPos * numSteps;
  if(nextIndex == currentPosition) {
    timePassedBuffer += timePassed;
  }
  else {
    timePassedBuffer = 0;
  }
  
  return nextIndex;
}

