#include <stdio.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_NEO_PIXEL  13
#define NUM_PIXELS     512

#define MAX_LENGTH 200

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

int score = 0;
int misses = 0;

float wooded[MAX_LENGTH][2] = {
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

int IndexFix(int pixel) {
  int i = pixel / 16;
  if (i % 2 == 0) {
    pixel = (((i+1) * 16) - pixel - 1) + (i * 16);
  }
  return pixel;
}

void DrawTiles(float song[MAX_LENGTH][2]) {
  for (int i = 0; i < 10; i++) {
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

void ShowGrid() {
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

void MoveTiles() {
  for (int i = 0; i < 10; i++) {
    wooded[i][0] -= 0.25f;
    if (round(wooded[i][0]) == -1) {
      misses += 1;
    }
  }
}

void CheckButtons() {
  if (digitalRead(12) == LOW) {
    for (int i = 16; i <= 48; i+=16) {
      NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
    }
  }
  if (digitalRead(11) == LOW) {
    
  }
  if (digitalRead(10) == LOW) {
    
  }
  if (digitalRead(9) == LOW) {
    
  }
}

int delayMove = 0;

void loop() {
  NeoPixel.clear();
  
  if (delayMove >= 20) {
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
