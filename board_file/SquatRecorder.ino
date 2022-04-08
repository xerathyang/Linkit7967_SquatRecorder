#define LED_1_pin 4
#define LED_2_pin 5
#define AUDIO_pin 7

#include <LRemote.h>

LRemoteLabel Header;
LRemoteLabel SportLabel;
LRemoteLabel SportCount;
LRemoteLabel CaloriesLabel;
LRemoteLabel CaloriesCount;
LRemoteButton Resetbutton;
int counter = 0;
int calories = 0;

bool stat = false;
bool breaker = false;

String str;

void setup(void)  
{
  Serial.begin(9600);

  // init pin states
  pinMode(LED_1_pin, OUTPUT);
  digitalWrite(LED_1_pin,LOW);
  pinMode(LED_2_pin, OUTPUT);
  digitalWrite(LED_2_pin,LOW);


  LRemote.setName("SportRecorder");
  LRemote.setOrientation(RC_PORTRAIT);
  LRemote.setGrid(2,8);

//  Header.setText("SportRecorder");
//  Header.setPos(0,0);
//  Header.setSize(2,1);
//  Header.setColor(RC_BLUE);
//  LRemote.addControl(Header);

  SportLabel.setText("Squat Count");
  SportLabel.setPos(0,0);
  SportLabel.setSize(2,1);
  SportLabel.setColor(RC_ORANGE);
  LRemote.addControl(SportLabel);

  SportCount.setText(String(counter,10));
  SportCount.setPos(0,1);
  SportCount.setSize(2,2);
  SportCount.setColor(RC_GREY);
  LRemote.addControl(SportCount);

  CaloriesLabel.setText("Calories");
  CaloriesLabel.setPos(0,3);
  CaloriesLabel.setSize(2,1);
  CaloriesLabel.setColor(RC_ORANGE);
  LRemote.addControl(CaloriesLabel);

  CaloriesCount.setText(String(calories,10));
  CaloriesCount.setPos(0,4);
  CaloriesCount.setSize(2,2);
  CaloriesCount.setColor(RC_GREY);
  LRemote.addControl(CaloriesCount);

//  Resetbutton.setText("Reset");
//  Resetbutton.setPos(0,6);
//  Resetbutton.setSize(2,1);
//  Resetbutton.setColor(RC_BLUE);
//  LRemote.addControl(Resetbutton);

  LRemote.begin();
}

void loop(void)  
{
  int i;

  if(!LRemote.connected()) {
    //Serial.println("waiting for connection");
    delay(1000);
  } else {
    // The interval between button down/up
    // can be very short - e.g. a quick tap
    // on the screen.
    // We could lose some event if we
    // delay something like 100ms.
    delay(15);
  }
  LRemote.process();
  
  if (Serial.available()) {
    // 讀取傳入的字串直到"\n"結尾
    str = Serial.readStringUntil('\n');

    if (str.startsWith("a")) {
      str.remove(0,1);
      counter = str.toInt();
    }
    else if (str.startsWith("b")) {
      str.remove(0,1);
      calories = str.toInt();
    }
    else if (str == "command_1") {
      if(!stat){
        tone(AUDIO_pin, 110, 100);
        delay(100);
        tone(AUDIO_pin, 165, 100);
      }
      stat = true;
      digitalWrite(LED_1_pin,HIGH);
      digitalWrite(LED_2_pin,LOW);
    } 
    else if (str == "command_2") {
      digitalWrite(LED_1_pin,HIGH);
      digitalWrite(LED_2_pin,HIGH);
      if(!breaker){
        tone(AUDIO_pin, 200, 100);
        delay(100);
        tone(AUDIO_pin, 200, 100);
        breaker = true;
      }
    }
    else if(str == "command_4" ) {
      if(stat){
        tone(AUDIO_pin, 165, 100);
        delay(100);
        tone(AUDIO_pin, 110, 100);
      }
      stat = false;
      breaker = false;
      digitalWrite(LED_1_pin,LOW);  
      digitalWrite(LED_2_pin,LOW);
    }


    SportCount.updateText(String(counter, 10));
    CaloriesCount.updateText(String(calories, 10));
  }
}


