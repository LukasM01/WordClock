#include <digitalWriteFast.h>

int serexlen, serindex, cmdvalid, actionsuccess;
char seraction;
String actionstr = ""; 

int SER_Pin = 8;   //pin 14 on the 75HC595
int RCLK_Pin = 9;  //pin 12 on the 75HC595
int SRCLK_Pin = 10; //pin 11 on the 75HC595

int defalutdisplay[10][11] ={{0,0,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,0,0,0,0,0,0,0,0,0},
                            {0,1,1,1,1,1,1,1,1,1,0},
                            {0,0,0,0,0,0,0,0,0,0,0}};

int display[10][11] ={{1,0,0,0,0,0,0,0,0,0,1},
                      {0,1,1,1,1,1,1,1,1,1,0},
                      {0,1,0,0,0,0,0,0,1,1,0},
                      {0,1,0,0,0,0,0,1,0,1,0},
                      {0,1,0,0,0,0,1,0,0,1,0},
                      {0,1,0,1,0,1,0,0,0,1,0},
                      {0,1,0,0,1,0,0,0,0,1,0},
                      {0,1,0,0,0,0,0,0,0,1,0},
                      {0,1,1,1,1,1,1,1,1,1,0},
                      {1,0,0,0,0,0,0,0,0,0,1}};



#define number_of_74hc595s 3 


#define numOfRegisterPins number_of_74hc595s * 8

boolean registers[numOfRegisterPins];

void setup(){
  pinMode(SER_Pin, OUTPUT);
  pinMode(RCLK_Pin, OUTPUT);
  pinMode(SRCLK_Pin, OUTPUT);
  
 // Serial.begin(115200);
  Serial.begin(9600);
  actionstr.reserve(100);
  
  //reset all register pins
  clearRegisters();
  writeRegisters();
}               

void clearRegisters(){
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
     registers[i] = LOW;
  }
} 


void writeRegisters(){
  digitalWrite(RCLK_Pin, LOW);
  for(int i = numOfRegisterPins - 1; i >=  0; i--){
    digitalWrite(SRCLK_Pin, LOW);
    digitalWrite(SER_Pin, registers[i]);
    digitalWrite(SRCLK_Pin, HIGH);

  }
  digitalWrite(RCLK_Pin, HIGH);
}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}


void loop(){
  while(Serial.available()){
   char inByte = Serial.read();
   serindex++;
   if(inByte == 10 || inByte == 13){
      if(serindex == serexlen && cmdvalid){
        
        // ACTIONS
        switch(seraction){
        case 'x':
          for(int y=9; y>=0; y--){
            for(int x=0; x<11; x++){
              display[y][x] = 0;
            }
          }
          actionsuccess = true;
          break;
        case 'o':
          for(int y=9; y>=0; y--){
            for(int x=0; x<11; x++){
              display[y][x] = 1;
            }
          }
          actionsuccess = true;
          break;
        case 'i':
          for(int y=9; y>=0; y--){
            for(int x=0; x<11; x++){
              display[y][x] = !display[y][x];
            }
          }
          actionsuccess = true;
          break;
        case 'd':
          for(int y=9; y>=0; y--){
            for(int x=0; x<11; x++){
              display[y][x] = defalutdisplay[y][x];
            }
          }
          actionsuccess = true;
          break;
        case 'f':
          for(int y=actionstr.charAt(1)-'A'; y<=actionstr.charAt(3)-'A'; y++){
            for(int x=actionstr.charAt(0)-'A';x<=actionstr.charAt(2)-'A'; x++){
              display[y][x] = actionstr.charAt(4)-'0';
            }
          }
          actionsuccess = true;
          break;
        case 'a':
          display[actionstr.charAt(1)-'A'][actionstr.charAt(0)-'A'] = 1;
          actionsuccess = true;
          break;
        case 'b':
          display[actionstr.charAt(1)-'A'][actionstr.charAt(0)-'A'] = 0;
          actionsuccess = true;
          break;
      }
        
        if(actionsuccess)
          Serial.write('1');
        else
          Serial.write('0');
          
        Serial.write('\n');
      }
      actionsuccess = false;
      actionstr = ""; 
      cmdvalid = false;
      serindex = 0;
    }else if(serindex == 1){
      seraction = inByte;
      switch(seraction){
        case 'x':
          serexlen = 2;
          cmdvalid = true;
          break;
        case 'o':
          serexlen = 2;
          cmdvalid = true;
          break;
        case 'i':
          serexlen = 2;
          cmdvalid = true;
          break;
        case 'd':
          serexlen = 2;
          cmdvalid = true;
          break;
        case 'f':
          serexlen = 7;
          cmdvalid = true;
          break;
        case 'a':
          serexlen = 4;
          cmdvalid = true;
          break;
        case 'b':
          serexlen = 4;
          cmdvalid = true;
          break;
        default:
          Serial.println('0');
          cmdvalid = false;
      }
    }else{
      actionstr += inByte;
    }
  }
  for(int y=9; y>=0; y--){
    setRegisterPin(11 + y, HIGH);
    for(int x=0; x<11; x++){
        setRegisterPin(x, display[y][x]);
    }
    writeRegisters();
   /* for(int x=0; x<11; x++){
        setRegisterPin(x, LOW);
    }*/
    setRegisterPin(11 + y, LOW);
  }

}
