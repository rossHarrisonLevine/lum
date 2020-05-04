//code created by Ross Levine and Mitchell McMillan
//last edit: 3/9/2020

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

const byte  ROWS = 4;
const byte  COLS = 4;
char        Message = "";



char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {5, 4, 3, 2}; 
byte colPins[COLS] = {9, 8, 7, 6}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int   STATE;
int   turns;
int   tpi = 60; //initialized to 60
bool  clearflag;
int   len = 48; //Change this if you have a different length of machine. Represents total string length in inches.
int   ratio = 4; //Change if you have a different gear ratio
int   twists;

//motor assignment
#define encoder1 4
#define encoder2 5

int counter1 = 0;
int State;
int LastState;

const int Motor1 = 12;
const int Motor1Speed = 3;
const int Motor1Brake = 9;

const int CW = HIGH;
const int CCW = LOW;


void setup()
{
  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  STATE = 1;
  clearflag = 1;
  
  //encoder
  pinMode (encoder1, INPUT);
  pinMode (encoder2, INPUT);
  
  //motor assignment
  pinMode(Motor1, OUTPUT);
  pinMode(Motor1Speed, OUTPUT);
  pinMode(Motor1Brake, OUTPUT);

  Serial.begin(9600);
  
  LastState = digitalRead(encoder1);
}
 
void loop()
{
    
  char customKey = customKeypad.getKey();
  
  if (customKey){
    Message = customKey; 
    }
  if (STATE == 1){
    
    twists = tpi * len * ratio;
    
    printer("A to Start", "B for Settings");
    
    if (Message =='A'){
      STATE = 2;
      clearflag = 1;
      Message = '.';
    }
    if (Message =='B'){
      STATE = 3;
      clearflag = 1;
      Message = '.';
      printer("Choose tightness","on scale of 1-6");
    }
  }

  if (STATE == 2) {
    printer("Winding @","");
    //TURN ON MOTOR 
    State = digitalRead(encoder1);
    if (State != LastState) {
        if (digitalRead(encoder2) != State) {
          counter1 ++;
        }
        else {
          counter1 --;
        }
        Serial.print("Position: ");
        Serial.println(counter1);
      
      }
      LastState = State;
    
      if (counter1 < twists) {
        brake('A', 0);
        moveMotor('A',CW,255);
      }
      else {
        brake('A', 1);
      }
    }

 
  if (STATE == 3) {
    switch (Message) {
      case '1':
        tpi = 20;
        clearflag = 1;
        printer("TPI set to 20","");
        clearflag = 1;
        delay(2000);
        STATE = 1;
        break;
      case '2':
        tpi = 40;
        clearflag = 1;
        printer("TPI set to 40","");
        clearflag = 1;
        delay(2000);
        STATE = 1;
        break;
      case '3':
        tpi = 60;
        clearflag = 1;
        printer("TPI set to 60","");
        clearflag = 1;
        delay(2000);
        STATE = 1;
        break;
      case '4':
        tpi = 80;
        clearflag = 1;
        printer("TPI set to 80","");
        clearflag = 1;
        delay(2000);
        STATE = 1;
        break;
      case '5':
        tpi = 100;
        clearflag = 1;
        printer("TPI set to 100","");
        clearflag = 1;
        delay(2000);
        STATE = 1;
        break;
      case '6':
        tpi = 120;
        clearflag = 1;
        printer("TPI set to 120","");
        clearflag = 1;
        delay(2000);
        STATE = 1;
        break;
    }
  }
}

void printer(char* a, char* b){
    if (clearflag == 1){
      lcd.clear();
      clearflag = 0;
    }
    lcd.setCursor(0,0);
    lcd.print(a);
    lcd.setCursor(0,1);
    lcd.print(b);
}

void  moveMotor(char motor, int dir, int speed) {
  
  int motorPin;
  int motorSpeedPin;

  if(motor == 'A') {
    motorPin = Motor1;
    motorSpeedPin = Motor1Speed;
  }
  digitalWrite(motorPin, dir);
  analogWrite(motorSpeedPin, speed);
}

void brake(char motor, int brk){ //1 to apply, 0 to release
  if(motor == 'A') {
    digitalWrite(Motor1Brake, brk);
    //delay(1000);
  }
  
}

/*
 * maybe come back to this when you want the user to enter their own TPI
  if (STATE == 4){
    if (customKey){
      i = 0;
      while (Data[i]){
        Data[i] = NULL;
      }
      if (fourflag == 0){ //this ensures that the LCD is cleared on first button press
        lcd.clear();
        fourflag = 1;
      }
    Data[data_count] = customKey; 
    lcd.setCursor(data_count,0); 
    lcd.print(Data[data_count]); 
    data_count++;
    
    if (customKey == '#' || data_count > 2){
      Data[data_count + 1] = NULL; //terminate Data stirng with a null
      printer("Hi", "");
      i = 0;
      while (Data[i]){
        lcd.setCursor(i,1);
        lcd.print(Data[i]);
        i++;
      }
      delay(2000);
      STATE = 1;
      }
    }
  }
  */
