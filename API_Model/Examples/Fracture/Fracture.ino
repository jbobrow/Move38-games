// Simple Fracture

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
}

void buttonTriplePressed() {
  // change team
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
    setColor(colors[myState()];
  }
}
