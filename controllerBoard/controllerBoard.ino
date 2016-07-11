//libs
#include <Wire.h>
#include <RTClib.h>
//rtc object
RTC_DS1307 RTC;
//clock variables
int displayhour;
int displayminute;
//clock update minute
int time;
//startup
void setup() {
  //intialize Serial to second mcu
  Serial.begin(9600);
  clear();
  //intialize RTC
  Wire.begin();
  RTC.begin();
  //time update:
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  time = 61; //incorrect so clock will update
}

void loop() {
  //only update if minute is different
  DateTime now = RTC.now();
   if(now.minute()!=time){
    printtime(now.hour(),now.minute());
    time = now.minute();
  }
  //wait a few seconds
  delay(10000);

}

void printtime(int hour, int minute){
  //first clear clock
  clear();
  //1-12 hour conversion
  int clockhour = hour%12;
  //locals
  displayhour = clockhour;
  displayminute = round(minute/5.0)*5;

  //general words
  clock_fill(0,0,1,0);//es
  clock_fill(3,0,5,0);//ist
  //round hour
  if (displayminute >= 25) {
    displayhour++;
  }

  //time with miutes?
  if (displayminute != 0 && displayminute != 30 && displayminute != 60){
    if (displayminute % 15 == 0) {
      clock_fill(4,2,10,2);//viertel
    } else if (displayminute % 20 == 0) {
      clock_fill(4,1,10,1);//zwanzig
    } else if (displayminute % 10 == 0) {
      clock_fill(0,1,3,1);//zehn
    } else if (displayminute % 5 == 0) {
      clock_fill(7,0,10,0);//fünf
    }
  }
  //display vor?
  if (displayminute == 25||(displayminute >= 40 && displayminute <= 55)) {
    clock_fill(6,3,8,3);//vor
  } else 
  //display nach?
  if (displayminute == 35||(displayminute >= 5 && displayminute <= 20)) {
    clock_fill(2,3,5,3);//nach
  }
  //or is minute rounded half with or without vor/nach
  if ((displayminute >= 25 && displayminute <= 35)|| displayminute == 0) {
    if (displayminute >= 25 && displayminute <= 35)
      clock_fill(0,4,3,4);//halb
  }
  //hour switch
    switch(displayhour){
    case 1:
    
      if(displayminute == 0 || displayminute == 60)
        clock_fill(2,5,4,5);//ein (uhr)
      else
        clock_fill(2,5,5,5);//eins (viertel vor)
      break;
    case 2:
      clock_fill(0,5,3,5);
      break;
    case 3:
      clock_fill(1,6,4,6);
      break;
    case 4:
      clock_fill(7,7,10,7);
      break;
    case 5:
      clock_fill(7,6,10,6);
      break;
    case 6:
      clock_fill(1,9,5,9);
      break;
    case 7:
      clock_fill(5,5,10,5);
      break;
    case 8:
      clock_fill(1,8,4,8);
      break;
    case 9:
      clock_fill(3,7,6,7);
      break;
    case 10:
      clock_fill(5,8,8,8);
      break;
    case 11:
      clock_fill(0,7,2,7);
      break;
    case 0:
      clock_fill(5,4,9,4);
      break;
  }
  //new hour
  if(displayminute == 0 || displayminute == 60){
    clock_fill(8,9,10,9);//uhr
  }
}


void clock_fill(int x1, int y1, int x2, int y2){
  //fill
  Serial.write("f");
  //x1
  Serial.write(x1+'A');
  //y1
  Serial.write(y1+'A');
  
  //x2
  Serial.write(x2+'A');
  //y2
  Serial.write(y2+'A');
  
  //color
  Serial.write('1');
  //stop byte
  Serial.write('\n');
}
void clear(){
  //clear
  //x and new line
  Serial.println('x');
}

time_t requestSync()
{
  return setTime(RTC.now());
}
