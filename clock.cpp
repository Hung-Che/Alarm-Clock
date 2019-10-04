#include <LiquidCrystal.h> 
#include "RTClib.h"

RTC_DS1307 rtc;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int buzzerPin = 10;
const int snoozePin = 9;

void setup() {
  lcd.begin(16, 2);
  Serial.begin(9600);
  if(rtc.begin()&&rtc.isrunning()){
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

void loop() {
    DateTime now = rtc.now();
    lcd.setCursor(0, 0);
    lcd.print(now.hour());
    lcd.print(':');
    lcd.print(now.minute());
    lcd.print(':');
    if(now.second()<10){
      lcd.print("0");
    }
    lcd.print(now.second());
    lcd.print("    ");
    alarm(19, 58, 00);
    snooze();
}
void alarm(int h, int m, int s){
  DateTime now = rtc.now();
  if(now.hour()==h && now.minute()==m && now.second()==s){
    tone(buzzerPin, 100);
  }
}
void snooze(){
  if(digitalRead(buzzerPin)==HIGH){
    lcd.setCursor(0,1);
    lcd.print("bingo");
  }
}
