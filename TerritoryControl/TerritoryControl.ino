/*  Territory control w/ time(Game)
 *  
 *  Setup: 2 player game. Tiles die slowly over time. Quicker when surrounded by enemies, and slower when surrounded by family.
 *    
 *  Technical Details:
 *    A long press on the tile changes the color of the tile for prototyping (switch state 1 or 2)
 *    
 *    Tiles resets to wait mode when double press occurs (pulse white)
 *    
 *    Game starts when single press in wait mode (enter player 1 & player 2 states)
 *    Shares the start message with neighbors (hold on green for 3 pulses...)
 *    
 *    States for game piece
 *    0 = no piece (never in state 0)
 *    1 = player 1 alive (glow blue)
 *    2 = player 2 alive (glow red)
 *    3 = player 1/2 dead (pulse white)
 *    4 = wait mode (pulse white)
 *    5 = start mode (green starting lights pattern)
 * 
 *  Game devopment by: Nick Bently, Jonathan Bobrow
 *
 *  --------------------------------------------------------------------------------------------------
 *  IMPORTANT: To use this code in Arduino's IDE, first move the AutomaTiles folder
 *  into the right directory i.e. <user home directory>/Documents/Arduino/hardware/AutomaTiles
 *  Then open the Arduino IDE and select Tools > Board > "AutomaTile"
 *  Now you should be good to go :) (thanks to the hard work of Joshua Sloane)
 *  -------------------------------------------------------------------------------------------------- 
 *     
 *  by Jonathan Bobrow
 *  10.07.2016
 */

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
