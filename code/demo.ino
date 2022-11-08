
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
//== Classes ==//
class Module {
  // Basic Module class. Has a function OnClick().
  // TODO: display properties (location, content, textsize, etc)
  private:

  public:
  string Name;
  void OnClick();
  void Update();

};


class Layer {
  // Basic Layer class. Handles addition of modules, changing the cursor, passing clicking events.
  // TODO: draw all the modules onto the screen, cursor indicator
  private:


  public:
  Modules modules;

  void AddModule(Module module) {
    this->modules.AddModule(module);
  }

  void Update() {
    if (toRight | toLeft) {
      this->modules.Next(toRight);
    }
    if (clicked) {
      this->modules.ClickModule();
    }

    modules.Update();
  }
};



class Modules {
  // Helper class for Modules objects.
  private:
  public:
  Module modules[20];
  int size = 0;
  int currentModule = 0;

  void AddModule(Module module) {
    this->modules[size] = module;
    size++;
  }

  void ClickModule() {
    this->modules[currentModule].OnClick();
  }

  void Next(bool direction) {
    // direction is + for going right.
    if (direction) {
      if (currentModule < size - 1) {
        currentModule++;
      } else {
        currentModule = 0;
      }
    } else {
      if (currentModule > 0 ) {
        currentModule--;
      } else {
        currentModule = size - 1;
      }
    }
  }

  void Update() {
    for (int i = 0; i++; i<size) {
      this->modules[i].Update();
    }
  }
};


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

Layer defaultLayer;
Layer currentLayer = &defaultLayer;

//============================================================
//== Main Loop ==//
void loop() {
  //updateDisplay();

  // parse input data
  updateInputs();

  // update the current layer
  currentLayer.Update();

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
