#include <Wire.h> 
#include <SparkFun_MicroPressure.h>
#include <LiquidCrystal_I2C.h>

#include <SPI.h>

#define MAX6675_SO 2
#define MAX6675_CS 3
#define MAX6675_SCK 4

//SparkFun_MicroPressure mpr(EOC_PIN, RST_PIN, MIN_PSI, MAX_PSI);
SparkFun_MicroPressure mpr; // Use default values with reset and EOC pins unused

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);

int mmHG;
int pressure_opc = 0;

int releBomba = 13, releSolenoide = 12, 
    botonTrabajo = 5, botonMas = 6, botonMenos = 7,
    contadorGen = 0;

bool estadoTrabajo = true, bool estado_anteriorTrabajo = true ;
bool estadomas = true, bool estado_anteriormas = true ;
bool estadomenos = true, bool estado_anteriormenos = true ;

bool estado_solenoide = false;

int CURRENT_SENSOR = A0; // pin A0 de Arduino, donde va la salida del sensor
int sensorCorriente;
float corriente;

float temperatura;

void setup()
{    
    
    Serial.begin(9600);
    Wire.begin();
    
    pinMode(botonTrabajo, INPUT_PULLUP);
    pinMode(botonMas, INPUT_PULLUP);
    pinMode(botonMenos, INPUT_PULLUP);
    
    pinMode(releBomba, OUTPUT);
    pinMode(releSolenoide, OUTPUT);
    
    lcd.init(); 
    lcd.backlight();

    lcd.setCursor(0,0);
    lcd.print("   Bienvenidos  ");
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

    digitalWrite(releBomba, LOW);
    digitalWrite(releSolenoide, LOW);

    pinMode(CURRENT_SENSOR, INPUT);
}

void loop()
{

  mmHG = (int) mpr.readPressure(TORR);

  estadoTrabajo = digitalRead(botonTrabajo);
  contadorGen = funcBoton(contadorGen);

  if (contadorGen == 0 )
  {
    if (estado_solenoide == false)
    {
      lcd.setCursor(0,0);
      lcd.print("Select pressure");
      lcd.setCursor(0,1);
      lcd.print("Pressure: ");
      lcd.print(pressure_opc)
      delay(1000);
      // lcd.clear();

      estadomas = digitalRead(botonMas);
      pressure_opc = funcSumar(pressure_opc);

      pressure_opc = funcRestar(pressure_opc);
      estadomenos = digitalRead(botonMenos);
      }
      else 
      { 
        solenoideScreen();
        
        if (mmHG >= 0)
        {
          digitalWrite(releBomba, LOW);
          digitalWrite(releSolenoide, LOW);

          estado_solenoide = false;
        }
        
      }
  }
  else if(contadorGen == 1) 
  {

    if (mmHG != pressure_opc)
    {
      digitalWrite(releSolenoide, LOW);
      digitalWrite(releBomba, HIGH);
      
      delay(100);
      
      lcd.clear();
      bombaScreen();
    }
    else if (mmHG == pressure_opc)
    {
      digitalWrite(releBomba, LOW);
      digitalWrite(releSolenoide, LOW);

      delay(100);
      digitalWrite(releSolenoide, HIGH);

      lcd.clear();
      solenoideScreen();

      contadorGen = 0;
      estado_solenoide = true;
    } 
  
  }
}

int funcSumar(int contador)
{
  if (estadomas != estado_anteriormas) 
    {
       if (estadomas == LOW){ 
             contador = contador + 10;
             }
       estado_anteriormas = estadomas ; 
     }
     
     return contador;
}

int funcRestar(int contador)
{
  if (estadomenos != estado_anteriormenos) 
    {
       if (estadomenos == LOW){ 
             contador = contador - 10;
             }
       estado_anteriormenos = estadomenos ; 
     }
     
     return contador;
}

int funcBoton(int contador)
{
  if (estadoTrabajo != estado_anteriorTrabajo) 
    {
      if (estadoTrabajo == LOW)
      { 
        if (contador == 0)
        {
          contador = contador + 1;
        } 
        else
        {
          contador = 0;
        }
          
      }
      estado_anteriorTrabajo = estadoTrabajo ; 
    }
     
    return contador;
}

void bombaScreen()
{
  sensorCorriente = analogRead(CURRENT_SENSOR);
  corriente = fmap(sensorValue, 0, 1023, 0.0, 5.0);
  
  temperatura = leer_termopar();

  lcd.setCursor(0,1);
  lcd.print("I:");
  lcd.print(corriente, 2);
  lcd.print("mA");

  lcd.setCursor(8,1);
  lcd.print("T:");
  lcd.print(temperatura, 2);
  lcd.print("°C");

  lcd.setCursor(1,1);
  lcd.print("Pressure:");
  lcd.print(mmHG, 2);
  lcd.print(" torr");
}

void solenoideScreen()
{
  lcd.setCursor(0,1);
  lcd.print("Solenoid Active!");
  lcd.setCursor(1,1);
  lcd.print("Pressure:");
  lcd.print(mmHG, 2);
  lcd.print(" torr");
}

float fmap(float x, float in_min, float in_max, float out_min, float out_max)
{
   return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double leer_termopar()
{

    uint16_t v;
    pinMode(MAX6675_CS, OUTPUT);
    pinMode(MAX6675_SO, INPUT);
    pinMode(MAX6675_SCK, OUTPUT);

    digitalWrite(MAX6675_CS, LOW);
    delay(1);

    // Read in 16 bits,
    //  15    = 0 always
    //  14..2 = 0.25 degree counts MSB First
    //  2     = 1 if thermocouple is open circuit
    //  1..0  = uninteresting status

    v = shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);
    v <<= 8;
    v |= shiftIn(MAX6675_SO, MAX6675_SCK, MSBFIRST);

    digitalWrite(MAX6675_CS, HIGH);
    if (v & 0x4)
    {
        // Bit 2 indicates if the thermocouple is disconnected
        return NAN;
    }

    // The lower three bits (0,1,2) are discarded status bits
    v >>= 3;

    // The remaining bits are the number of 0.25 degree (C) counts
    return v * 0.25;
}