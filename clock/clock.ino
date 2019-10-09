
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
unsigned long key_value = 0;
int a = 0;
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
  if (irrecv.decode(&results)){
    if(results.value==0xFFB04F){
      if(setAlarm>=4){
        setAlarm=0;}
      else {
        setAlarm++;}
      delay(100);
    }
    Serial.println(setAlarm);
    while(setAlarm==0){
      lcd.setCursor(0,0);
      lcd.print("Set Hour");
      lcd.setCursor(0,1);
      for(int i = 0; i<10; i++){
        if(results.value==num[i]){
          a = a*10+i;
        }else if(results.value==0xFFFFFFFF){
          a = a;  
        }
        irrecv.resume();
      }
      lcd.print(a);
    }
    Serial.println(results.value, HEX);
  }
}
