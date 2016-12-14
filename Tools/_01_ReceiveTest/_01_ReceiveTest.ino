/*  Color Sides
 *  
 *  Show a color based on the side a neighbor is sensed
 *  
 *  Pulse white if alone
 *
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  -------------------------------------------------------------------------------------------------- 
 *     
 *  by Jonathan Bobrow
 *  12.2016
 */

// colors arranged to travel the farthest in transition between colors
uint8_t colors[6][3] = {{204,0,0},        // Red
                        {0,204,204},      // Cyan
                        {204,0,204},      // Magenta
                        {204,204,0},      // Yellow
                        {0,0,255},        // Blue
                        {204,64,0}};      // Orange
                        
                        
uint8_t brightness[30] = {
  71,84,96,107,
  116,122,127,128,
  127,122,116,107,
  96,84,71,57,
  44,32,21,12,
  6,1,0,1,
  6,12,21,32,
  44,57};

uint8_t displayColor[3];

uint8_t neighbors[6];
uint8_t numNeighbors = 0;

uint32_t lastUpdateTime = 0;
uint16_t updateFrequency = 50;  //milliseconds

uint8_t aloneCount = 0;

void setup() {
  setState(1);
  setMicOff();
  setTimeout(60);  // 1 minute
}

void loop() {
  uint32_t curTime = getTimer();
  if(curTime - lastUpdateTime > updateFrequency) {
    // Todo: animate the colors from one state to the next through a smooth transition
    getNeighborStates(neighbors);
    uint8_t didChangeState = 0;
    if(aloneCount < 254)
      aloneCount++;
    uint8_t numNeighbors = 0;
    for(uint8_t i=0; i<6; i++) {
      if(neighbors[i] != 0) {
        aloneCount = 0;
        numNeighbors++;
      }
    }
    
    if(aloneCount > 10) {// isAlone) {
      //pulse white
      int8_t idx = (curTime%3000)/100;
      displayColor[0] = 32 + brightness[idx]; 
      displayColor[1] = 32 + brightness[idx]; 
      displayColor[2] = 32 + brightness[idx];
      setColor(displayColor); 
    }
    else {
      // show color based on the sides that neighbors are connected to
      if(neighbors[0] != 0){
        setColor(colors[0]);
      }
      else if(neighbors[1] != 0) {
        setColor(colors[1]);
      }
      else if(neighbors[2] != 0) {
        setColor(colors[2]);
      }
      else if(neighbors[3] != 0) {
        setColor(colors[3]);
      }
      else if(neighbors[4] != 0) {
        setColor(colors[4]);
      }
      else if(neighbors[5] != 0) {
        setColor(colors[5]);
      }
    }
    
    // update our update time
    lastUpdateTime = curTime;
  }
}
