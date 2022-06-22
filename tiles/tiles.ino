#include <stdio.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <LiquidCrystal_I2C.h>

#define PIN_NEO_PIXEL  13
#define NUM_PIXELS     512

#define MAX_LENGTH 150

#define SONG_LENGTH 150
#define MENU_SONG_LENGTH 146

#include <Tone.h>
Tone bass;
Tone melody;

Adafruit_NeoPixel NeoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 16, 2);

bool inMenu = true;
int score = 0;
int selected_song = 1;

float song_length[3] = {26000, 29000, 18000};
float song_speed[3] = {11, 8, 10};

float gusty[MAX_LENGTH][2] = { // Song 1 Layout. "Gusty Garden Galaxy" - Super Mario Galaxy (2007)
  {8, 1},
  {9.75, 3},
  {10.5, 4},
  {11.25, 3},
  {12, 4},
  {14, 1},
  {15.5, 1},
  {16, 2},
  {18, 1},
  {18.75, 2},
  {19.5, 3},
  {20.25, 4},
  {21.5, 3},
  {22.75, 2},
  {24, 1},
  {27, 1},
  {27, 3},
  {30, 1},
  {30, 3},
  {32, 1},
  {32, 4},
  {35, 1},
  {35, 4},
  {38, 1},
  {38, 4},
  {40, 2},
  {40, 4},
  {43, 2},
  {43, 4},
  {46, 2},
  {46, 4},
  {48, 1},
  {48, 3},
  {51, 1},
  {51, 3},
  {54, 3},
  /*------*/
  {56, 3},
  {61, 2},
  {62, 4},
  {63, 3},
  {64, 2},
  {69, 1},
  {70, 2},
  {71, 3},
  {72, 2},
  {75, 1},
  {80, 2},
  {83, 2},
  {86, 1},
  {88, 3},
  {93, 2},
  {94, 4},
  {95, 3},
  {96, 2},
  {99, 1},
  {101, 2},
  {103, 3},
  {104, 2},
  {107, 1},
  {118, 1},
  {120, 4},
  {125, 1},
  {126, 3},
  {127, 2},
  {128, 3},
  {131, 2},
  {133, 1},
  {134, 2},
  {135, 3},
  {136, 4},
  {141, 2},
  {142, 4},
  {143, 3},
  {144, 2},
  {147, 1},
  {152, 2},
  {157, 1},
  {158, 3},
  {159, 2},
  {160, 1},
  {163, 2},
  {165, 3},
  {167, 4},
};

float wooded[MAX_LENGTH][2] = { // Song 2 Layout. "Wooded Kingdom" - Super Mario Odyssey (2017)
  {9, 1},
  {10, 2},
  {11, 3},
  {12, 1},
  {12, 4},
  {14, 1},
  {14, 4},
  {17, 3},
  {18, 1},
  {19, 2},
  {25, 1},
  {26, 2},
  {27, 3},
  {28, 2},
  {28, 4},
  {30, 2},
  {30, 4},
  {33, 4},
  {34, 3},
  {35, 1},
  {41, 1},
  {42, 2},
  {43, 3},
  {44, 4},
  {44, 3},
  {46, 4},
  {46, 3},
  {49, 3},
  {50, 2},
  {51, 1},
  {57, 4},
  {58, 2},
  {59, 3},
  /*------*/
  {73, 1},
  {74, 2},
  {75, 3},
  {76, 4},
  {78, 4},
  {76, 1},
  {78, 1},
  {80, 1},
  {82, 1},
  {82, 4},
  {83, 3},
  {84, 1},
  {86, 2},
  {88, 1},
  {90, 2},
  {92, 3},
  {94, 3},
  {97, 2},
  {98, 3},
  {99, 2},
  {92, 1},
  {94, 1},
  {96, 1},
  {98, 1},
  
  {97, 1},
  {98, 2},
  {99, 3},
  {100, 4},
  {102, 4},
  {100, 1},
  {102, 1},
  {104, 1},
  {106, 1},
  {106, 4},
  {107, 3},
  {108, 2},
  {110, 2},
  {112, 2},
  {114, 2},
  {116, 4},
  {118, 4},
  {121, 2},
  {122, 3},
  {123, 4},
  {116, 1},
  {118, 1},
  {120, 1},
  {122, 1},
  {124, 1},
  {124.5, 2},
  {125, 3},
  {125.5, 4},
  {126, 1},
  {126.5, 2},
  {127, 3},
  {127.5, 4},
  {128, 3},
  {129, 1},
  {130, 2},
  {131, 3},
  /*------*/
  {132, 3},
  {132, 4},
  {135, 2},
  {135, 3},
  {138, 1},
  {138, 2},

  {145, 1},
  {146, 2},
  {147, 3},
  
  {148, 1},
  {148, 2},
  {151, 2},
  {151, 3},
  {154, 3},
  {154, 4},

  {161, 1},
  {162, 2},
  {163, 3},

  {164, 3},
  {164, 4},
  {167, 2},
  {167, 3},
  {170, 1},
  {170, 2},

  {177, 3},
  {178, 1},
  {179, 3},
  {180, 1},
};

float yoshi[MAX_LENGTH][2] = { // Song 3 Layout. "Athletic Theme" - Yoshi's Island (1995)
  {8, 4},
  {8.5, 3},
  {9, 1},
  {9.5, 4},
  {10.5, 3},
  {11, 1},

  {12, 4},
  {12.5, 3},
  {13, 1},
  {13.5, 4},
  {14.5, 3},
  {15, 1},

  {16, 1},
  {16.5, 2},
  {17, 1},
  {17.5, 4},
  {18.5, 3},
  {19, 2},
  
  {20, 3},
  {21, 1},
  {22, 2},
  {23, 3},
  /*------*/
  {24, 1},
  {25, 2},
  {26, 3},
  {27, 4},
  {28, 3},
  {29, 3},
  {31, 4},
  {32, 4},
  {33, 3},
  {34, 3},
  {35, 2},
  {36, 1},
  {39, 4},
  {26, 1},
  {27, 2},
  {28, 1},
  {29, 2},
  {30, 1},
  {31, 2},
  {32, 1},
  {33, 2},

  {40, 4},
  {41, 3},
  {42, 4},
  {43, 3},
  {44, 4},
  {47, 3},
  {48, 4},
  {49, 3},
  {50, 3},
  {51, 2},
  {52, 1},
  {55, 2},
  {40, 1},
  {41, 1},
  {42, 2},
  {43, 2},
  {44, 1},
  {45, 1},
  {46, 2},
  {47, 2},

  {56, 1},
  {57, 2},
  {58, 3},
  {59, 4},
  {60, 3},
  {61, 3},
  {63, 4},
  {64, 4},
  {65, 3},
  {66, 2},
  {67, 4},
  {68, 2},
  {71, 2},
  {72, 2},
  {73, 1},
  {74, 2},
  {75, 3},
  {76, 2},
  {78, 1},
  {79, 4},
  {80, 3},
  {81, 4},
  {82, 1},
  {83, 4},
  {84, 3},
  {85, 4},
  {86, 1},
  {87, 4},
  {68, 1},
  {69, 1},
  {70, 1},
  {71, 1},
  /*------*/
  {72, 4},
  {72.5, 3},
  {73.5, 4},
  {74, 3},
  {75, 4},
  {75.5, 3},
  {76.5, 4},
  {77, 3},
  {78, 4},
  {79, 3},
  {80, 3},
  {80.5, 2},
  {81.5, 3},
  {82, 2},
  {83, 3},
  {83.5, 2},
  {84, 4},
  {85, 3},
  {86, 2},
  /*------*/
  {88, 3},
  {88.5, 2},
  {89.5, 3},
  {90, 2},
  {91, 3},
  {91.5, 2},
  {92.5, 3},
  {93, 2},
  {94, 3},
  {95, 2},
  {96, 2},
  {96.5, 1},
  {97.5, 2},
  {98, 1},
  {99, 2},
  {99.5, 1},
  {100, 4},
  {101, 4},
  {102, 3},
  {103, 2},
  {104, 1}};

float current_song[MAX_LENGTH][2] = {};

float menu_song[MENU_SONG_LENGTH][3] = { // Menu Music. "Delfino Plaza" - Super Mario Sunshine (2002)

  {12.125, NOTE_FS4, 0},
  {12.25, NOTE_CS5, 0},
  {12.5, NOTE_CS5, 0},
  {12.75, NOTE_E4, 0},
  {13, NOTE_D4, 0},
  {13.375, NOTE_B4, 0},
  {14.125, NOTE_FS4, 0},
  {14.25, NOTE_CS5, 0},
  {14.5, NOTE_CS5, 0},
  {14.75, NOTE_E4, 0},
  {15, NOTE_D4, 0},
  {15.375, NOTE_B4, 0},
  {16.125, NOTE_FS4, 0},
  {16.25, NOTE_CS5, 0},
  {16.5, NOTE_CS5, 0},
  {16.75, NOTE_CS5, 0},
  {17, NOTE_D5, 0},
  {17.375, NOTE_FS4, 0},
  {17.625, NOTE_FS4, 0},
  {17.75, NOTE_GS4, 0},
  {18, NOTE_A4, 0},

  {28.25, NOTE_D5, 0},
  {29, NOTE_FS4, 0},
  {29.5, NOTE_D5, 0},
  {30, NOTE_CS5, 0},
  {32.25, NOTE_D5, 0},
  {33, NOTE_FS4, 0},
  {33.5, NOTE_D5, 0},
  {34, NOTE_CS5, 0},
  {35, NOTE_A4, 0},

  {36.25, NOTE_D5, 0},
  {37, NOTE_FS4, 0},
  {37.5, NOTE_D5, 0},
  {38, NOTE_CS5, 0},
  {38.5, NOTE_B4, 0},
  {39, NOTE_AS4, 0},
  {40.25, NOTE_A4, 0},
  {41, NOTE_F4, 0},
  {41.5, NOTE_A4, 0},
  {42, NOTE_GS4, 0},
  {43, NOTE_E5, 0},
  /* -------------------- */

  {12, NOTE_A2, 1},
  {12.25, NOTE_E3, 1},
  {12.5, NOTE_AS2, 1},
  {12.75, NOTE_E3, 1},
  {13, NOTE_B2, 1},
  {13.25, NOTE_FS3, 1},
  {13.5, NOTE_E3, 1},
  {13.75, NOTE_GS3, 1},
  {14, NOTE_A2, 1},
  {14.25, NOTE_E3, 1},
  {14.5, NOTE_AS2, 1},
  {14.75, NOTE_E3, 1},
  {15, NOTE_B2, 1},
  {15.25, NOTE_FS3, 1},
  {15.5, NOTE_E3, 1},
  {15.75, NOTE_GS3, 1},
  {16, NOTE_A2, 1},
  {16.25, NOTE_E3, 1},
  {16.5, NOTE_AS2, 1},
  {16.75, NOTE_E3, 1},
  {17, NOTE_B2, 1},
  {17.25, NOTE_FS3, 1},
  {17.5, NOTE_E3, 1},
  {17.75, NOTE_GS3, 1},
  {18, NOTE_A2, 1},
  {18.25, NOTE_E3, 1},
  {18.5, NOTE_AS2, 1},
  {18.75, NOTE_E3, 1},
  {19, NOTE_B2, 1},
  {19.25, NOTE_FS3, 1},
  {19.5, NOTE_E3, 1},
  {19.75, NOTE_GS3, 1},

  {28, NOTE_D3, 1},
  {28.25, NOTE_FS3, 1},
  {28.5, NOTE_D3, 1},
  {28.75, NOTE_FS3, 1},
  {29, NOTE_D3, 1},
  {29.25, NOTE_FS3, 1},
  {29.5, NOTE_D3, 1},
  {29.75, NOTE_FS3, 1},
  {30, NOTE_A2, 1},
  {30.25, NOTE_CS3, 1},
  {30.5, NOTE_A2, 1},
  {30.75, NOTE_CS3, 1},
  {31, NOTE_A2, 1},
  {31.25, NOTE_CS3, 1},
  {31.5, NOTE_A2, 1},
  {31.75, NOTE_CS3, 1},
  {32, NOTE_D3, 1},
  {32.25, NOTE_FS3, 1},
  {32.5, NOTE_D3, 1},
  {32.75, NOTE_FS3, 1},
  {33, NOTE_D3, 1},
  {33.25, NOTE_FS3, 1},
  {33.5, NOTE_D3, 1},
  {33.75, NOTE_FS3, 1},
  {34, NOTE_A2, 1},
  {34.25, NOTE_CS3, 1},
  {34.5, NOTE_GS2, 1},
  {34.75, NOTE_CS3, 1},
  {35, NOTE_FS2, 1},
  {35.25, NOTE_CS3, 1},
  {35.5, NOTE_E2, 1},
  {35.75, NOTE_CS3, 1},

  {36, NOTE_D3, 1},
  {36.25, NOTE_FS3, 1},
  {36.5, NOTE_D3, 1},
  {36.75, NOTE_FS3, 1},
  {37, NOTE_D3, 1},
  {37.25, NOTE_FS3, 1},
  {37.5, NOTE_D3, 1},
  {37.75, NOTE_FS3, 1},
  {38, NOTE_A2, 1},
  {38.25, NOTE_CS3, 1},
  {38.5, NOTE_GS2, 1},
  {38.75, NOTE_CS3, 1},
  {39, NOTE_FS2, 1},
  {39.25, NOTE_CS3, 1},
  {39.5, NOTE_FS2, 1},
  {39.75, NOTE_CS3, 1},
  {40, NOTE_F2, 1},
  {40.25, NOTE_C3, 1},
  {40.5, NOTE_F2, 1},
  {40.75, NOTE_C3, 1},
  {41, NOTE_F2, 1},
  {41.25, NOTE_C3, 1},
  {41.5, NOTE_F2, 1},
  {41.75, NOTE_C3, 1},
  {42, NOTE_E2, 1},
  {42.25, NOTE_B2, 1},
  {42.5, NOTE_E2, 1},
  {42.75, NOTE_B2, 1},
  {43, NOTE_E3, 1},
  {43.25, NOTE_D3, 1},
  {43.5, NOTE_CS3, 1},
  {43.75, NOTE_B2, 1},
};

int IndexFix(int pixel) { // Fixes pixel indexing cuz the original one is TRASH
  int i = pixel / 16;
  if (i % 2 == 0) pixel = (((i + 1) * 16) - pixel - 1) + (i * 16);
  return pixel;
}

void ChooseSong(int song) {
  for (int i = 0; i < SONG_LENGTH; i++) {
    if (song == 1) {
      current_song[i][0] = gusty[i][0];
      current_song[i][1] = gusty[i][1];
    }
    if (song == 2) {
      current_song[i][0] = wooded[i][0];
      current_song[i][1] = wooded[i][1];
    }
    if (song == 3) {
      current_song[i][0] = yoshi[i][0];
      current_song[i][1] = yoshi[i][1];
    }
  }
}

void DrawTiles(float song[MAX_LENGTH][2]) { // Draws the tiles on the screen. if it goes offscreen, it doesn't render the pixel
  for (int i = 0; i < SONG_LENGTH; i++) {
    int posy = 3 + (song[i][0] * 4);
    int posx = 64 * (song[i][1] - 1);

    float note_size;
    if (selected_song <= 2) note_size = 5;
    else note_size = 4;
    
    for (int j = 0; j < note_size; j++) {

      int rgb[3] = {0, 0, 0};
      if (song[i][1] == 1) rgb[0] = 10;
      if (song[i][1] == 2) rgb[2] = 10;
      if (song[i][1] == 3) rgb[1] = 10;
      if (song[i][1] == 4) rgb[0] = 10;
      if (song[i][1] == 4) rgb[1] = 10;

      if (j == note_size - 1) {
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
    NeoPixel.setPixelColor(IndexFix((i * 16) + 3), NeoPixel.Color(2, 0, 5));
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
  }
}

float comboMultiplier = 1;

void Hit() {
  score += 100 * comboMultiplier;
  notesPassed += 1;
  combo += 1;
  comboMultiplier *= 1.1;
  if (comboMultiplier > 2) comboMultiplier = 2;
}

void Miss() {
  notesPassed += 1;
  score -= 50;
  if (score < 0) score = 0;
  misses += 1;
  combo = 0;
  comboMultiplier = 1;
}

float menu_music = 10;
float song_length_left;

void MoveTiles() { // Moves the tiles in a 2d array downwards. I should make this take in a 2d array but idk how lol
  for (int i = 0; i < SONG_LENGTH; i++) {
    if (current_song[i][0] > -3) current_song[i][0] -= 0.25f;
    if (current_song[i][0] == -1) {
      Miss();
    }
    song_length_left -= 0.25f;
    if (song_length_left <= 0) {
      inMenu = true;
      menu_music = 0;
      NeoPixel.clear();
      score = 0;
      misses = 0;
      combo = 0;
      comboMultiplier = 0;
      ShowMenu();
    }
  }
}

bool button1Pressed = true;
bool button2Pressed = true;
bool button3Pressed = true;
bool button4Pressed = true;

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
      for (int i = 16; i <= 48; i += 16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }

    button1Pressed = true;
  } else button1Pressed = false;

  if (digitalRead(5) == HIGH) {
    if (!button2Pressed) {
      buttonPressed = 2;
      for (int i = 80; i <= 112; i += 16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }

    button2Pressed = true;
  } else button2Pressed = false;

  if (digitalRead(6) == HIGH) {
    if (!button3Pressed) {
      buttonPressed = 3;
      for (int i = 144; i <= 176; i += 16) { // Flashes the column that you pressed white
        NeoPixel.setPixelColor(IndexFix(i + 3), NeoPixel.Color(5, 5, 5));
      }
      CheckHit(buttonPressed);
    }

    button3Pressed = true;
  } else button3Pressed = false;

  if (digitalRead(7) == HIGH) {
    if (!button4Pressed) {
      buttonPressed = 4;
      for (int i = 208; i <= 240; i += 16) { // Flashes the column that you pressed white
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

void ShowMenu() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Choose song:");
  lcd.setCursor(0, 1);
  lcd.write((byte)0);
  lcd.setCursor(15, 1);
  lcd.write((byte)1);

  if (selected_song == 1) {
    lcd.setCursor(1, 1);
    lcd.print("Gusty Garden");
  }
  if (selected_song == 2) {
    lcd.setCursor(1, 1);
    lcd.print("Wooded Kingdom");
  }
  if (selected_song == 3) {
    lcd.setCursor(1, 1);
    lcd.print("Yoshi's Island");
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
  for (int i = 20; i <= 100; i += 16) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));

  for (int i = 211; i <= 213; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  for (int i = 194; i <= 198; i++) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));
  for (int i = 148; i <= 228; i += 16) NeoPixel.setPixelColor(IndexFix(i), NeoPixel.Color(1, 1, 0));

  /* --------------- */

  for (int i = 19; i <= 20; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));
  for (int i = 34; i <= 37; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));
  for (int i = 50; i <= 53; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));
  for (int i = 67; i <= 68; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(0, 0, 5));

  for (int i = 97; i <= 102; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 118; i <= 150; i += 16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 113; i <= 145; i += 16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 146; i <= 147; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  NeoPixel.setPixelColor(IndexFix(131 + 256), NeoPixel.Color(1, 1, 0));

  for (int i = 177; i <= 182; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 225; i <= 230; i++) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 193; i <= 209; i += 16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));
  for (int i = 198; i <= 214; i += 16) NeoPixel.setPixelColor(IndexFix(i + 256), NeoPixel.Color(1, 1, 0));

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

bool loop1 = false;

void MenuMusic() {
//  for (int i = 0; i < MENU_SONG_LENGTH; i++) {
//    if (abs(menu_song[i][0] - menu_music) < 0.05f) {
//      if (menu_song[i][2] == 0) melody.play(menu_song[i][1], 125);
//      if (menu_song[i][2] == 1) bass.play(menu_song[i][1], 125);
//    }
//  }
//
//  menu_music += 0.125f;
//  
//  if (menu_music > 19.95 && menu_music < 20.05 && !loop1) {
//    menu_music = 12;
//    loop1 = true;
//  }
//  if (menu_music > 19.95 && menu_music < 20.05 && loop1) menu_music = 28;
//  if (menu_music > 40) loop1 = false;
//  
//  if (menu_music >= 43.95) menu_music = 12;
}

bool button1PressedMenu = true;
bool button2PressedMenu = true;
bool button3PressedMenu = true;

void CheckMenuButtons() {
  if (digitalRead(4) == HIGH) {
    if (!button1PressedMenu) {
      if (selected_song > 1) selected_song -= 1;
      ShowMenu();
    }
    button1PressedMenu = true;
  } else button1PressedMenu = false;

  if (digitalRead(5) == HIGH) {
    if (!button2PressedMenu) {

      for (int i = 0; i < MAX_LENGTH; i++) {
        current_song[i][0] = 10000;
        current_song[i][1] = 1;
      }

      song_length_left = song_length[selected_song - 1];
      ChooseSong(selected_song);
      NeoPixel.clear();
      comboMultiplier = 1;
      inMenu = false;
    }
    button2PressedMenu = true;
  } else button2PressedMenu = false;

  if (digitalRead(6) == HIGH) {
    if (!button3PressedMenu) {
      if (selected_song < 3) selected_song += 1;
      ShowMenu();
    }
    button3PressedMenu = true;
  } else button3PressedMenu = false;
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
    delay(song_speed[selected_song - 1]);
  }
  else {
    MenuMusic();
    CheckMenuButtons();
    delay(90);
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

  melody.begin(8);
  bass.begin(9);

  lcd.createChar(0, left_arrow);
  lcd.createChar(1, right_arrow);
}
