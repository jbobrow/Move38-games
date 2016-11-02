/* base station for FlickFlop flicking game. 
 * Nick Bentley
 */

int flipinterval = 1000;        // interval between color flips in ms 
uint32_t lastUpdateTime = 0;        // the last time the color flipped
int flipstate = 0;             // reference state
uint8_t colors[2][3] = {{204,0,0},  // Red - player 1             
                        {0,0,255}}; // Blue - player 2
                        
void setup() {
  setColor(colors[0]);  //set initial color
  setState(0);          //set initial state
  setMicOff();          //turn the mic off
  setTimeout(600);      //10 minutes to timout
}
                

//todo: select number of players from base

//base flips periodically between the players' colors, and change output to match
//(transition should eventually be animated smoothly, but for now just flip)
void loop() {
  uint32_t curTime = getTimer();
  if(curTime - lastUpdateTime > flipinterval) {
    flipstate++; 
    setColor(colors[flipstate % 2]);  //flip the color of the tile
    setState((flipstate % 2)+1);  //flip the output
    lastUpdateTime = curTime; //mark the time of the update
  } 
}
 
