//Hanheld Speech Generator 01 Revision b (HSG-01b) "Reinier"

#include "TTS.h"
#include "LiquidCrystal.h"
#include "LedControl.h"

//Misc constants that can be changed as settings
#define PITCH               8       //Voice's pitch
#define BUTTON_DELAY        150     //Delay before the button gets read again. If it's set too low a single button press might get read as 2
#define LED_BRIGHTNESS      0       //Can be set up to 15, currently set to minimum
#define MIN_BLINK_INTERVAL  1000    //Sets the minimum and maximum time between Reinier's blinks (in ms)
#define MAX_BLINK_INTERVAL  10000
#define BLINK_DURATION      500     //Sets how long Reinier's eyes stay closed during blinking


//Pin mappings
//Buttons
#define UP    A0
#define DOWN  A1
#define LEFT  A2
#define RIGHT A3
#define PLAY  A4
#define BACK  A5
//LCD display
#define RS  7
#define EN  6
#define D4  5
#define D5  4
#define D6  3
#define D7  2
//Speaker
#define SPK 10
//LED matrix
#define DIN 13
#define CS  12
#define CLK 11


//The LCD, speaker and LED matrix libraries get initialized
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
TTS text2speech(SPK);
LedControl lc=LedControl(DIN,CLK,CS,1);

//Main menu
const byte NUM_MENU_OPTIONS = 3;
char menu_list[NUM_MENU_OPTIONS][16] = {" Preset menu",
                                         " Type menu",
                                         " Faces menu"};


//Declaration of presets, new presets can be added to the array
char blank[33] = "                                ";        //This one is used for the type menu, shouldn't be changed'

char presets[][33] = {"Hello World                     ",
                  "My name is Reinier              ",
                  "Today is a beautiful day        ",
                  "Nice to meet you                ",
                  "The electrons are my friends    ",
                  "I wish I was louder             ",
                  "Higher, higher                  ",
                  "I wonder how the world is like  ",
                  "Lame C skills made me defective ",
                  "You are my friend               ",
                  "Please, be careful with me      ",
                  "I dislike water                 ",
                  "I wish I was a super computer   ",
                  "Varta is my favourite battery   ",
                  "32K surely makes you feel tiny  ",
                  "My brother is an hourglass clock",
                  "Please, bear with my accent     "};


//These also need to be updated when adding new presets
const byte NUM_PRESETS = 17;          //Total number of presets
const byte NUM_PRESET_ROWS = 4;       //How many rows of presets are in the menu
char preset_list[NUM_PRESET_ROWS][16] = {" 00 01 02 03 04 ",
                                         " 05 06 07 08 09 ",
                                         " 10 11 12 13 14 ",
                                         " 15 16"};

//Declaration of faces
const byte IDLE[8] = {B00000000,
                      B01100110,
                      B01100110,
                      B01111110,
                      B11111111,
                      B10100101,
                      B10000001,
                      B01111110};

const byte BLINK[8] = {B00000000,
                       B01100110,
                       B01100110,
                       B01111110,
                       B11111111,
                       B10000001,
                       B10000001,
                       B01111110};

const byte DUMB[8] = {B00000000,
                      B01100110,
                      B01100110,
                      B01111110,
                      B11111111,
                      B10100001,
                      B10000101,
                      B01111110};

const byte SMILE[8] = {B01100110,
                       B01100110,
                       B01111110,
                       B11111111,
                       B10000001,
                       B10100101,
                       B10011001,
                       B01111110};

const byte AWAKE[8] = {B01100110,
                       B01100110,
                       B01111110,
                       B11111111,
                       B10100101,
                       B10000001,
                       B10011001,
                       B01111110};

const byte SEMIOPEN[8] = {B01100110,
                          B01111110,
                          B11111111,
                          B10100101,
                          B10000001,
                          B10011001,
                          B10011001,
                          B01111110};

const byte FULLYOPEN[8] = {B01100110,
                           B01111110,
                           B11111111,
                           B10100101,
                           B10011001,
                           B10011001,
                           B10011001,
                           B01111110};

const byte TEST[8] = {B11111111,
                      B11111111,
                      B11111111,
                      B11111111,
                      B11111111,
                      B11111111,
                      B11111111,
                      B11111111};

//These get used for the faces menu, works like the presets menu
const byte NUM_FACES = 8;
const char face_list[NUM_FACES][16] = {" Idle",
                                         " Blink",
                                         " Dumb",
                                         " Smile",
                                         " Awake",
                                         " Semi open mouth",
                                         " Open mouth",
                                         " Test pattern"};



//Other constants
const byte A_ASCII_VALUE = 97;
const byte Z_ASCII_VALUE = 122;
//byte menu_option = 0;         //These two keep the selected preset
//byte selected_preset = 0;



void setup() {
  //Initialize the button pins as INPUT_PULLUP and the speaker as output
  pinMode(SPK, OUTPUT);
  pinMode(UP, INPUT_PULLUP);
  pinMode(DOWN, INPUT_PULLUP);
  pinMode(LEFT, INPUT_PULLUP);
  pinMode(RIGHT, INPUT_PULLUP);
  pinMode(PLAY, INPUT_PULLUP);
  pinMode(BACK, INPUT_PULLUP);

  //Initializes the LED matrix
  lc.shutdown(0,false);
  lc.setIntensity(0,LED_BRIGHTNESS);
  lc.clearDisplay(0);

  //Set the speech of the voice
  text2speech.setPitch(PITCH);

  //Initializes the lcd
  lcd.begin(16, 2);

  //Print the welcome message and shows the awake face
  display(AWAKE);

  lcd.print("     HSG-1b");
  lcd.setCursor(0, 1);
  lcd.print("    \"Reinier\"");
  delay(1500);
  lcd.clear();

  //The face gets changed to idle
  display(IDLE);
  lcd.print("       by");
  lcd.setCursor(0, 1);
  lcd.print("     AIMMXI");
  delay(500);
  lcd.clear();
}


void loop() {
  byte selected = 0;

  //Everything has to be put inside a while loop or else BACK presses will misbehave
  while(true) {
    selected = top_down_menu(menu_list, NUM_MENU_OPTIONS);
    switch (selected) {
      case 0:
        preset_menu();
        break;
      case 1:
        type_menu(blank);
        break;
      case 2:
        face_menu();
    }
  }
}


/**
 * Jumps to the preset menu.
 */
void preset_menu() {
  //The seleccion arrow gets added at the beggining of the list and the position is kept
  preset_list[0][0] = '>';
  byte cursor_col = 0;
  byte cursor_row = 0;
  byte selected_preset = 0;

  //We clear the display and print the first two lines
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(preset_list[0]);
  lcd.setCursor(0,1);
  lcd.print(preset_list[1]);

  while (true) {
    switch(listen_input()) {
      case UP:
        //If the next preset exists on the range, update the menu
        if (selected_preset - 5 >= 0) {
            //The cursor gets updated
            preset_list[cursor_row][cursor_col] = ' ';    //The previous cursor gets deleted
            cursor_row--;
            preset_list[cursor_row][cursor_col] = '>';    //A cursor gets added on the previous row

            //The selected preset gets updated
            selected_preset -=5;
        }
        break;
      case DOWN:
        //If the next preset exists on the range, update the menu
        if (selected_preset + 5 < NUM_PRESETS) {
            //The cursor gets updated
            preset_list[cursor_row][cursor_col] = ' ';    //The previous cursor gets deleted
            cursor_row++;
            preset_list[cursor_row][cursor_col] = '>';    //A cursor gets added on the previous row

            //The selected preset gets updated
            selected_preset +=5;
        }
        break;
      case LEFT:
        //If the next preset exists on the range, update the menu
        if (cursor_col != 0) {
            //The cursor gets updated
            preset_list[cursor_row][cursor_col] = ' ';    //The previous cursor gets deleted
            cursor_col -= 3;
            preset_list[cursor_row][cursor_col] = '>';    //A cursor gets added on the previous row

            //The selected preset gets updated
            selected_preset -= 1;
        }
        break;
      case RIGHT:
        //If the next preset exists on the range, update the menu
        if (cursor_col != 12 && selected_preset < NUM_PRESETS - 1) {
            //The cursor gets updated
            preset_list[cursor_row][cursor_col] = ' ';    //The previous cursor gets deleted
            cursor_col += 3;
            preset_list[cursor_row][cursor_col] = '>';    //A cursor gets added on the previous row

            //The selected preset gets updated
            selected_preset += 1;
        }
        break;
      case PLAY:
        //The arrow gets cleared before entering the typing menu
        preset_list[cursor_row][cursor_col] = ' ';

        //If PLAY gets pressed, the selected preset gets displayed
        type_menu(presets[selected_preset]);
        break;
      case BACK:
        loop();
        break;
    }

    //The screen gets redrawn
    lcd.clear();
    lcd.print(preset_list[cursor_row]);

    //Checks if a new line of presets can be drawn or if there are no more to fill a second row
    if (cursor_row + 1 < NUM_PRESET_ROWS) {
      lcd.setCursor(0,1);
      lcd.print(preset_list[cursor_row + 1]);
    }
    delay(100);
  }
}


/**
 * Jumps to the interactive text editor.
 * @param preset Text that wants to be displayed on the text editor.
 */
void type_menu(char preset[]) {
  bool from_presets = true;
  byte selected_char = 0;
  if (preset == blank) {
   from_presets = false;
  }

  printovf(preset);
  lcd.blink();
  lcd.setCursor(0,0);

  while (true) {
    switch(listen_input()) {
      case UP:
        //Once you get to 'a', loop back to 'z'
        if (preset[selected_char] <= A_ASCII_VALUE || preset[selected_char] > Z_ASCII_VALUE) {
          preset[selected_char] = Z_ASCII_VALUE;
        } else {
          //The value of the character gets updated
          preset[selected_char]--;
        }
        break;
      case DOWN:
        //Once you get to 'z', loop back to 'a'
        if (preset[selected_char] >= Z_ASCII_VALUE || preset[selected_char] < A_ASCII_VALUE) {
          preset[selected_char] = A_ASCII_VALUE;
        } else {
          //The value of the character gets updated
          preset[selected_char]++;
        }
        break;
      case LEFT:
        //If LEFT is pressed at the beginning, loop to the end
        if (selected_char == 0) {
          selected_char = 31;
        } else {
          selected_char--;
        }
        break;
      case RIGHT:
        //If RIGHT is pressed at the end, loop to the beginning
        if (selected_char == 31) {
          selected_char = 0;
        } else {
          selected_char++;
        }
        break;
      case PLAY:
        speak(preset);
        break;
      case BACK:
        lcd.noBlink();
        if (from_presets) {
          preset_menu();
        } else {
          loop();
        }

        break;
    }
    //The display gets refreshed
    printovf(preset);
    lcd.setCursor(selected_char % 16,(int) selected_char / 16);
  }
}


/**
 * Jumps to the face selection menu.
 */
void face_menu() {
  byte selected;
  while (selected != BACK) {
    //A top down menu gets displayed and the selected option gets saved
    selected = top_down_menu(face_list, NUM_FACES);

    //Deppending on the selected option, a different menu gets drawn
    switch (selected) {
      case 0:
        display(IDLE);
        break;
      case 1:
        display(BLINK);
        break;
      case 2:
        display(DUMB);
        break;
      case 3:
        display(SMILE);
        break;
      case 4:
        display(AWAKE);
        break;
      case 5:
        display(SEMIOPEN);
        break;
      case 6:
        display(FULLYOPEN);
        break;
      case 7:
        display(TEST);
        break;
    }
  }
}

/**
 * Draws a menu that can be scrolled upside down.
 *
 * @param entries Array of characters with all the menu options
 * @param num_entries How many entries are in the array
 * @return The number of the selected element or BACK
 *
 */
byte top_down_menu(char entries[][16], byte num_entries) {
  //Keeps record of the selected menu option
  byte menu_option = 0;

  while (true) {
    //After the key has been pressed, the menu gets updated
    lcd.clear();
    //If the selected option is the last one, the selected item is on the second row instead of the first one
    if (menu_option >= num_entries - 1) {
      lcd.setCursor(0, 0);
      lcd.print(entries[menu_option - 1]);
      lcd.setCursor(0, 1);
      lcd.print(entries[menu_option]);
      lcd.setCursor(0, 1);   //The cursor gets put at the second row to draw the > symbol
    } else {
      lcd.setCursor(0, 0);
      lcd.print(entries[menu_option]);
      lcd.setCursor(0, 1);
      lcd.print(entries[menu_option + 1]);
      lcd.setCursor(0, 0);   //The cursor gets put at the first row to draw the > symbol
    }
    lcd.print(">");

    //Updates the arrow and the menu screen according to input
    switch(listen_input()) {
      case UP:
        if (menu_option <= 0) {
          menu_option = num_entries - 1;
        } else {
          menu_option--;
        }
        break;
      case DOWN:
        if (menu_option >= num_entries - 1) {
          menu_option = 0;
        } else {
          menu_option++;
        }
        break;
      //If PLAY gets presed, it returns the corresponding menu
      case PLAY:
        return menu_option;
      case BACK:
        return BACK;
    }
  }
}

/**
 * Listens for input and returns the button that was pressed. Also blinks the character after some time.
 *
 * @return The button pressed
 */
byte listen_input() {
  unsigned long next_blink = millis() + random(MIN_BLINK_INTERVAL,MAX_BLINK_INTERVAL);
  bool is_blinking = false;

  //Keeps checking until a button is pressed
  while (true) {
    //If a certain button is pressed, it returns the constant that identifies the button
    if (digitalRead(UP) == LOW) {
      delay(BUTTON_DELAY);
      return UP;
    } if (digitalRead(DOWN) == LOW) {
      delay(BUTTON_DELAY);
      return DOWN;
    } if (digitalRead(LEFT) == LOW) {
      delay(BUTTON_DELAY);
      return LEFT;
    } if (digitalRead(RIGHT) == LOW) {
      delay(BUTTON_DELAY);
      return RIGHT;
    } if (digitalRead(PLAY) == LOW) {
      delay(BUTTON_DELAY);
      return PLAY;
    } if (digitalRead(BACK) == LOW) {
      delay(BUTTON_DELAY);
      return BACK;
    }

    //If the blink time has reached the time, blink
    if (millis() > next_blink) {
      //If it's not blinking, blink and wait BLINK_DURATION ms
      if (is_blinking == false) {
        display(BLINK);
        is_blinking = true;
        next_blink = millis() + BLINK_DURATION;
      } else {
        //Stop blinking and wait for a random amount of time
        display(IDLE);
        is_blinking = false;
        next_blink = millis() + random(MIN_BLINK_INTERVAL,MAX_BLINK_INTERVAL);
      }
    }
  }
}



/**
 * Automatically scrolls to the next line on a 16x2 display when printing a string.
 * @param phrase[] Phrase to write
 */
void printovf(char phrase[]) {
  //Clear the display to avoid overlapping
  lcd.clear();
  Serial.print("iter");
  //Iterate over all letters in the array
  for(byte i = 0; i <= strlen(phrase); i++) {
    //If the end of the first line has been reached, jump to the beginning of the next line
    if (i == 16) {
      lcd.setCursor(0,1);
    }
    //Write the letter
    lcd.write(phrase[i]);
  }
}



/**
 * Spells the phrase and animates the character on the led matrix.
 *
 * @param phrase[] Phrase to speak
 */
void speak(char phrase[]) {
  display(SEMIOPEN);
  delay(100);
  display(FULLYOPEN);
  text2speech.sayText(phrase);
  display(IDLE);
}



/**
 * Displays the specified face on the LED matrix.
 *
 * @param face[] The face or array of bytes to be displayed
 */
void display(char face[]){
  //Iterates over every row of the array and prints it on the corresponding line.
  //Since the display is mounted upside down, the array is printed upside down as well
  for (byte i = 0; i < 8; i++) {
    lc.setRow(0,(i),face[i]);
  }
}
