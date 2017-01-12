/* Simple Live Free or Die
 *  
 *  Rules:
 *  Tiles start w/ 100 health 
 *  Over time, tiles die (shown by pulsing faster as health decreases)
 *  Tiles die faster when surrounded by enemies
 *  Tiles die slower when surrounded by friends
 *  Tiles gain health when moved individually
 *  Tiles pulse slowly white when dead
 *  
 */

int numTeams = 6;
Color colors[6] = {RED, BLUE, YELLOW, MAGENTA, CYAN, GREEN};

float health = 100.0;         // 100% health for start of life
float damageRate = 2.0;       // how much health is lost / second
float moveBoost = 20.0;       // 20% health boost when moved

boolean tileWasAlone = true;

void setup() {
  // put your setup code here, to run once:
  setState(1);  // initialize to team 1
}

void loop() {
  // put your main code here, to run repeatedly:

  // evaluate state every 10th of a second...
  
  if(health > 0)
    alive();
  else
    dead();
}

void buttonTriplePressed() {
  // change team
  setState((myState() + 1) % numTeams + 1);
}

void neighborChanged() {
  if(tileWasAlone) {
    // if was alone and no longer alone, give boost
    tileWasAlone = false;
    health += 20;
  }
  else(isAlone()) {    
    // if alone, get ready for boost
    tileWasAlone = true;
  }
  // calculate damageRate
  int numEnemies = 0;
  int numFriends = 0;
  for(int i = 0; i<6; i++) {
    if(neighbors[i] != 0) {
      if(neighbors[i] == myState()) {
        numFriends++;
      }
      else {
        numEnemies++;
      }
    }
  }
  
  if( numFriends > numEnemies ) {
    damageRate = 2.0;
  }
  else if ( numFriends == numEnemies ) {
    damageRate = 4.0;
  }
  else {
    damageRate = 6.0; 
  }
}

void alive() {
  health -= damageRate;
  int period = 2000 - 20*health;
  pulse(period);
}

void dead() {
  pulse(white, 4000);
  setState(0);  // or some state that doesn't cause harm to others...
}

