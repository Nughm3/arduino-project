#include <stdio.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_NEO_PIXEL  13
#define NUM_PIXELS     512

#define MAX_LENGTH 200

#define SONG_LENGTH 10

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

int score = 0;
int misses = 0;

float wooded[MAX_LENGTH][2] = { // The song. "Wooded Kingdom" - Super Mario Odyssey
    {9,1},
    {10,2},
    {11,3},
    {12,1},
    {12,4},
    {14,1},
    {14,4},
    {17,3},
    {18,1},
    {19,2},
};

int IndexFix(int pixel) { // Fixes pixel indexing cuz the original one is TRASH
  int i = pixel / 16;
  if (i % 2 == 0) {
    pixel = (((i+1) * 16) - pixel - 1) + (i * 16);
  }
  return pixel;
}

void DrawTiles(float song[MAX_LENGTH][2]) { // Draws the tiles on the screen. if it goes offscreen, it doesn't render the pixel
  for (int i = 0; i < SONG_LENGTH; i++) {
    int posy = 3 + (song[i][0] * 4);
    int posx = 64 * (song[i][1] - 1);

    for (int j = 0; j < 4; j++) {
      if (posy + j < 32 && posy + j >= 0) {
        if (posy + j >= 16) {
          NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx + 256 - 16), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx + 256 - 16), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 48 + posx + 256 - 16), NeoPixel.Color(10, 0, 0));
        }
        else {
          NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 48 + posx), NeoPixel.Color(10, 0, 0));
        }
      }
    }
  }
}

void ShowGrid() { // shows the grid that seperates the 4 notes and shows the line where you need to time the note.
  for (int i = 0; i < 16; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }
  for (int i = 64; i < 80; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }
  for (int i = 128; i < 144; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }
  for (int i = 192; i < 208; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }

  for (int i = 256; i < 272; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }
  for (int i = 320; i < 336; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }
  for (int i = 384; i < 400; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }
  for (int i = 448; i < 464; i++) {
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  }  
  
  for (int i = 0; i < 16; i++) {
     NeoPixel.setPixelColor(IndexFix((i*16) + 3), NeoPixel.Color(0, 5, 0));
  }
}

void MoveTiles() { // Moves the tiles in a 2d array downwards. I should make this take in a 2d array but idk how lol
  for (int i = 0; i < SONG_LENGTH; i++) {
    wooded[i][0] -= 0.25f;
    if (round(wooded[i][0]) == -1) {
      misses += 1;
    }
  }
}

bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;
bool button4Pressed = false;

void CheckHit(int buttonPressed) {
  for (int i = 0; i < SONG_LENGTH; i++) {
    if (wooded[i][0] <= 0.25f && wooded[i][0] >= -1.25f && wooded[i][1] == buttonPressed) {
      score += 1;
      Serial.println("yes");
      wooded[i][1] = -2;
    }
    else {
      misses += 1;
//      Serial.println("miss");
    }
  }
}

void CheckButtons() { // W.I.P checks for button presses. PIN 1 is the first button, PIN 2 is the second one, and so on.
  int buttonPressed = 0;
  
  if (digitalRead(1) == HIGH) {
    if (!button1Pressed) {
      buttonPressed = 1;
      for (int i = 16; i <= 48; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button1Pressed = true;
  }
  else button1Pressed = false;
  
  if (digitalRead(2) == HIGH) {
    if (!button2Pressed) {
      buttonPressed = 2;
      for (int i = 80; i <= 112; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button2Pressed = true;
  }
  else button2Pressed = false;
  
  if (digitalRead(3) == HIGH) {
    if (!button3Pressed) {
      buttonPressed = 3;
      for (int i = 144; i <= 176; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button3Pressed = true;
  }
  else button3Pressed = false;
  
  if (digitalRead(4) == HIGH) {
    if (!button4Pressed) {
      buttonPressed = 4;
      for (int i = 208; i <= 240; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button4Pressed = true;
  }
  else button4Pressed = false;
}

int delayMove = 0; // The timer delay for the tiles moving down.

void loop() {
  NeoPixel.clear(); // yea you know what this does
  
  if (delayMove >= 20) { // Every time this counter goes to 20 the notes/tiles move down 1 pixel.
    MoveTiles();
    delayMove = 0;
  }

  DrawTiles(wooded);
  ShowGrid();
  CheckButtons();
  NeoPixel.show();

  delayMove += 10;
  delay(10);

}

void setup() {
  NeoPixel.begin();
  pinMode(12, INPUT);
  Serial.begin(9600);
}
