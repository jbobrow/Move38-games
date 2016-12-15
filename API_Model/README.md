void getNeighbor(int n)

boolean isAlone()


void setColor(int r, int g, int b)  // instant


void fadeTo(Color c, int ms)  // timed change to color

void fadeToAndReturn(Color c, int ms)  // timed change to color and back

void fadeTo(int r, int g, int b, int ms)  //


void blink(int ms) // defaults to on/off of current color

void blink(int ms, Color c) // defaults to on/off of this color

void blink(int ms, Color[n] c) // send array of colors to blink between

void blink(int ms, int min, int max) // low and high levels for blinking and the time between them

void blink(int ms, Color c, int min, int max) // low and high levels for blinking and the time between them


void pulse(int ms) // phase

void pulse(int ms, int min, int max) // phase w/ low and high brightness

void pulse(int ms, Colors[n] c) // phased pulse between colors (depends on fadeTo)

//setButtonCallback(button);

void buttonPressed()
void buttonReleased()

void setButtonClickThreshold(int ms); // defaulted to 200ms, but function available to make slower or faster clicking part of the game

void buttonClicked()
void buttonDoubleClicked()
void buttonTripleClicked()

void buttonLongPressed()


// Library
// When connected, flash brighter
//
