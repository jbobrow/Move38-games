/*
 *  Prototyping state propogation for games like Disco Simon
 * 
 *  by Jonathan Bobrow
 *  03.11.2017
 *  
 */

#define READY            1
#define CORRECT_RED      2
#define CORRECT_GREEN    3
#define INCORRECT_RED    4
#define INCORRECT_GREEN  4

uint8_t colors[4][3] = {
                       {0,0,0},          // Not used, just help our indexing for gameplay
                       {64,64,64},       // Ready state, show white
                       {255,0,0},        // Red
                       {0,255,0}};       // Green
                       
uint8_t currentColor = CORRECT_RED; // red or green

uint8_t white[3]  = {255,255,255};
uint8_t yellow[3] = {255,255,0};

uint8_t displayColor[3];

uint8_t neighbors[6];
uint8_t prevNeighbors[6];

uint32_t lastUpdatedStateTime = 0;
uint16_t updateStateBuffer = 500;

uint32_t lastUpdatedColorTime = 0;
uint16_t updateColorBuffer = 1000;

uint8_t buttonSequence[32] = {0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0,
                              0,0,0,0,0,0,0,0};             // this will be used to remember sequence of presses
uint8_t buttonSequenceLength = 32;       // length of sequence
uint8_t buttonIndex = 0;                 // tells us where in the remembered sequence of button presses to click

uint16_t nextPlayerDelay = 1500;
uint32_t timeOfPlayerSwitch = 0;

uint16_t incorrectDisplayDelay = 3000;
uint32_t timeOfIncorrect = 0;
                       
void setup() {
  // put your setup code here, to run once:
  setButtonCallback(button);  // respond to button press
  setState(READY);            // start in the ready position  
  setTimeout(60);             // go to sleep after 60 seconds of inactivity
}

void loop() {
  // put your main code here, to run repeatedly:
 
  if(getTimer() - lastUpdatedColorTime > updateColorBuffer) {             // flip color represented by tile every second
    currentColor = 3 - (currentColor % 2);  // 2 -> 3 and 3 -> 2
    lastUpdatedColorTime = getTimer();
  }
  
  // check surroundings
  getNeighborStates(neighbors);

  if( getState() == READY ) {
    if(getTimer() - timeOfPlayerSwitch < nextPlayerDelay) {
      setColor(yellow);                                               // turn yellow for player switch
    }
    else if(getTimer() - timeOfIncorrect < incorrectDisplayDelay) {
      setColor(white);                                             // turn white for incorrect
    }
    else {
      // show the color we represent
      displayColor[0] = colors[currentColor][0] * 0.33;
      displayColor[1] = colors[currentColor][1] * 0.33;
      displayColor[2] = colors[currentColor][2] * 0.33;
      
      setColor(displayColor);
    }
  
    // since we are in the ready state, look for changes around us
    for( uint8_t i = 0; i<6; i++) {
      if(neighbors[i] != 0) {
        // a neighor is present
        if(neighbors[i] != prevNeighbors[i]) {
          // a neighbor is different than before
          if(neighbors[i] != READY) {
            // something other than ready, let's propogate
            setState(neighbors[i]);
            lastUpdatedStateTime = getTimer();
            // and record what we just received
            handleReceivedState(neighbors[i]);
            // and break out of here...
            break;
          }
        }
      }
    }

  }
  else if ( getState() == CORRECT_RED ) {
    // if we are in the broadcast state, broadcast and wait
    setColor(colors[CORRECT_RED]);  // show we are propogating
  }
  else if ( getState() == CORRECT_GREEN ) {
    // if we are in the broadcast state, broadcast and wait
    setColor(colors[CORRECT_GREEN]);  // show we are propogating
  }
  else if ( getState() == INCORRECT_GREEN || getState() == INCORRECT_RED ) {
    setColor(white);  // show we are incorrect
  }
  else {
    // what other state can we be in?
  }

  // return to ready if it's been a while since we started propogating
  if(getTimer() - lastUpdatedStateTime > updateStateBuffer && getState() != READY) {
    setState(READY);    
  }

  // update prev neighbors
  for( uint8_t i = 0; i<6; i++) {
    prevNeighbors[i] = neighbors[i];
  }
}

void button() {
  if (buttonSequence[buttonIndex] == 0) {        //if we've come to the end of the sequence
    buttonSequence[buttonIndex] = currentColor;  //add the pressed color to the remembered sequence of presses
    setState(currentColor);                      //setState to notify other Blinks of result of buttonPress
    nextPlayer();
  }
   
  else {  //Else we are in the middle of a sequence
    
    if (currentColor != buttonSequence[buttonIndex]) { //If it’s an error
        didIncorrectMove(currentColor);
    }  
    else {
      buttonIndex++;           // Increment index (for checking color state against memory)
      setState(currentColor);  // setState to CORRECT_RED or CORRECT_GREEN
    }
  }

  lastUpdatedStateTime = getTimer();
}

void nextPlayer() {
  buttonIndex = 0; //set the buttonIndex back to the beginning of the remembered sequence
  //next player turn signal
  timeOfPlayerSwitch = getTimer();
}

void handleReceivedState(uint8_t state) {
  if( state == CORRECT_RED || state == CORRECT_GREEN ) {
    if(buttonSequence[buttonIndex] == 0) {    // if we've come to the end of the sequence
      buttonSequence[buttonIndex] = state;    // add the pressed color to the remembered sequence of presses
      nextPlayer();                           // show next players turn
    }
    else {
      buttonIndex++;
    }
  }
  else {
    // incorrect
    didIncorrectMove(state);
  }
}

void didIncorrectMove(uint8_t state) {
  if(state == CORRECT_RED) {
    setState(INCORRECT_RED);
  }
  else if(state == CORRECT_GREEN) {
    setState(INCORRECT_GREEN);
  }

  timeOfIncorrect = getTimer(); // time we failed miserably :) (go practice simon... fool)
  resetGame();                  // Reset the game*
}

void resetGame() { // Reset the game
  //clearing the button press memory
  for(int i = 0; i<buttonSequenceLength; i++) {
      buttonSequence[i] = 0;
  }
  buttonIndex = 0; //set the index for checking color state back to zero)
}

