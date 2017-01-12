/* Simple Fracture
 *  
 *  Rules:
 *  Tiles are only happy when next to at least 2 tiles 
 *  and none of their own color (state)
 *  
 */

// assuming neighbors and numNeighbors are global static variable
 
int numTeams = 3;
int colors[3][3] = {{255,0,0},        // Red                    
                    {0,0,255},        // Blue
                    {255,255,0}};     // Yellow

void setup() {
  // put your setup code here, to run once:
  setState(1);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void buttonTriplePressed() {
  // change team
  setState((myState() + 1) % numTeams + 1);
}

void neighborChanged() {
  boolean isHappy = true;
  for(int i=0; i<6; i++) {
    if(neighbors[i] == myState()){
      isHappy = false;
    }
  }

  if(numNeighbors >= 2 && isHappy) {
    blink(1000);
  }
  else {
    setColor(colors[myState()][0],
             colors[myState()][1],
             colors[myState()][2]);
  }
}
