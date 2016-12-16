/* Simple Fracture
 *  
 *  Rules:
 *  Tiles are only happy when next to at least 2 tiles 
 *  and none of their own color (state)
 *  
 */
 
int numTeams = 6;
Color colors[6] = {RED, BLUE, YELLOW, MAGENTA, CYAN, GREEN};

void setup() {
  // put your setup code here, to run once:
  setState(1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void buttonTriplePressed() {
  // change team
  setState((myState() + 1) % numTeams);
}

void neighborChanged() {
  boolean isHappy = true;
  for(int i=0; i<6; i++) {
    if(getNeighbor(i) == myState()){
      isHappy = false;
    }
  }

  if(getNumNeighbors() >= 2 && isHappy) {
    blink(1000);
  }
  else {
    setColor(colors[myState()]);
  }
}
