////////// LIBRARIES //////////
#include <Keypad.h>
#include <LiquidCrystal.h>

////////// LAMP SETUP //////////
# define RELAY 46
volatile byte lamp = LOW;

////////// MOTION SETUP //////////
# define RCWL 5
# define LED 44
volatile byte motion = LOW;

////////// TONE SETUP //////////
# define LM386 8
volatile byte sound = LOW;  |

////////// KEYPAD SETUP //////////
// Keypad dimensions
const byte ROWS = 4;
const byte COLS = 4;
// Keypad rows and columns pin wiring
byte rowPins[ROWS] = {37, 35, 33, 31};
byte colPins[COLS] = {29, 27, 25, 23};
// Keys initialization
char hexaKeys[ROWS][COLS] = {
  {'1', '4', '7', '0'},
  {'2', '5', '8', '*'},
  {'3', '6', '9', '#'},
  {'A', 'B', 'C', 'D'}
};
// Keypad initialization
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

////////// LCD SETUP //////////
#define LCD_COLS 16 
#define LCD_ROWS 2
const int rs = 32, en = 30, d4 = 28, d5 = 26, d6 = 24, d7 = 22;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

////////// STIMULATON PARAMETERS //////////
int toneFrequency = 0;
unsigned int toneTime = 0;
int stimulationTime = 0;
int movementAnalysisTime = 0;
int intervalTime = 0;
int numberOfEvents = 0;

int maxCharValue = 0;
String parameterMsg;
String parameterUnit;

////////// FUNCTIONS //////////
void HomeLCD(){
  lcd.setCursor(0, 0);
  lcd.print("Conditioning");
  lcd.setCursor(0, 1);
  lcd.print("Chamber");
}

char PressedKey(){
  char customKey = customKeypad.getKey();
  if (customKey){
    return customKey;
  }
}

void ReadKeys(){
  char customKey = customKeypad.getKey();
  if (customKey == 'A'){
    // Tone Frequency
    parameterMsg = "Tone Frequency";
    parameterUnit = "Hz";
    maxCharValue = 4;
    toneFrequency = SetParameter(maxCharValue, parameterUnit, parameterMsg);

    // Tone Duration
    parameterMsg = "Tone Duration";
    parameterUnit = "s";
    maxCharValue = 4;
    toneTime = SetParameter(maxCharValue, parameterUnit, parameterMsg);
  }
  if (customKey == 'B'){
    // Stimulation Duration
    parameterMsg = "Stim. Duration";
    parameterUnit = "s";
    maxCharValue = 4;
    stimulationTime = SetParameter(maxCharValue, parameterUnit, parameterMsg);
  }
  if (customKey == 'C'){
    // Movement Analysis Duration
    parameterMsg = "Mov. Analysis";
    parameterUnit = "s";
    maxCharValue = 4;
    movementAnalysisTime = SetParameter(maxCharValue, parameterUnit, parameterMsg);

    // Wait Time Between Events
    parameterMsg = "Waiting Time";
    parameterUnit = "s";
    maxCharValue = 4;
    intervalTime = SetParameter(maxCharValue, parameterUnit, parameterMsg);

    // Number of events
    parameterMsg = "Num. of Events";
    parameterUnit = "events";
    maxCharValue = 2;
    numberOfEvents = SetParameter(maxCharValue, parameterUnit, parameterMsg);
  }
  if(customKey == '1'){
    RelayActivation();
  }
  if(customKey == '2'){
    ToneTest();
  }
  if(customKey == '3'){
    
  }
  if(customKey == 'D'){
    Experiment();
  }

}

int SetParameter(int maxCharValue, String parameterUnit, String parameterMsg){
    char parameter[maxCharValue];
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(parameterMsg);
    int cursorUnitLoc =  maxCharValue + 1;
    lcd.setCursor(cursorUnitLoc, 1);
    lcd.print(parameterUnit);

    bool valid = false;
    int i = 0;
    while( i < maxCharValue){
      lcd.setCursor(i, 1);
      {
        char newKey = customKeypad.getKey();
        if(newKey){
          if(newKey == 'A'|| newKey == 'B'|| newKey == 'C' || newKey == 'D' || newKey == '#'){
            i = maxCharValue;
            valid = false;
            lcd.clear();
          }
          else{
            if(newKey != '*')
            {
              parameter[i] = newKey;
              lcd.print(newKey);
              i++;
              valid = true;
            }
            else{
              if(i > 0){
                valid = true;
                i = maxCharValue;
                lcd.clear();
              }
            }
          }
        }
      }
    }

    if(valid == true){
      delay(2000);
      ConfirmParameter();
      String parameterStr = String(parameter);
      return parameterStr.toInt();
    } 
}

void ConfirmParameter(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Settled");
    lcd.setCursor(0, 1);
    lcd.print("Value");
    delay(2000);
    lcd.clear();
}

void RelayActivation(){
    lamp = !lamp;
    if(lamp){
        digitalWrite(RELAY, HIGH);
    }
    else {
        digitalWrite(RELAY, LOW);
    }
}

void ToneActivation(){
    sound = !sound;
    if(sound){
        tone(LM386, toneFrequency, toneTime);
    }
    else {
        noTone(LM386);
    }
}

void MotionDetection(){
  startTime = millis();
  while((elapsedTime - startTime < (movementAnalysisTime*)) )
  int val = digitalRead(RCWL);
  if (val == HIGH) {
    if (motion == false) {
      motion = true;
      digitalWrite(LED, HIGH);
    }
  }
  else {
    if (motion == true) {
      motion = false;
      digitalWrite(LED, LOW);
    }
  }
}

void Experiment(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EXPERIMENT");
  delay(2000);
  lcd.clear();
}

void ToneTest(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("TONE TEST");
  toneFrequency = 4000;
  toneTime = 10000000;
  ToneActivation();
  delay(2000);
  lcd.clear();
}

void setup(){
  // Serial communication
  Serial.begin(9600);

  // PIR HC-SR501 motion sensor
  pinMode(PIR, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // SRD-05VDC-SL-C relay module
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, HIGH);
  
  // 2x16 LCD Display
  lcd.begin(LCD_COLS, LCD_ROWS);
  
  pinMode(5, INPUT);
  pinMode(4,OUTPUT);
  
}
  
void loop(){

  HomeLCD();
  ReadKeys();
  MotionDetection();
  while(digitalRead(5)==1){
    digitalWrite(4, HIGH);
  }
  digitalWrite(4, LOW);

}
