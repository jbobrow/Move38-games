/*  Mortals(Game)
 *
 *  !!! Only works w/ AutomaTiles Legacy API !!!
 *
 *  Setup: 2 player game. Tiles die slowly over time, all at the same rate. Moving a single tile to a new place allows it to suck life from surrounding tiles, friendly or enemy.
 *
 *  When a tile is moved, it sucks 10 points of health from its neighbors
 *
 *  Technical Details:
 *    A long press on the tile changes the color of the tile for prototyping (switch state 1 or 2)
 *
 *    Tiles resets health to full when triple press occurs
 *
 *    States for game piece
 *    0 = no piece
 *    1 = player 1 alive (glow purple)
 *    2 = player 2 alive (glow green)
 *    3 = player 1 alive moving solo
 *    4 = player 2 alive moving solo
 *    5 = dead (pulse white)
 *
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
 *  03.06.2017
 */

uint8_t deadWhite[3] = {16,16,16};      // white  (dead state)

uint8_t team1Strong[3] = {255,0,64};    // red    (player 1 beginning of life)
uint8_t team1Weak[3]   = {153,0,255};   // purple (player 1 end of life)
uint8_t team2Strong[3] = {64,255,0};    // green  (player 2 beginning of life)
uint8_t team2Weak[3]   = {0,255,153};   // cyan   (player 2 end of life)
// future use
uint8_t team3Strong[3] = {255,255,0};   // yellow (player 3 beginning of life)
uint8_t team3Weak[3]   = {255,64,0};    // orange (player 3 end of life)

uint8_t brightness[60] = {
  128,128,128,127,125,122,
  119,116,112,107,102,96,
  90,84,77,71,64,57,
  51,44,38,32,26,21,
  16,12,9,6,3,1,
  0,0,0,1,3,6,
  9,12,16,21,26,32,
  38,44,51,57,64,71,
  77,84,90,96,102,107,
  112,116,119,122,125,127};

uint8_t displayColor[3];

uint8_t neighbors[6];
uint8_t prevNeighbors[6];
uint8_t bAlone = 0;
uint8_t aloneCount = 0;

uint8_t team = 0; // which team are we part of purple or green (player 1 or player 2)

uint32_t lastPressTime = 0;
uint8_t bDidLongPress = 0;

// show when receive boost by glowing brighter
uint32_t boostTime = 0;
uint16_t boostHoldoff = 1000;  // prevent boost on reset
uint16_t boostDuration = 300;

uint32_t gameStartTime = 0;   // to know how far into the game we are
float health = 120.0;         // 120pts health for start of life
float damageRate = 2.0;       // how much health is lost / second
float moveBoost = 10.0;       // 10pts health drain from neighbors when moved alone
uint8_t bLeachLife = 0;       // boolean for health drain
uint32_t leachTime = 0;
uint16_t leachBuffer = 600;

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
  setTimeout(600);  // 10 minute
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t curTime = getTimer();
  
  // check surroundings
  getNeighborStates(neighbors);
  
  bLeachLife = 0;  

  // if we are alive...
  if(getState() >= 1 && getState() <= 4) {
    // check how much life is remaining
    health -= damageRate * ((curTime - lastUpdateTime) / 1000.0);
    // same as health = health - ....;

    if(health <= 1.0) {
      // we are dead, show dead state
      health = 0.0;
      setState(5);
    }
    else {
      // deal with a living tile here
      uint8_t numNeighbors = 0;
      uint8_t numAliveNeighbors = 0;

      for(uint8_t i=0; i<6; i++) {
    
        // count total number of neighbors
        if(neighbors[i] != 0) {
          
          numNeighbors++;
          aloneCount = 0;

          // if a neighbor is leaching life
          if(neighbors[i] == 3 || neighbors[i] == 4) {
            bLeachLife = 1;
          }
          
          if(neighbors[i] != 5) {
            numAliveNeighbors++;
          }
        }
      }

      // assume we are alone
      if(aloneCount < 254)
        aloneCount++;

      // prevent actions if just restarted (prevents accidental boosts)
      if(getTimer() - gameStartTime > boostHoldoff) {

        if(numNeighbors != 0) {
          // we are not alone
          // if we were alone, suck health from each neighbor
          if(bAlone == 1) {
            health += moveBoost * numAliveNeighbors;
            bAlone = 0;
            // only show boost animation if attaching to at least one living tile to leach from
            if(numAliveNeighbors != 0) {
              boostTime = curTime;
            }
            //return to attached state
            if(team == 0) {
              setState(1);
            }
            else {
              setState(2);
            }
          }
          else  {
            // we were not alone, check if we are being leached
            if(bLeachLife && curTime - leachTime > leachBuffer) {
              health -= moveBoost;
              leachTime = curTime;
            }
          }
        }
        else {
          // we are lonely or being moved. 
  
          // make sure we register as alone for 10 reads
          if(aloneCount > 10) {
            bAlone = 1;
            // set state to moving
            if(team == 0) {
              setState(3);
            }
            else {
              setState(4);
            }
          }
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
  if(getState() == 5) {
    // display state using white (dead white)
    displayColor[0] = deadWhite[0];
    displayColor[1] = deadWhite[1];
    displayColor[2] = deadWhite[2];
  }
  else{
    // based on the amount of health fade towards a death color
    float progHealth;
    if(health > 100.0){
      progHealth = 100.0;
    }
    else {
      progHealth = health;
    }
    float progress = progHealth / 100.0;
    if(team == 0) {      
      displayColor[0] = team1Strong[0] * progress + team1Weak[0] * (1.0 - progress);
      displayColor[1] = team1Strong[1] * progress + team1Weak[1] * (1.0 - progress);
      displayColor[2] = team1Strong[2] * progress + team1Weak[2] * (1.0 - progress);
    }
    else if(team == 1) {      
      displayColor[0] = team2Strong[0] * progress + team2Weak[0] * (1.0 - progress);
      displayColor[1] = team2Strong[1] * progress + team2Weak[1] * (1.0 - progress);
      displayColor[2] = team2Strong[2] * progress + team2Weak[2] * (1.0 - progress);
    }
  }

  uint8_t r, g, b;
  if(curTime - boostTime < boostDuration) {
    // fade from white for boost
    float progress = (curTime - boostTime) / (float)boostDuration;
    r = displayColor[0] * progress + 255 * (1.0 - progress);
    g = displayColor[1] * progress + 255 * (1.0 - progress);
    b = displayColor[2] * progress + 255 * (1.0 - progress);
    // set the index of the pulse to the brightest point to start
    prevIdx = 0;
  }
  else if(getState() == 5){
    // if dead, barely pulse
    r = (displayColor[0] * (2 + brightness[idx]))/255.0;
    g = (displayColor[1] * (2 + brightness[idx]))/255.0;
    b = (displayColor[2] * (2 + brightness[idx]))/255.0;
  }
  else {
    // pulse based on health
    r = (displayColor[0] * (32 + brightness[idx]))/255.0;
    g = (displayColor[1] * (32 + brightness[idx]))/255.0;
    b = (displayColor[2] * (32 + brightness[idx]))/255.0;
  }

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
    // set start time
    gameStartTime = getTimer();
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
