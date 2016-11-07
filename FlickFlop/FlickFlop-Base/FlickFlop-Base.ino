/* base station for FlickFlop flicking game. 
 * Nick Bentley
 */

int flipinterval = 1000;        // interval between color flips in ms 
uint32_t lastUpdateTime = 0;        // the last time the color flipped
int flipstate = 0;             // reference state
uint8_t numTeams = 3;
uint8_t bLongPressed = 0;
uint8_t colors[3][3] = {{0,255,255},  // Cyan      
                        {255,192,32},  // Orange
                        {255,0,255}}; // Magenta
                        
void setup() {
  setColor(colors[0]);  //set initial color
  setState(0);          //set initial state
  setMicOff();          //turn the mic off
  setTimeout(600);      //10 minutes to timout
  setLongButtonCallback(longButton, 2000); // change initial color
  setButtonCallback(button); // change initial color
}
                

//todo: select number of players from base

//base flips periodically between the players' colors, and change output to match
//(transition should eventually be animated smoothly, but for now just flip)
void loop() {
  uint32_t curTime = getTimer();
  if(curTime - lastUpdateTime > flipinterval) {
    flipstate++; 
    setColor(colors[flipstate % numTeams]);  //flip the color of the tile
    setState((flipstate % numTeams)+1);  //flip the output
    lastUpdateTime = curTime; //mark the time of the update
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
    
    // switch the number of teams in play
    
    if (numTeams == 2) {
      numTeams = 3; 
    }

    else if (numTeams == 3) {
      numTeams = 2; 
    }
  }
}
