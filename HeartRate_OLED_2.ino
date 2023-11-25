/*ESQUEMA DE CONEXIONES ARDUINO
MAX30102
GND
5V 
SDA Y SCL (A5 Y A4, Ó DEPSUES DE AREF )
PANTALLA LOS MISMOS

esp32 SCL-22 SDA-21*/


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <LiquidCrystal_I2C.h>
byte a[8] = {
B00000,			
B00000,			
B00000,			
B00111,			
B00111,			
B01111,			
B01111,			
B01111			
};

byte b[8] = {
B00000,			
B11100,			
B11110,			
B11111,			
B11111,			
B11111,			
B11111,			
B11111			
};

byte c[8] = {
B00000,			
B00111,			
B01111,			
B11111,			
B11111,			
B11111,			
B11111,			
B11111			
};


byte d[8] = {
B00000,			
B00000,			
B00000,			
B11100,			
B11100,			
B11110,			
B11110,			
B11110			  
/////////////
};


byte e[8] = {
B00111,			
B00011,			
B00001,			
B00000,			
B00000,			
B00000,			
B00000,			
B00000			
};

byte f[8] = {
B11111,			
B11111,			
B11111,			
B11111,			
B01111,			
B00111,			
B00011,			
B00000			
};

byte g[8] = {
B11111,				
B11111,				
B11111,				
B11111,				
B11110,				
B11100,				
B11000,				
B00000				
};


byte h[8] = {
B11100,				
B11000,				
B00000,				
B00000,				
B00000,				
B00000,				
B00000,				
B00000				
};

LiquidCrystal_I2C lcd(0x27,16,2);
int counter = 0;
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good. Numero de muestras que toma para calcular el latido por minuto promedio. 
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup()
{

  Serial.begin(115200);     // iniciar la interface serial a 115200 de velocidad, por lo que si desea ver los datos de ritmo cardiaco en el motinor serie del software de arduino, debe configurar esta misma velocidad de el monitor serie
   //LCD
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, a);
  lcd.createChar(1, b);
  lcd.createChar(2, c);
  lcd.createChar(3, d);
  lcd.createChar(4, e);
  lcd.createChar(5, f);
  lcd.createChar(6, g);
  lcd.createChar(7, h);
  // Clear the buffer
  lcd.clear();
  lcd.print("Initializing...");
  delay(500);

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)){ //Use default I2C port, 400kHz speed
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop(){
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)        // Estructura if(){} que detecta un latido 
  {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0);
    if (beatsPerMinute < 255 && beatsPerMinute > 20)      // estructura if(){} que promedia los latidos segun el RATE_SIZE para obtener medidas mas estables
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable
      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
      
       
      corazonP();          // 4 instrucicones para ilustrar el valor correspodiente a los latidos del corazon promedio de las ultimas 4 muestras                    corazonP();
     // delay(500);
      lcd.clear();
      lcd.setCursor(5,6);             
      lcd.print(beatAvg);
      delay(400);
      lcd.clear();
      corazonP();
    //tone(12,2000,20);                 // (buzzer)emite un sonido en el pin 12 de la plca nano, con 2000 hz de frecuencia, y 20 milisegundos de duracion 
    }
      corazonP();
  }
// impresion o trasnmision de los paramentros de intensidad de luz infraroja, latidos por minuto, promedio de latidos por minuto al monitor serie
  Serial.print("IR=");
  Serial.print(irValue);
  Serial.print(", BPM=");
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM=");
  Serial.print(beatAvg);

// estructura if que determina si no hay un dedo colocado en el sensor
  if (irValue < 50000){
    Serial.print(" No finger?");
      lcd.clear();        
      lcd.setCursor(0,0);             // Start at top-left corner
      lcd.print("Ubique su dedo");
  }
  Serial.println();
}

  void corazonP(){
  lcd.setCursor(0, 0);
  lcd.write(byte(0));

  lcd.setCursor(1, 0);
  lcd.write(byte(1));

  lcd.setCursor(2, 0);
  lcd.write(byte(2));


  lcd.setCursor(3, 0);
  lcd.write(byte(3));

///
  lcd.setCursor(0, 1);
  lcd.write(byte(4));

  lcd.setCursor(1, 1);
  lcd.write(byte(5));

  lcd.setCursor(2, 1);
  lcd.write(byte(6));


  lcd.setCursor(3, 1);
  lcd.write(byte(7));

  }





