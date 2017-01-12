/* Simple Fracture
 *  
 *  Rules:
 *  Tiles are only happy when next to at least 2 tiles 
 *  and none of their own color (state)
 *  
 */
 
Color colors[8] = {RED, GREEN, ORANGE, BLUE, YELLOW, MAGENTA, CYAN};

int timeDelay = 1000;
int timeSinceChanged = 0;

void setup() {
  // put your setup code here, to run once:
  setState(1);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(isAlone()) {
    pulse(WHITE, 3000);
  }
  else{
    // fade to the current state color
    fadeTo(colors[myState()], 2000);
  }
}

void buttonPressed() {
  // change color
  setState((myState() + 1) % numTeams + 1);
}

void neighborChanged() {
  for(int i=0; i<6; i++) {
    if(neighbors[i] != myState()) {
      if (curTime - timeSinceChanged > timeDelay) {
        setState(neighbors[i]);
      }
    }
  }
}
