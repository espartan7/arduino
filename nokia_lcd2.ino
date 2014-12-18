/*********************************************************************
This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

These displays use SPI to communicate, 4 or 5 pins are required to
interface

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

/********************************************************************
Modified by Siew Wai Hung - February 2013
http://waihung.net
Display bitmap images on PCD8544 GLCD
Bitmap images are 80 x 45 in size
MSB to LSB Left to Right

Thanks to Adafruit's Library and 
Image2GLCD.exe by Jirawat Kongkaen http://avrportal.com
********************************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <MemoryFree.h>

// Please use a level shifter (ex. 74HC4050) if you're using a 5V Arduino
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)

Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 3, 4);

int potPin = 5;    // select the input pin for the potentiometer
int ledPin = 13;   // select the pin for the LED
int val = 0;       // variable to store the value coming from the sensor


const unsigned char PROGMEM idun [] = {
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x7c,0x0,0x0,0x1f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x7e,0x0,0x0,0x1f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x7e,0x0,0x0,0x1f,0x80,0x1f,0xe0,0x0,0x0,0x0,0x0,
0x7e,0x0,0x0,0x1f,0x80,0x7f,0xfc,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x1f,0x81,0xff,0xff,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x1f,0x83,0xff,0xff,0x80,0x0,0x0,0x0,
0x78,0x0,0x7c,0x1f,0x87,0xff,0xff,0xc0,0x3,0xf0,0x0,
0x7c,0x1,0xff,0x1f,0x88,0x1f,0xf8,0x20,0x1f,0xfc,0x0,
0x7e,0x3,0xff,0x9f,0x98,0x1f,0xf0,0x30,0x7f,0xff,0x0,
0x7e,0x7,0xff,0xff,0x98,0x1f,0xf0,0x30,0xff,0xff,0x80,
0x7e,0xf,0xff,0xff,0xb8,0x1f,0xf0,0x39,0xff,0xff,0xc0,
0x7e,0x1f,0xf1,0xff,0xb8,0x1f,0xf0,0x39,0xfc,0x1f,0xc0,
0x7e,0x1f,0xc0,0x7f,0xb8,0x1f,0xf0,0x39,0xf8,0xf,0xe0,
0x7e,0x1f,0x80,0x7f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x80,0x3f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x1f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0xf8,0x1f,0xf0,0x3f,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0xb8,0x1f,0xf0,0x3b,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0xb8,0x1f,0xf0,0x3b,0xf0,0x7,0xe0,
0x7e,0x3f,0x0,0x3f,0x38,0x1f,0xe0,0x33,0xf0,0x7,0xe0,
0x7e,0x1f,0x80,0x3f,0x18,0x1f,0xf0,0x33,0xf0,0x7,0xe0,
0x7e,0x1f,0xc0,0x7f,0x8,0xf,0xe0,0x63,0xf0,0x7,0xe0,
0x7e,0x1f,0xe1,0xfe,0xc,0x7,0x80,0x63,0xf0,0x7,0xe0,
0x7e,0xf,0xff,0xfe,0x4,0x0,0x0,0xc3,0xf0,0x7,0xe0,
0x7e,0xf,0xff,0xfc,0x2,0x0,0x0,0x83,0xf0,0x7,0xe0,
0x7e,0x7,0xff,0xf8,0x1,0x0,0x1,0x3,0xf0,0x7,0xe0,
0x7e,0x1,0xff,0xe0,0x0,0xc0,0x6,0x3,0xf0,0x3,0xe0,
0x0,0x0,0x3f,0x0,0x0,0x3f,0xf8,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0  
};

int led = 9;
char recurso[18];

void setup()   {
  pinMode(ledPin, OUTPUT);  // declare the ledPin as an OUTPUT
  Serial.begin(9600);
  display.begin();

  pinMode(2, INPUT);
  for(int i=0; i<=00; i++){
    display.setContrast(i);
    //imprimirTexto(i);
    analogWrite(led,i*2);
    delay(5);
  }
  display.setContrast(50);
  display.clearDisplay();
  //screensplash();
  analogWrite(led,200);  
  digitalWrite(2,LOW);
  
}


void loop() {
  int r=random(10,50);
  Serial.readBytes(recurso, 18);
  //record("Grabando\naudio y video\n", r, recurso);
  val = analogRead(potPin);    // read the value from the sensor

    analogWrite(led,val/6);

  digitalWrite(ledPin, HIGH);  // turn the ledPin on
  delay(val/10);                  // stop the program for some time
  digitalWrite(ledPin, LOW);   // turn the ledPin off
  delay(val/20);                  // stop the program for some time
  record("Grabando\naudio y video\n", r, recurso);
}

void screensplash(){
  display.clearDisplay();
  display.drawBitmap(0, 0, idun, 84, 48, BLACK);
  display.display();
  for(int i=0; i<200; i++){
    analogWrite(led,i);
    delay(5);
  }
  delay(2000);
  display.clearDisplay();
}


void record(String a, int r, char ram[]){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  int i=0;
  String msg = String("[");
  msg += (r*2);
  msg += "%]\n";
  msg += a;
  String points = String(".");
  for(i=1; i<=(r/4); i++){
        msg += points;
  }
  msg += "\n";
  msg = msg + ram;
  display.println(msg);
  display.display();
}

void imprimirTexto(int a){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.println(a);
  display.display();
}
