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

float mmHG = 0.0;
int pressure_opc = 0;

int releBomba = 13, releSolenoide = 12, 
    botonTrabajo = 5, botonMas = 6, botonMenos = 7,
    contadorGen = 0;

bool estadoTrabajo = true, bool estado_anteriorTrabajo = true ;
bool estadomas = true, bool estado_anteriormas = true ;
bool estadomenos = true, bool estado_anteriormenos = true ;

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
}

void loop()
{

    estadoTrabajo = digitalRead(botonTrabajo);
    contadorGen = funcBoton(contadorGen);

  if (contadorGen == 0)
  {
    lcd.setCursor(0,0);
    lcd.print("Select pressure");
    lcd.setCursor(0,1);
    lcd.print("Pressure: ");
    lcd.print(pressure_opc)
    delay(1000);
    // lcd.clear();

    estadomas = digitalRead(botonMas);
    estadomenos = digitalRead(botonMenos);

    pressure_opc = funcSumar(pressure_opc);
    pressure_opc = funcRestar(pressure_opc);

  }
  else if(contadorGen == 1) {
   
  
  }
}

int funcSumar(int contador)
{
  if (estadomas != estado_anteriormas) 
    {
       if (estadomas == LOW){ 
             contador = contador + 1;
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
             contador = contador - 1;
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

void firstScreen()
  {
    lcd.setCursor(0,1);
    lcd.print("I:");
    lcd.print("");
    lcd.setCursor(8,1);
    lcd.print("V:");
    lcd.print("");
  }