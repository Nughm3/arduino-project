#include <stdio.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN_NEO_PIXEL  13
#define NUM_PIXELS     512

#define MAX_LENGTH 200

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

float wooded[MAX_LENGTH][2] = {
    {5,1},
    {6,2},
    {7,3},
    {8,4},
    {10,4},
};

int IndexFix(int pixel) {
  int i = pixel / 16;
  if (i % 2 == 0) {
    pixel = (((i+1) * 16) - pixel - 1) + (i * 16);
  }
  return pixel;
}

void DrawTiles(float song[MAX_LENGTH][2]) {
  for (int i = 0; i < 5; i++) {
    int posy = 3 + (song[i][0] * 4);
    int posx = 80 * (song[i][1] - 1);

    for (int j = 0; j < 4; j++) {
      if (posy + j < 32) {
        if (posy + j >= 16) {
          NeoPixel.setPixelColor(IndexFix(posy + j + posx + 256), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx + 256), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx + 256), NeoPixel.Color(10, 0, 0));
        }
        else {
          NeoPixel.setPixelColor(IndexFix(posy + j + posx), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx), NeoPixel.Color(10, 0, 0));
          NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx), NeoPixel.Color(10, 0, 0));
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

//float MoveTiles(float song[]) {
//  for (int i = 0; i < 5; i++) {
////    song[i][0] -= 0.25f;
//  }
//  return song;
//}

void loop() {
  NeoPixel.clear();
//  wooded = MoveTiles(wooded);

  DrawTiles(wooded);
  ShowGrid();
  NeoPixel.show();
  delay(250);
}

void setup() {
  NeoPixel.begin();
}
