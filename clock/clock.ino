
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
    lcd.print("     ");
    getAlarm();
    snooze();
}
void snooze(){
  while(digitalRead(buzzerPin)==HIGH){
     if(digitalRead(snoozePin)==HIGH){
        noTone(buzzerPin);
     }
  }
}
void getAlarm(){
  if (irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    irrecv.resume();
    if(results.value==0xFFB04F){
        setAlarm=1;
      delay(300);
    }else if(results.value==0xFFFFFFFF){
      setAlarm = setAlarm;  
    }
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
        }else{
          lcd.clear();
          lcd.print("Invalid");
          if(irrecv.decode(&results)){
            lcd.clear();
            setAlarm=0;
            a=0;
          }
        }
        if(results.value==0xFFB04F){
          setAlarm=2;
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
      }else{
        lcd.clear();
        lcd.print("Invalid");
        if(irrecv.decode(&results)){
          lcd.clear();
          setAlarm=1;
          b=0;
        }
      }
      if(results.value==0xFFB04F){
          setAlarm=3;
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
        }else{
          lcd.clear();
          lcd.print("Invalid");
          if(irrecv.decode(&results)){
            lcd.clear();
            setAlarm=2;
            c=0;
          }
        }
        lcd.print(c);
        if(results.value==0xFFB04F){
          setAlarm=4;
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
  }
