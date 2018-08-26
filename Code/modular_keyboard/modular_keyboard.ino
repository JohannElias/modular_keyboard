/* Modular Keyboard for people with limites fine-motor skills
  
  uses LiquidCrystal Library
  
  LCD-Display is for selecting characters, confirming or correcting
  modular design: Interface with Buttons, Joystick (big, robust or small, flilgran for moving by tongue)
                  or even use an eyetracker do give these commands...
                  Buttons are in bright color in our demo
  
  Interface-Connection:
  select next character  ("up")       (yellow butten = sun, above)        - Pin 10 with internal pull-up
  select prev character  ("down")     (blue butten = water, ground level) - Pin  9 with internal pull-up
  proceed to next input  ("right")    (green button = ok, proceed)        - Pin  8 with internal pull-up
  correct prev character ("backspace")(red button = false, go back)       - Pin  7 with internal pull-up

  future releases will include a heuristic matrix of the co-occurences of characters in words, to make typing faster...
  

  we use a standard 16x2 display first (compatible with the Hitachi HD44780 driver)
  
  The circuit
    LCD RS pin to digital pin 12
    LCD Enable pin to digital pin 11
    LCD D4 pin to digital pin 5
    LCD D5 pin to digital pin 4
    LCD D6 pin to digital pin 3
    LCD D7 pin to digital pin 2
    LCD D0-D3 5K resistor to ground
    LCD R/W pin to ground
    LCD VSS pin to ground
    LCD VCC pin to 5V
    contrast adjustment:

       10K potentiometer: ends to +5V and ground, middle to LCD VO pin (pin 3)
*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// define the interface connection for modular inputs:

const int up   = 10;
//cursor up = yellow (sun)
const int down = 9;
//curser down = blue (water = ground level)
const int right = 8;
//cursor left = green (correct/proceed = green)
const int left = 7;
//curser right = red (false, left = go back)


// define some variables/pointers for textposition, selected characters etc.
int pos_txt =0;
int pos_char=0;
int key     =0;


// define the virtual keybard - the list bof characters to choose from
// this will be updated on the spot in a later version running heuristics

char letter[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',' ','0','1','2','3','4','5','6','7','8','9'};

// max number of characters to choose from
const int max_letters = 36;


// initialize procedure
void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD - example for HaD Prize ;-)
  lcd.print("HaD Prize: HID");

  // set the interface-pins to Input and internal Pullup
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);
  
}


// main loop of the program:

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  
  /* a little trick to filter only single key-presses, especially with joystick
      filter out e.g. left and down -> just checking one condition after the other would result in
      both actions, we want only the single directional commands.

      So we calculate a number like in binary fashion from the inputs (key pressed make the bit zero!)
      and check only for the 4 conditions with one single zero (=key pressed)

      inside the switchcase we wait 100ms to make sure there is no signal bouncing any more (filter noise from switch)
      and then wait for the key to be released again. Otherwise the inc/dec of characters would be way to fast for humans ;-)
   */
  
  key=digitalRead(up)*8+digitalRead(down)*4+digitalRead(left)*2+digitalRead(right);
  
  switch (key) {
    case 7:
      //cursor up
      delay(100);
      while(digitalRead(up) == false){
         // wait for release of key
      };
      pos_char++;
      
      // check if we reached the last char in the virtual keyboard -> go to first char
      if (pos_char>max_letters) {
        pos_char=0;
      }
      break;
    case 11:
      //cursor down
      delay(100);
      while(digitalRead(down) == false){
         // wait for release of key
      };
      pos_char--;
      // check if we reached the first char in the virtual keyboard -> go to last char
      if (pos_char<0) {
        pos_char=max_letters;
      }
      break;
    case 14:
      //cursor right
      delay(100);
      while(digitalRead(right) == false){
         // wait for release of key
      };
            
      pos_txt++;

      // don't move beyond the last column in the display ;-)
      if (pos_txt>15) {
        pos_txt=15;
      }
      break;
    case 13:
      //cursor left
      delay(100);
      while(digitalRead(left) == false){
         // wait for release of key
      };
      lcd.setCursor(pos_txt, 1);
      lcd.print(' ');
      pos_txt--;

      // don't move beyond the first column in the display ;-)
      if (pos_txt<0) {
        pos_txt=0;
      }
      break;
  
    default:
      // if nothing else matches, do the default
      // default is optional
    break;
  }

  // print the character in row1 at the selected position
  lcd.setCursor(pos_txt, 1);
  lcd.print( letter[pos_char]);
}
 
