
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
    lcd.print("    ");
    getAlarm();
    snooze();
}
void alarm(int h, int m, int s){
  DateTime now = rtc.now();
  if(now.hour()==h && now.minute()==m && now.second()==s){
    tone(buzzerPin, 100);
  }
}
void snooze(){
  while(digitalRead(buzzerPin)==HIGH){
     if(digitalRead(snoozePin)==HIGH){
        noTone(buzzerPin);
     }
  }
}
void getAlarm(){
  int a = 0;
  int h = 0;
  int m = 0;
  int s = 0;
  if (irrecv.decode(&results)){
    if(a==0){
      if(results.value==0xFF02FD){
        h = getHour();
        a++;
      }
    }
    if(a==1){
      if(results.value==0xFF02FD){
        m = getMinute();
        a++;
      }
    }
    if(a==2){
      if(results.value==0xFF02FD){
        s = getSecond();
      }
    }
  }
  alarm(h,m,s);
}
int getHour(){
  int a = 0;
  if (irrecv.decode(&results)) {
    while(a<24){
      lcd.setCursor(0,1);
      switch(results.value){
        case 0xFF6897: a = a*10 + 0;
        lcd.print(a);
        break;
        case 0xFF30CF: a = a*10 + 1;
        lcd.print(a);
        break;
        case 0xFF18E7: a = a*10+2;
        lcd.print(a);
        break;
        case 0xFF7A85: a = a*10+3;
        lcd.print(a);
        break;
        case 0xFF10EF: a = a*10+4;
        lcd.print(a);
        break;
        case 0xFF38C7: a = a*10+5;
        lcd.print(a);
        break;
        case 0xFF5AA5: a = a*10+6;
        lcd.print(a);
        break;
        case 0xFF42BD: a = a*10+7;
        lcd.print(a);
        break;
        case 0xFF4AB5: a = a*10 +8;
        lcd.print(a);
        break;
        case 0xFF52AD: a = a*10+9;
        lcd.print(a);
        break;
      }
      irrecv.resume();  
    }
  }
  return a;
}
int getMinute(){
  int a = 0;
  if (irrecv.decode(&results)) {
    while(a<60){
      lcd.setCursor(0,1);
      switch(results.value){
        case 0xFF6897: a = a*10 + 0;
        lcd.print(a);
        break;
        case 0xFF30CF: a = a*10 + 1;
        lcd.print(a);
        break;
        case 0xFF18E7: a = a*10+2;
        lcd.print(a);
        break;
        case 0xFF7A85: a = a*10+3;
        lcd.print(a);
        break;
        case 0xFF10EF: a = a*10+4;
        lcd.print(a);
        break;
        case 0xFF38C7: a = a*10+5;
        lcd.print(a);
        break;
        case 0xFF5AA5: a = a*10+6;
        lcd.print(a);
        break;
        case 0xFF42BD: a = a*10+7;
        lcd.print(a);
        break;
        case 0xFF4AB5: a = a*10 +8;
        lcd.print(a);
        break;
        case 0xFF52AD: a = a*10+9;
        lcd.print(a);
        break;
      }
      irrecv.resume();  
    }
  }
  return a;
}
int getSecond(){
  int a = 0;
  if (irrecv.decode(&results)) {
    while(a<60){
      lcd.setCursor(0,1);
      switch(results.value){
        case 0xFF6897: a = a*10 + 0;
        lcd.print(a);
        break;
        case 0xFF30CF: a = a*10 + 1;
        lcd.print(a);
        break;
        case 0xFF18E7: a = a*10+2;
        lcd.print(a);
        break;
        case 0xFF7A85: a = a*10+3;
        lcd.print(a);
        break;
        case 0xFF10EF: a = a*10+4;
        lcd.print(a);
        break;
        case 0xFF38C7: a = a*10+5;
        lcd.print(a);
        break;
        case 0xFF5AA5: a = a*10+6;
        lcd.print(a);
        break;
        case 0xFF42BD: a = a*10+7;
        lcd.print(a);
        break;
        case 0xFF4AB5: a = a*10 +8;
        lcd.print(a);
        break;
        case 0xFF52AD: a = a*10+9;
        lcd.print(a);
        break;
      }
      irrecv.resume();  
    }
  }
  return a;
}