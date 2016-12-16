##Blinks API##

A simple API for programming Blink tiles to perform needed tasks
**A default blank sketch**
```c
void setup() {  
}

void loop() {
	// continuous time logic here
}

void buttonPressed() {
	// handle button down here
}

void onStep() {
	// discrete time logic here
}

```

###Blink methods###

**getNeighbor**
```c
int getNeighbor(int id);
// pass it a neighbor id (0-5) and get the value for that neighbor returned
```

**isAlone**
```c
boolean isAlone();
// returns true when the tile has no neighbors
// same as checking all 6 sides and seeing that they return 0
```

**setColor**
```c
void setColor(int r, int g, int b);
// instantly changes the color of the RGB LED to the values passed
```

**fadeTo**
```c
void fadeTo(Color c, int ms);  // timed change to color

void fadeToAndReturn(Color c, int ms);  // timed change to color and back

void fadeTo(int r, int g, int b, int ms);  //
```

**blink**
```c
void blink(int ms); // defaults to on/off of current color

void blink(int ms, Color c); // defaults to on/off of this color

void blink(int ms, Color[n] c); // send array of colors to blink between

void blink(int ms, int min, int max); // low and high levels for blinking and the time between them

void blink(int ms, Color c, int min, int max); // low and high levels for blinking and the time between them
```

**pulse**
```c
void pulse(int ms); // phase

void pulse(int ms, int min, int max); // phase w/ low and high brightness

void pulse(int ms, Colors[n] c); // phased pulse between colors (depends on fadeTo)
```

**button**
```c
void buttonPressed();
void buttonReleased();

void setButtonClickThreshold(int ms); // defaulted to 200ms, but function available to make slower or faster clicking part of the game

void buttonClicked();
void buttonDoubleClicked();
void buttonTripleClicked();

void buttonLongPressed();
```
