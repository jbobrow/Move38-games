/*
 *  Competitive Disco Simon
 *   
 *  by Nick Bentley
 *  01.28.2017
 */
 
uint8_t colors[2][3] = {{204,0,0},        // Red
                       {0,204,0}};       // Green                                                               
int numColors = 2;  // number of available colors

uint8_t errorColor[3] ={0,0,204};  //error/gameover color 
int delays[6] = {500,700,900,1100,1300,1500}; 
int delayIndex = 0; 
int numDelays = 6; //the number of different intervals for color changes
int index = 0;      // color index
int pressedColor = 0; // the color the button was when pressed
uint32_t lastChange = 0; //time of last neighbor state change
uint32_t timeDiff = 0;
uint32_t Pause = 1000;  //amount of time between switching colors 
uint32_t checkTime = 0; //amount of time passed since last checking for neighbor state change
uint32_t checkPause = 50; // time to wait after detecting neighbor state change before checking again. must be no shorter than readyDelay 
uint8_t startColor[] = {255, 0, 0};
uint8_t buttonSequence[8]= {0,0,0,0,0,0,0,0}; //this will be used to remember sequence of presses
int buttonIndex = 0; //tells us where in the remembered sequenc of button presses to click
int readyDiff = 0; // difference between the current time and the last time the button was pressed
int readyDiffLoop = 0; // difference between the current time and the last time a new state happened, in loop
int lastPress = 0; //last time the button was pressed
int lastStateTime = 0; //last time state was changed, in loop
int readyDelay = 50; // time to wait before going back to ready state after button press 
uint8_t oldNeighborStates[] = {0,0,0,0,0,0}; //gets old neighborstates for the purpose of detecting neighbor state transition
uint8_t currentNeighborStates[] = {0,0,0,0,0,0}; //gets current neighbor states
uint8_t newNeighbor = 0; 

void setup() {
   setTimeout(180);  
   timeDiff = 0; 
   setColor(startColor);
}

void loop() {
    //random color changes-----------------------------------------------------------
    timeDiff = getTimer() - lastChange; 
    if (timeDiff > Pause) { // if enough time has passed since last change
      lastChange = getTimer(); // mark the time of the state change
      index = (index+1) % numColors; //advance color index    
      pressedColor = index + 2;  // will be compared to button press to decide if correct      
      setColorRGB(colors[index][0], colors[index][1], colors[index][2]);
      delayIndex = getTimer() % numDelays; //determine which delay to use
      Pause = delays[delayIndex]; //choose a new random interval for next color change
    } //-----------------------------------------------------------------------------  

   //check for neighbor states shifting from ready to another state------------------
   int i;
   for (i = 0; i <= 5; i++ ) {
     checkTime = getTimer() - lastStateTime; 
     if ((oldNeighborStates[i] == 0) && (getNeighbor(i)!=0) && (checkTime>checkPause)) { //if neighbor state transition from 0 to anything else
       newNeighbor = getNeighbor(i); 
       setState(newNeighbor);
       lastStateTime = getTimer();  

         if (newNeighbor==4) {   //if error
            fadeTo(errorColor[0], errorColor[1], errorColor[2], 200);
            resetGame(); 
         }

         else if (buttonSequence[buttonIndex] == 0) {//if we've come to the end of the sequence
            buttonSequence[buttonIndex] = pressedColor;  //add the pressed color to the remembered sequence of presses
            setState(pressedColor); //setState to notify other Blinks of result of buttonPress
         }

         else {
            buttonIndex++; //Increment index (for checking color state against memory)
         }
     } 
   }
   
   //go back to ready state after having been in another state for a bit
   readyDiffLoop = getTimer() - lastStateTime;
   if (readyDiffLoop>=readyDelay) {
        setState(1); 
   }

}

void buttonClicked(){

if (buttonSequence[buttonIndex] == 0) {//if we've come to the end of the sequence
      buttonSequence[buttonIndex] = pressedColor;  //add the pressed color to the remembered sequence of presses
      setState(pressedColor); //setState to notify other Blinks of result of buttonPress
}
 
else {  //Else we are in the middle of a sequence
        if (pressedColor != buttonSequence[buttonIndex]) { //If it’s an error       
            fadeTo(errorColor[0], errorColor[1], errorColor[2], 1000);
            setState(4);   //setState to 4 to alert other tiles of an error
            resetGame();   //Reset the game*
        }
          
        else  {
          buttonIndex++; //Increment index (for checking color state against memory)
          setState(pressedColor); //setState to 2 or 3 (red or green)  
        }         
}

//return to ready state after delay
readyDiff = getTimer() - lastPress;
if (readyDiff>=readyDelay) {
  setState(1); 
}
  
}

void nextPlayer() {
buttonIndex = 0; //set the buttonIndex back to the beginning of the remembered sequence
//next player turn signal
}

void resetGame() { // Reset the game
uint8_t buttonSequence[8]= {0,0,0,0,0,0,0,0}; //clearing the button press memory
buttonIndex = 0; //set the index for checking color state back to zero)
}



void buttonDoubleClicked(){
}

void buttonTripleClicked(){

}

void buttonPressed() {

}

void buttonReleased() {
}

void buttonLongPressed(){
}
