#include <Wire.h>
#include <string.h>
#include <Adafruit_SH1106.h>

#define OLED_RESET 4
Adafruit_SH1106 display(OLED_RESET);

short turn = 0;       // 
short score = 0;
short ran = 999;
bool event = false;
bool fin = false;   // event finished
short relen = 11;
short rn = 0;
short sc = 1;   // 1 -- main screen; 0 -- xx screen.

short cursoron = 0;
float p = 70;
int xxsel = 4;

short sklen = 11;
short a[4];

bool ini = true;

void setup()
{
  Serial.begin(9600);
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  pinMode(2,INPUT_PULLUP);
  
  randomSeed(999);

  for(int i=0;i<4;i++){
    a[i] = random(sklen);
    for(int j=0;j<i;j++){
      if(a[i]==a[j]){
        i--;
        break;
      }
    }
  }
  
}

void loop()
{ 
  int xValue = 0;   // A0
  int yValue = 0;   // A1
  int bValue = 0;   // 2; button

//  int xx[4]={1,2,3,4};
  String xxsim[5] = {"U","R","D","L","-"};
  float xxdp[5]={0.1,0,-0.1,0,0};
  
//  String re[11] = {"Place","Place","Place","Place","Place","Distroy","Distroy","Capture","Capture","Spawn","Disassemble"};       // random events
  int re[11] = {0,0,0,0,0,1,1,2,2,3,4};
  
  short pd[11] = {5, -10, -10, 10, -10, 20, -20, 5, 30, -30, -50};
  
  bValue = digitalRead(2);
  xValue = analogRead(A0);
  yValue = analogRead(A1);
  
//  check screen switch
  if (sc && xValue<50 && yValue<600 && yValue>400){
    sc = 1-sc;
    xxsel = 4;
    
    delay(200);
    return;
  }

  if (!sc && !bValue){
    sc = 1-sc;
    if(xxsel!=4){
      p += pd[a[xxsel]];
      p = min(100,p);
      p = max(0,p);
    }
    delay(200);
    return;
  }
  
// checkings on main screen
  if(sc){
  //  check the up/down (score)
    if (xValue>490 && yValue<50){        // up
      score++;
      score=min(99999,score);
    }
    else if (xValue>490 && yValue>800){  //down
      score--;
      score=max(0,score);
    }
  
  //  check the right (end the event)
    if (xValue>800 && yValue>500){
      fin = true;
    }
  
    //  check the button
    if(!bValue && !event){  
      turn = 1-turn;
      if(turn){
        //  generate the event
        ran = random(100);
        if (ran < p){
          event = true;
          rn = random(relen);
        }
      }
    }
  }else{
//  checkings on xx screen
      
    if (xValue>490 && xValue < 800 && yValue<50){        // up
      xxsel = 0;
    }
    else if (xValue>490 && yValue>800){  //down
      xxsel = 2;
    }
    else if (xValue>800 && yValue>500){  // right
      xxsel = 1;
    }
    else if (xValue<50 && yValue<600 && yValue>400){    //left
      xxsel = 3;
    }
    else if (xValue>900 && yValue<30){  // up right
      xxsel = 4;
    }
  }

  display.clearDisplay();


  
  if(sc){
  //  ----------------start the display [main screen]----------------
  //  score display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.print("SCORE:");
    display.println(score);
  

//    event prob display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(60,0);
    display.print("PROB:");
    display.println(float(p/100));
    
  //  turn display
    display.setTextSize(1.5);
    display.setTextColor(WHITE);
    display.setCursor(38,20);
  
    if(turn){
      display.println("YOUR TURN!");
    }
    else{
      display.println("  WAIT!  ");
    }
    
  
  //  event display
    if(event){
      if(fin){
        event = false;
        fin = false;
      }else {
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,40);
        display.println(re[rn]);
        }
    }
  }else{
//  ----------------start the display [xx screen]----------------
    
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("Select your xx!");

    //    event prob display
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(100,0);
    display.println(float(p/100));


//    up
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(55,15);
    display.println(a[0]);

//    right
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(85,35);
    display.println(a[1]);

//    down
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(50,55);
    display.println(a[2]);

//    left
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(15,35);
    display.println(a[3]);

//    top right
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(90,15);
    display.println("X");


//    cursor
    if(cursoron){
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(55,35);
      display.println(xxsim[xxsel]);      
    }
    cursoron = 1-cursoron;
  }


//  display display
  display.display();

//  monitor
  Serial.print(bValue);
  Serial.print("\t");
  Serial.print(xValue);
  Serial.print("\t");
  Serial.print(yValue);
  Serial.print("\t");
  Serial.println(ran);
  
  
  
  delay(100);
}
