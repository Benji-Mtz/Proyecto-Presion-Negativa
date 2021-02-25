#include <Wire.h> 

#include <SparkFun_MicroPressure.h>
#include <LiquidCrystal_I2C.h>

/*
 * Initialize Constructor
 * Optional parameters:
 *  - EOC_PIN: End Of Conversion (defualt: -1)
 *  - RST_PIN: Reset (defualt: -1)
 *  - MIN_PSI: Minimum Pressure (default: 0 PSI)
 *  - MAX_PSI: Maximum Pressure (default: 25 PSI)
 */
//SparkFun_MicroPressure mpr(EOC_PIN, RST_PIN, MIN_PSI, MAX_PSI);
SparkFun_MicroPressure mpr; // Use default values with reset and EOC pins unused

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);
 
int WhichScreen =1;   // This variable stores the current Screen number
boolean hasChanged = true;
const int buttonPin = 4;    // the number of the pushbutton pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

float mmHG = 0.0;

void setup()
{    
    
    Serial.begin(9600);
    Wire.begin();
    
    int reverse = 3;
    pinMode(buttonPin, INPUT);
    
    lcd.init(); 
    lcd.backlight();

    lcd.setCursor(0,0);
    lcd.print("   Bienvenidos   ");
    lcd.setCursor(0,1);
    // lcd.print("                ");
    lcd.print("   Al Sistema   ");
    delay(1000);
    lcd.clear();

    if(!mpr.begin())
    {
      Serial.println("Cannot connect to MicroPressure sensor.");
      lcd.setCursor(0,0);
      lcd.print("Cannot connect");
      lcd.setCursor(0,1);
      // lcd.print("                ");
      lcd.print("to Pressure Sen");
      delay(5000);
      lcd.clear();
      while(1);
    }

    for (int i = reverse; i >= 1; i--)
    {
        lcd.setCursor(0,0);
        lcd.print("                ");
        lcd.setCursor(6,1);
        lcd.print(i);
        lcd.print("...");
        delay(500);
    }
    lcd.clear();
}
void loop()
{

  mmHG = (mpr.readPressure(INHG),4);
  Serial.println(" inHg");
 
  if (hasChanged == true) {
   
  switch(WhichScreen) {
    case 1:
    {
      firstScreen();
    }
      break;
   
    case 2:
      {
        secondScreen();
      }
      break;
   
    case 3:
      {
        thirdScreen();
      }
      break;
   
    case 4:
      {
        fourthScreen();
      }
      break;
   
    case 5:
      {
        fifthScreen();
      }
      break;
   
    case 6:
      {
        sixthScreen();
      }
      break;
    case 0:
      {
       
      }
      break;
    }
}
 
    //-------------------------------
    // BEGIN of the switch debouncing code
    int reading = digitalRead(buttonPin);
    
    Serial.print("reading: ");
    Serial.println(reading);

  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }
 
if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
 
    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;
 
      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        hasChanged = true;
        WhichScreen++;
      }
    } else {
      hasChanged = false;
    }
  }
  lastButtonState = reading;
  // END of the switch Debouncing code
  // --------------------------------------
  if (WhichScreen > 6){
    WhichScreen = 1;
  }
}
 
void firstScreen()
  {
   lcd.clear();
   lcd.setCursor(0,0); // Column, line
   lcd.print("Screen number 1");
   lcd.setCursor(0,1);
   lcd.print("On a menu system");
  }
void secondScreen()
  {
    lcd.clear();
    lcd.setCursor(0,0); // Column, line
    lcd.print("Screen number 2");
    lcd.setCursor(0,1);
    lcd.print("of my menu !!");
  }
void thirdScreen()
  {
    lcd.clear();
    lcd.setCursor(0,0); // Column, line
    lcd.print("Screen number 3");
    lcd.setCursor(0,1);
    lcd.print("Third screen (3)");
  }
void fourthScreen()
  {
    lcd.clear();
    lcd.setCursor(0,0); // Column, line
    lcd.print("Screen number 4");
    lcd.setCursor(0,1);
    lcd.print("Just press btn");
  }
void fifthScreen()
  {
    lcd.clear();
    lcd.setCursor(0,0); // Column, line
    lcd.print("Screen number 5");
    lcd.setCursor(0,1);
    lcd.print("i2C LCD screen");
  }
void sixthScreen()
  {
    lcd.clear();
    lcd.setCursor(0,0); // Column, line
    lcd.print("Screen number 6");
    lcd.setCursor(0,1);
    lcd.print("  Sixth and last");
  }