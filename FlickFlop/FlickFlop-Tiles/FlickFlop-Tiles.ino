/* player tiles for FlickFlop flicking game
 *  
 *  N player game where each tile is flicked towards a base, when a tile attaches, 
 *  it permanently becomes the color of the base tile. Tile attachment should be celebrated
 *  and proportional to the number of neighbors attached to.
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
 *  by Nick Bentley
 *  11.02.2016
 */

uint8_t neighbors[6];
int numNeighbors = 0; 
uint8_t colors[3][3] = {{0,255,255},  // Cyan      
                        {255,64,0},  // orange
                        {255,0,255}}; // Magenta

uint8_t white[3] = {255, 255, 255};   // White
int bCelebrate = 0; 
uint32_t celebrateTime = 0; 
uint8_t team = 0;
uint8_t numTeams = 3;
uint16_t neighborDelay = 200;
int celebrateDuration = 300;  //time of celebratory burst
uint32_t lastUpdateTime = 0;
uint8_t bLongPressed = 0;
int startColor = 0; 
                        
void setup () {
  setColor(colors[1]);  //set initial color
  setState(0);          //set initial state
  setLongButtonCallback(longButton, 1000); // change initial color
  setButtonCallback(button); // change initial color
}

void loop () {  
  /*
   *  If we gained our first neighbor or neighbors. Set color based on state.
   *  (Use state of majority of neighbors in case of inconsistency)
   *  
   *  Once we are part of the pack, we need to keep our state, the same as our neighbors
   *  If neighbor does not equal my state, change to neighbor state, then wait 200 millis (buffer) before changing again
   */
  
  //check to see if tile has gone from 0 to more neighbors
  getNeighborStates(neighbors);
  
  // number of neighbors last time through the loop
  int prevNumNeighbors = numNeighbors;      

  // reset num neighbors for counting
  numNeighbors = 0;
  uint8_t consensusNeighborState = 0;

  // count the number of neighbors
  for (uint8_t i=0; i<6; i++) {   // loop over each side
    if (neighbors[i] != 0){       // if there's a neighbor on that side...
      numNeighbors++;             // add to neighbor count
      
      // if the neighbor isn't the same as me
      // and I haven't recently changed state (i.e. < 200ms)
      // I should update to broadcast the base's message
      if(neighbors[i] != getState() && getTimer() - lastUpdateTime > neighborDelay){
        setState(neighbors[i]);
        lastUpdateTime = getTimer();
        break;
      }
    }
  }
  
  if ((numNeighbors !=0) && (prevNumNeighbors == 0)) { //if was alone and now has neighbors 

    // do we celebrate
    if(getState() == (team + 1)) {
      bCelebrate = 1;
      celebrateTime = getTimer();  
    }
    else {
      bCelebrate = 0;
    }
    
    //show new team color
    
    team = getState()-1; 
    setColor(colors[team]);
    
  }
  else{}

  if(bCelebrate == 1) {
    if(getTimer() - celebrateTime < celebrateDuration) {    
      setColor(white);
    }
    else {
      bCelebrate = 0;
      // set back to actual color
      setColor(colors[team]);
    }
  }
  
}

/*
 * Button down
 */
void button() {
  bLongPressed = 0;
}
/*
 * Long Button Press
 * switches which initial team we are on
 */
void longButton() {
  if(bLongPressed == 0) {
    // set long press to true
    bLongPressed = 1;
    
    // switch which team we are on
    team = (team + 1) % numTeams;
    
    setColor(colors[team]);  

  }
}

