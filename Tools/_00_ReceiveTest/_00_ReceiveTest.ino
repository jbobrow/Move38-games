/*  Receive Test - Colored Sides
 *   
 *  Show white if alone
 *  Show a color based on the side a neighbor is sensed
 *  
 *        __0__
 *   5  /       \ 1
 *     /  [  *]  \
 *   4 \         / 2
 *      \_______/
 *          3
 *          
 *    0 = Red
 *    1 = Cyan
 *    2 = Magenta
 *    3 = Yellow
 *    4 = Blue
 *    5 = Orange
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

// colors arranged to travel the farthest in transition between colors on HSV wheel (high contrast from one to the next)
uint8_t colors[6][3] = {{204,0,0},        // Red
                        {0,204,204},      // Cyan
                        {204,0,204},      // Magenta
                        {204,204,0},      // Yellow
                        {0,0,255},        // Blue
                        {204,64,0}};      // Orange
                        
uint8_t white[3] = {255,255,255};

uint8_t neighbors[6];

boolean isAlone;

void setup() {
  setState(1);
  setMicOff();
  setTimeout(60);  // 1 minute
}

void loop() {

  getNeighborStates(neighbors);

  for(uint8_t i=0; i<6; i++) {
    if(neighbors[i] != 0) { // suggesting that anything read on this side other than 0 is evidence of a neighbor
      isAlone = true;
    }
  }
  
  if(isAlone) {
    setColor(white); 
  }
  else {
    // show color based on the side that a neighbor is connected to
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
}
