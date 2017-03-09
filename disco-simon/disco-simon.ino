/*
 *  Competitive Disco Simon
 *   
 *  by Nick Bentley
 *  03.09.2017
 */

#define READY          1
#define CORRECT_RED    2
#define CORRECT_GREEN  3
#define INCORRECT      4

uint8_t colors[4][3] = {
                       {0,0,0},          // Not used, just help our indexing for gameplay
                       {0,0,0},          // Not used, just help our indexing for gameplay
                       {204,0,0},        // Red
                       {0,204,0}};       // Green                                                                                  
int numColors = 2;                       // number of available colors

uint8_t errorColor[3] ={255,255,255};    //error/gameover color 
int index = 0;                           // color index
int displayColor = 2;                    // the color the button was when pressed

uint32_t lastChange = 0;                          // time of last neighbor state change
uint32_t colorChangeDelay = 1000;                 // amount of time between switching colors 
uint32_t neighborCheckDelay = 50;                 // time to wait after detecting neighbor state change before checking again. must be no shorter than readyDelay 

uint8_t buttonSequence[8]= {0,0,0,0,0,0,0,0};     // this will be used to remember sequence of presses
int buttonSequenceLength = 8;                     // length of sequence
int buttonIndex = 0;                              // tells us where in the remembered sequence of button presses to click

int readyDiff = 0;                                // difference between the current time and the last time the button was pressed
int lastStateTime = 0;                            // last time state was changed
int readyDelay = 50;                              // time to wait before going back to ready state after button press 

uint8_t oldNeighborStates[6] = {0,0,0,0,0,0};     // gets old neighborstates for the purpose of detecting neighbor state transition
uint8_t foundDifferentNeighbor = 0;
uint8_t uniqueNeighbor = 0;

void setup() {
   setTimeout(180);  
   setColorRGB(255, 0, 255);
}

void loop() {
    
  //random color changes
  //----------------------------------------------------------------------
  if(getState() != INCORRECT) {
    if(getTimer() - lastChange > colorChangeDelay) {                        // if enough time has passed since last change
      displayColor = displayColor % numColors == 0 ? 3 : 2;                 // advance color index
      setColorRGB(colors[displayColor][0],                                  // set our color using the color index
                  colors[displayColor][1], 
                  colors[displayColor][2]);
      lastChange = getTimer();                                              // mark the time of the state change
    }
  }

  //check for neighbor states shifting from ready to another state
  //----------------------------------------------------------------------
  
  // if it's been a while since we last checked neigbors
//  if (getTimer() - lastStateTime > neighborCheckDelay) {
//
//    uniqueNeighbor = 0; // initialize our unique neighbor to not unique at all...
//    
//    for (int i = 0; i < 6; i++ ) {
//
//      if(foundDifferentNeighbor == 0) {
//
//        // look for different neigbor
//        if(oldNeighborStates[i] == READY && getNeighbor(i) != 0 ) {
//          foundDifferentNeighbor = 1;
//          uniqueNeighbor = getNeighbor(i);
//          setState(uniqueNeighbor);
//          lastStateTime = getTimer();
//        }
//      }
//      oldNeighborStates[i] = getNeighbor(i);
//    }
//
//    if (uniqueNeighbor == 0) {
//      // don't have a unique neighbor
//    }
//    else if (uniqueNeighbor == CORRECT_RED || uniqueNeighbor == CORRECT_GREEN) {
//      
//      if(buttonSequence[buttonIndex] == 0) {          //if we've come to the end of the sequence
//        buttonSequence[buttonIndex] = displayColor;  //add the pressed color to the remembered sequence of presses
//        setState(displayColor);                      //setState to notify other Blinks of result of buttonPress
//        // show next players turn
//        nextPlayer();
//      }
//      else {
//        buttonIndex++;
//      }
//      
//    }
//    else if (uniqueNeighbor == INCORRECT) {   //if error, fade to the error color
//      setColor(errorColor);             
//      resetGame(); 
//    }
//  
//  } // end sporadic neighborhood check
  
  // go back to ready state after having been in another state for a bit
  if (getTimer() - lastStateTime >= readyDelay) {
    setState(READY); 
  }

}

void buttonClicked(){
        
  if (buttonSequence[buttonIndex] == 0) {        //if we've come to the end of the sequence
    buttonSequence[buttonIndex] = displayColor;  //add the pressed color to the remembered sequence of presses
    setState(displayColor);                      //setState to notify other Blinks of result of buttonPress
    nextPlayer();
  }
   
  else {  //Else we are in the middle of a sequence
    
    if (displayColor != buttonSequence[buttonIndex]) { //If it’s an error
        setColorRGB(errorColor[0],
              errorColor[1],
              errorColor[2]);  // turn the error color
        setState(INCORRECT);   // setState to INCORRECT to alert other tiles of an error
        resetGame();           // Reset the game*
    }  
    else {
      buttonIndex++;           //Increment index (for checking color state against memory)
      setState(displayColor);  //setState to CORRECT_RED or CORRECT_GREEN
    }
  }

  lastStateTime = getTimer();
}

void nextPlayer() {
  buttonIndex = 0; //set the buttonIndex back to the beginning of the remembered sequence
  //next player turn signal
}

void resetGame() { // Reset the game
  //clearing the button press memory
  for(int i = 0; i<buttonSequenceLength; i++) {
      buttonSequence[i] = 0;
  }
  buttonIndex = 0; //set the index for checking color state back to zero)
}

void buttonDoubleClicked(){}

void buttonTripleClicked(){}

void buttonPressed() {}

void buttonReleased() {}

void buttonLongPressed(){}
