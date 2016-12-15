void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  switch(gameState){
    case alive: handleAliveState(); break;
    case dead: handleDeadState(); break;
  }

}

void handleAliveState() {
  health--;
  pulse(2000 - 20*health);
}

void handleDeadState() {
  pulse(white, 4000);
}

