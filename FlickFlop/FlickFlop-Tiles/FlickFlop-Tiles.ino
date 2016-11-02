/* player tiles for FlickFlop flicking game
 *  Nick Bentley
 */

//check continuously to see if tile has become attached
//if tile has neighbors...
//match state to neighbor states
//if tile changed from zero to more than zero neighbors...
//permanently fix the color according to the global state

uint8_t neighbors[6];
int numNeighbors = 0; 
uint8_t colors[2][3] = {{204,0,0},  // Red - player 1             
                        {0,0,255}}; // Blue - player 2

uint8_t team = 0;
uint8_t numTeams = 2;
                        
void setup () {
  setColor(colors[1]);  //set initial color
  setState(0);          //set initial state
  setLongButtonCallback(button, 1000); // change initial color
}

void loop () {  
  
  //check to see if tile has gone from 0 to more neighbors
  getNeighborStates(neighbors);
  int prevNumNeighbors = numNeighbors;      // number of neighbors last time through the loop
  numNeighbors = 0;
  for (uint8_t i=0; i<6; i++) {   // loop over each side
    if (neighbors[i] != 0){       // if there's a neighbor on that side...
         numNeighbors++;            // add to neighbor count
         setState(neighbors[i]);  // match state to neighbor state  
         //there may be a problem with the logic above for spreading neighbor states   
    }
  }

  if ((numNeighbors !=0) && (prevNumNeighbors == 0)) { //if was alone and now has neighbors
    if (getState() == 1) {
      setColor(colors[0]); 
    }
    else if (getState() == 2) {
      setColor(colors[1]); 
    }
  }
}

void button() {
  // switch which team we are on
  team = (team + 1) % numTeams;
  
  if (team == 0) {
    setColor(colors[0]); 
  }
  else if (team == 1) {
    setColor(colors[1]); 
  }
}

