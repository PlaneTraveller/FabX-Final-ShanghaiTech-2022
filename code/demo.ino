
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SH1106 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int conXPin = 1;
const int conYPin = 2;
const int conDownPin = 3;

int conX = 0;
int conY = 0;
int conDown = 0;

bool clicked = false;
bool toRight = false;
bool toLeft = false;

//============================================================
//== Setup ==//
void setup() {

  Serial.begin(9600);

  // initialize OLED display with address 0x3C for 128x64
  if (!oled.begin(SH1106_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SH1106 allocation failed"));
    while (true);
  }

  delay(2000);
  oled.clearDisplay();

  pinMode(conXPin, INPUT);
  pinMode(conYPin, INPUT);
  pinMode(conDownPin, INPUT);

}

oledTest();

//============================================================
//== Main Loop ==//
void loop() {
   //updateDisplay();

  // parse input data
  updateInputs();

  // update the current layer

}


//============================================================
//== Functions ==//
void updateDisplay() {


  oled.display();
}

void updateInputs() {
  conX = digitalRead(conXPin);
  conY = digitalRead(conYPin);
  conDown = digitalRead(conDownPin);
  if (conDown == HIGH) {
    clicked = true;
  } else {
    clicked = false;
  }

  // Need to fill in toRight and toLeft
}


//============================================================
//== Parts ==//
void oledTest() {

  oled.clearDisplay();

  oled.setTextSize(1);          // text size
  oled.setTextColor(WHITE);     // text color
  oled.setCursor(0, 10);        // position to display
  oled.println("Hello World!"); // text to display

  oled.fillRect(0, 15, 60, 40, WHITE);
}
