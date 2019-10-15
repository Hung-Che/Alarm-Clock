
#include <IRremote.h>
#include <IRremoteInt.h>
#include <LiquidCrystal.h> 
#include "RTClib.h"
/**
 * Go to boarddefs.h 
 * // Arduino Duemilanove, Diecimila, LilyPad, Mini, Fio, Nano, etc
 * #else
 * #define IR_USE_TIMER1    tx = pin 9
 * //#define IR_USE_TIMER2    // tx = pin 3
 */
#include <stdlib.h>
RTC_DS1307 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int buzzerPin = 10;
const int snoozePin = 13;
int RECV_PIN = 9;
IRrecv irrecv(RECV_PIN);
decode_results results;
long num[10] = {0xFF6897, 0xFF30CF, 0xFF18E7, 0xFF7A85, 0xFF10EF, 0xFF38C7, 0xFF5AA5, 0xFF42BD, 0xFF4AB5, 0xFF52AD};
int a = 0;
int b = 0;
int c = 0;
int d = 0;
int setAlarm = 0;


void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  irrecv.enableIRIn();
  if(rtc.begin()&&rtc.isrunning()){
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    if(now.hour()<10)lcd.print("0");
    lcd.print(now.hour());
    lcd.print(':');
    if(now.minute()<10)lcd.print("0");
    lcd.print(now.minute());
    lcd.print(':');
    if(now.second()<10)lcd.print("0");
    lcd.print(now.second());
    lcd.print("       ");
    getAlarm();
}
void getAlarm(){
  int ques1 = rand()%100+1;
  int ques2 = rand()%100+1; 
  int ans = ques1 + ques2;
  if (irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    irrecv.resume();
    if(results.value==0xFFB04F){
        setAlarm=1;
    }else if(results.value==0xFFFFFFFF){
      setAlarm = setAlarm;  
    }
  }
  if(digitalRead(buzzerPin)==HIGH){
    setAlarm=5;  
  }
  while(setAlarm==1){
    lcd.setCursor(0,0);
    lcd.print("Set Hour");
    lcd.setCursor(0,1);
    if(irrecv.decode(&results)){
      for(int i = 0; i<10; i++){
        if(results.value==num[i]){
          a = a*10+i;
        }else if(results.value==0xFFFFFFFF){
          a = a;  
        }
        irrecv.resume();
      }
      if(a<24){
        lcd.print(a);
        if(results.value==0xFFB04F){
          setAlarm=2;
        }
      }else{
        lcd.clear();
        setAlarm = 1;
        a=0;
      }
    }
  }
  while(setAlarm==2){
    lcd.setCursor(0,0);
    lcd.print("Set Minute");
    lcd.setCursor(0,1);
    if(irrecv.decode(&results)){
      for(int i = 0; i<10; i++){
        if(results.value==num[i]){
          b = b*10+i;
        }else if(results.value==0xFFFFFFFF){
          b = b;  
        }else if(results.value==0xFFB04F){
          setAlarm++;
        }
        irrecv.resume();
      }
      lcd.print(a);
      lcd.print(':');
      if(b<60){
        lcd.print(b);
        if(results.value==0xFFB04F){
          setAlarm=3;
        }
      }else{
        lcd.clear();
        setAlarm = 2;
        b=0;
      }
    }
  }
  while(setAlarm==3){
    lcd.setCursor(0,0);
    lcd.print("Set Second");
    lcd.setCursor(0,1);
    if(irrecv.decode(&results)){
      for(int i = 0; i<10; i++){
        if(results.value==num[i]){
         c = c*10+i;
        }else if(results.value==0xFFFFFFFF){
          c = c;  
        }else if(results.value==0xFFB04F){
          setAlarm++;
        }
        irrecv.resume();
      }
      lcd.print(a);
      lcd.print(':');
      lcd.print(b);
      lcd.print(':');
      if(c<60){
        lcd.print(c);
        if(results.value==0xFFB04F){
          setAlarm=4;
        }
      }else{
        lcd.clear();
        setAlarm = 3;
        c=0;
      }        
    }
  }
  while(setAlarm==4){
    lcd.setCursor(0,1);
    lcd.print("Alarm-");
    if(a<10)lcd.print("0");
    lcd.print(a);
    lcd.print(':');
    if(b<10)lcd.print("0");
    lcd.print(b);
    lcd.print(':');
    if(c<10)lcd.print("0");
    lcd.print(c);
    if(results.value==0xFFB04F){
      setAlarm=0;
    }
  }
  DateTime now = rtc.now();
  if(now.hour()==a && now.minute()==b && now.second()==c){
    tone(buzzerPin, 100);
    a=0;
    b=0;
    c=0;
  }
  if(irrecv.decode(&results)){
    if(results.value==0xFFA25D){
      a=0;
      b=0;
      c=0; 
      lcd.setCursor(0,1);
      lcd.clear();
    }
  }
  while(setAlarm==5){
    lcd.setCursor(0,0);
    lcd.print(ques1);
    lcd.print('+');
    lcd.print(ques2);
    lcd.print("      ");
    if(irrecv.decode(&results)){
      for(int i = 0; i<10; i++){
        if(results.value==num[i]){
          d = d*10+i;
        }else if(results.value==0xFFA25D){
          d=0;
        }
        irrecv.resume();
      }
      lcd.print(d);
    }
    if(digitalRead(snoozePin)==HIGH&&d==ans){
      noTone(buzzerPin);
      setAlarm=0;
      lcd.setCursor(0,1);
      lcd.clear();
      d=0;
    }
  }
}
