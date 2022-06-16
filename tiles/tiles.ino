#include <stdio.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <LiquidCrystal_I2C.h>

#define PIN_NEO_PIXEL  13
#define NUM_PIXELS     512

#define MAX_LENGTH 200

#define SONG_LENGTH 10

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool inMenu = true;
int score = 0;

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

float current_song[MAX_LENGTH][2] = {};

float menu_song[MAX_LENGTH][2] = {
//  {9,NOTE_C4},
//  {10,NOTE_C4},
//  {11,NOTE_C4},
//  {12,NOTE_C4},
//  {14,NOTE_C4},
//  {17,NOTE_C4},
//  {18,NOTE_C4},
//  {19,NOTE_C4},
};

int IndexFix(int pixel) { // Fixes pixel indexing cuz the original one is TRASH
  int i = pixel / 16;
  if (i % 2 == 0) pixel = (((i+1) * 16) - pixel - 1) + (i * 16);
  return pixel;
}

void ChooseSong(int song) {
  for (int i = 0; i < MAX_LENGTH; i++) {
    if (song == 1) {
      current_song[i][0] = wooded[i][0];
      current_song[i][1] = wooded[i][1];
    }
  }
}

void DrawTiles(float song[MAX_LENGTH][2]) { // Draws the tiles on the screen. if it goes offscreen, it doesn't render the pixel
  for (int i = 0; i < SONG_LENGTH; i++) {
    int posy = 3 + (song[i][0] * 4);
    int posx = 64 * (song[i][1] - 1);

    for (int j = 0; j < 5; j++) {
      
      int rgb[3] = {0,0,0};
      if (song[i][1] == 1) rgb[0] = 10;
      if (song[i][1] == 2) rgb[2] = 10;
      if (song[i][1] == 3) rgb[1] = 10;
      if (song[i][1] == 4) rgb[0] = 10;
      if (song[i][1] == 4) rgb[1] = 10;
      
      if (j == 4) {
        rgb[0] = 0;
        rgb[1] = 0;
        rgb[2] = 0;
      }
      if (posy + j < 32 && posy + j >= 0) {
        if (posy + j >= 16) {
          NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx + 256 - 16), NeoPixel.Color(rgb[0], rgb[1], rgb[2]));
          NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx + 256 - 16), NeoPixel.Color(rgb[0], rgb[1], rgb[2]));
          NeoPixel.setPixelColor(IndexFix(posy + j + 48 + posx + 256 - 16), NeoPixel.Color(rgb[0], rgb[1], rgb[2]));
        } else {
          NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx), NeoPixel.Color(rgb[0], rgb[1], rgb[2]));
          NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx), NeoPixel.Color(rgb[0], rgb[1], rgb[2]));
          NeoPixel.setPixelColor(IndexFix(posy + j + 48 + posx), NeoPixel.Color(rgb[0], rgb[1], rgb[2]));
        }
      }
    }
  }
}

void ShowGrid() { // shows the grid that seperates the 4 notes and shows the line where you need to time the note.
  for (int i = 0; i < 16; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 64; i < 80; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 128; i < 144; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 192; i < 208; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 256; i < 272; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 320; i < 336; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 384; i < 400; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
     
  for (int i = 448; i < 464; i++)
     NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(3, 1, 0));
  
  for (int i = 0; i < 16; i++)
     NeoPixel.setPixelColor(IndexFix((i*16) + 3), NeoPixel.Color(0, 5, 0));
}

int noteTimer = 0;
int combo = 0;
int notesPassed = 0;
int misses = 0;

void ShowScore() {
  if (!inMenu) {
    
    lcd.clear();
    
    lcd.setCursor(0, 0);
    lcd.print(combo);
    lcd.print("x Combo");
  
    lcd.setCursor(0, 1);
    lcd.print("Score: ");
    lcd.print(score);
  
  //  lcd.setCursor(12, 1);
  //  int accuracy = round((notesPassed - misses) / notesPassed * 100);
  //  lcd.print(accuracy);
  //  lcd.print("%");
  }
}

void Hit() {
  score += 100;
  notesPassed += 1;
  combo += 1;
}

void Miss() {
  notesPassed += 1;
  score -= 30;
  if (score < 0) score = 0;
  misses += 1;
  combo = 0;
}

void MoveTiles() { // Moves the tiles in a 2d array downwards. I should make this take in a 2d array but idk how lol
  for (int i = 0; i < SONG_LENGTH; i++) {
    if (current_song[i][0] > -3) current_song[i][0] -= 0.25f;
    if (current_song[i][0] == -1) {
      Miss();
    }
    
  }
}

bool button1Pressed = false;
bool button2Pressed = false;
bool button3Pressed = false;
bool button4Pressed = false;

void CheckHit(int buttonPressed) {
  bool success = false;
  for (int i = 0; i < SONG_LENGTH; i++) {
    if (current_song[i][0] <= 0.25f && current_song[i][0] >= -1.25f && current_song[i][1] == buttonPressed) {
      Hit();

      int posy = 3 + (current_song[i][0] * 4); //Clear the tile after it is hit correctly
      int posx = 64 * (current_song[i][1] - 1);
  
      for (int j = 0; j < 4; j++) {
        if (posy + j < 32 && posy + j >= 0) {
          if (posy + j >= 16) {
            NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx + 256 - 16), NeoPixel.Color(0, 0, 0));
            NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx + 256 - 16), NeoPixel.Color(0, 0, 0));
            NeoPixel.setPixelColor(IndexFix(posy + j + 48 + posx + 256 - 16), NeoPixel.Color(0, 0, 0));
          } else {
            NeoPixel.setPixelColor(IndexFix(posy + j + 16 + posx), NeoPixel.Color(0, 0, 0));
            NeoPixel.setPixelColor(IndexFix(posy + j + 32 + posx), NeoPixel.Color(0, 0, 0));
            NeoPixel.setPixelColor(IndexFix(posy + j + 48 + posx), NeoPixel.Color(0, 0, 0));
          }
        }
      }
      
      current_song[i][0] = -2;
      success = true;
    }
  }
  
  if (!success) Miss();
}

void CheckButtons() { // checks for button presses. PIN 1 is the first button, PIN 2 is the second one, and so on.
  int buttonPressed = 0;
  
  if (digitalRead(4) == HIGH) {
    if (!button1Pressed) {
      buttonPressed = 1;
      for (int i = 16; i <= 48; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button1Pressed = true;
  } else button1Pressed = false;
  
  if (digitalRead(5) == HIGH) {
    if (!button2Pressed) {
      buttonPressed = 2;
      for (int i = 80; i <= 112; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button2Pressed = true;
  } else button2Pressed = false;
  
  if (digitalRead(6) == HIGH) {
    if (!button3Pressed) {
      buttonPressed = 3;
      for (int i = 144; i <= 176; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button3Pressed = true;
  } else button3Pressed = false;
  
  if (digitalRead(7) == HIGH) {
    if (!button4Pressed) {
      buttonPressed = 4;
      for (int i = 208; i <= 240; i+=16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }
    
    button4Pressed = true;
  } else button4Pressed = false;
}

byte left_arrow[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

byte right_arrow[8] = {
  0b00000,
  0b01000,
  0b01100,
  0b01110,
  0b01110,
  0b01100,
  0b01000,
  0b00000
};

int selected_song = 1;

void ShowMenu() {
  lcd.createChar(0, left_arrow);
  lcd.createChar(1, right_arrow);
  lcd.setCursor(2, 0);
  lcd.print("Choose song:");
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(15, 1);
  lcd.write((byte)1);

  if (selected_song == 1) {
    lcd.setCursor(1, 1);
    lcd.print("Wooded Kingdom");
  }
  
  for (int i = 42; i <= 43; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(5, 0, 0));
  for (int i = 57; i <= 60; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(5, 0, 0));
  for (int i = 73; i <= 76; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(5, 0, 0));
  for (int i = 90; i <= 91; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(5, 0, 0));

  for (int i = 170; i <= 171; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(0, 5, 0));
  for (int i = 185; i <= 188; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(0, 5, 0));
  for (int i = 201; i <= 204; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(0, 5, 0));
  for (int i = 218; i <= 219; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(0, 5, 0));

  for (int i = 35; i <= 37; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  for (int i = 50; i <= 54; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  for (int i = 20; i <= 100; i+=16) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));

  for (int i = 211; i <= 213; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  for (int i = 194; i <= 198; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  for (int i = 148; i <= 228; i+=16) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  
  /* --------------- */
  
  for (int i = 19; i <= 20; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));
  for (int i = 34; i <= 37; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));
  for (int i = 50; i <= 53; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));
  for (int i = 67; i <= 68; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));

  for (int i = 97; i <= 102; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 118; i <= 150; i+=16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 113; i <= 145; i+=16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 146; i <= 147; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  NeoPixel.setPixelColor(IndexFix(131 + 256), NeoPixel.Color(1, 1, 0));

  for (int i = 177; i <= 182; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 225; i <= 230; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 193; i <= 209; i+=16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 198; i <= 214; i+=16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));

  /* --------------- */

  for (int i = 74; i <= 76; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 0, 0));
  for (int i = 90; i <= 92; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 0, 0));

  for (int i = 171; i <= 173; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 187; i <= 189; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));

  for (int i = 140; i <= 142; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 1, 0));
  for (int i = 156; i <= 158; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 1, 0));

  for (int i = 110; i <= 111; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 1));
  for (int i = 126; i <= 127; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 1));
}

int lcdUpdate = 0;

void loop() {
  if (!inMenu) {
    DrawTiles(current_song);
    ShowGrid();
    CheckButtons();
    MoveTiles();
  
    lcdUpdate += 1;
    if (lcdUpdate >= 10) {
      ShowScore();
      lcdUpdate = 0;
    }
    delay(30);
  }
  else {
    
  }
  NeoPixel.show();
}

void setup() {
  NeoPixel.begin();
  NeoPixel.clear();
  NeoPixel.show();

  lcd.init();
  lcd.backlight();
  lcd.clear();
  ShowScore();

  if (inMenu) {
    ShowMenu();
  }

  Serial.begin(9600);
  ChooseSong(1);

//  melody.begin(8);
}
