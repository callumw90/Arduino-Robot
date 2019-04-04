#include <SoftwareSerial.h>
#include <NewPing.h>

#define TRIGGER_PIN 2
#define ECHO_PIN 3
#define MAX_DISTANCE 250

//SoftwareSerial BTserial(10, 11); //RX | TX
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);



const int analogInPinRight = A2;
const int analogInPinMid = A1;
const int analogInPinLeft = A0;
const int motor_left[] = {6, 7};
const int motor_right[] = {9, 8};
int distRight, distLeft, distance;
const int led = 13;

int sensorValRight = 0;
int sensorValLeft = 0;
int sensorValMid = 0;

char command = ' ';// int to store the data that we will receive from bluetooth


  void setup() {
    
      Serial.begin(38400); // opens serial port, sets data rate to 38400 bps
      Serial.println("Arduino Ready");
      //BTserial.begin(38400);
      command = 0;
    
      for(int i = 0; i < 2; i++){
  
        pinMode(motor_left[i], OUTPUT);
        pinMode(motor_right[i], OUTPUT);
      }

       pinMode(led, OUTPUT);
       digitalWrite(led, LOW);
       pinMode(analogInPinRight, INPUT);
       pinMode(analogInPinMid, INPUT);
       pinMode(analogInPinLeft, INPUT);

      
      distance = sonar.ping();
  }

void loop()
{
      if(Serial.available() > 0){
        command = Serial.read();
        Serial.println(command);
      }

      distance = sonar.ping();
     // Serial.println(distance);

      //sensorValRight = analogRead(analogInPinRight);
      //sensorValLeft = analogRead(analogInPinLeft);
      //sensorValMid = analogRead(analogInPinMid);

     // Serial.println("Right: ");
      //Serial.print(sensorValRight);
      //Serial.println("Left: ");
      //Serial.print(sensorValLeft);
      
     distRight = 0;
     distLeft = 0;
     delay(50);

     if(command == '0'){

        distance = sonar.ping();
        
          if(distance >= 600)
          {
            drive_forward();            
            digitalWrite(led, LOW);
            delay(500);
            motor_stop();
          }
        else
        {
          digitalWrite(led, HIGH);
        }
     }

     else if(command == '1'){

        drive_backward();
        
     }

     else if(command == '2'){

      turn_left();
      
     }

     else if(command == '3'){

      turn_right();
      
     }

     else if(command == '4'){

      motor_stop();
      
     }

      //auto mode no line following
     else if(command == '5'){

      if(distance <= 300){
        
        motor_stop();
        delay(500);
        drive_backward();
        delay(400);
        motor_stop();
        delay(300);
        distRight = measure_right();
        delay(300);
        distLeft = measure_left();
        delay(300);

        if(distRight > distLeft){
        
            turn_right();
            delay(500);  
        }

        else if(distRight < distLeft){

          turn_left();
          delay(500);          
        }

        else if((distRight <= 300) || (distLeft <= 300)){

          drive_backward();
          delay(300);          
        }

        else{
          
          drive_forward();
        }        
    }

    else{

      drive_forward();
    }      
   }

    //line following
   else if(command == '6'){

      sensorValRight = analogRead(analogInPinRight);
      sensorValLeft = analogRead(analogInPinLeft);
      //sensorValMid = analogRead(analogInPinMid);

      Serial.println("Right: ");
      Serial.print(sensorValRight);
      Serial.println("Left: ");
      Serial.print(sensorValLeft);
      //Serial.println("Middle: ");
      //Serial.print(sensorValMid);

      if(sensorValRight > 840 && sensorValLeft < 995){

        turn_left();
        delay(300);
        motor_stop();
      }
      else if(sensorValLeft > 950 && sensorValRight < 950){

        turn_right();
        delay(300);
        motor_stop();
      }
      else{

        drive_forward();
        delay(300);
        motor_stop();
      }
   }



}

  int measure_right(){

      int tempDist = 0;
      turn_right();
      delay(500);
      motor_stop();
      tempDist = sonar.ping();
      turn_left();
      delay(500);
      motor_stop();
      
      return tempDist;
  }

  int measure_left(){

      int tempDist = 0;
      turn_left();
      delay(500);
      motor_stop();
      tempDist = sonar.ping();
      turn_right();
      delay(500);
      motor_stop();
      
      return tempDist;
    
  }


          void motor_stop(){
            
            digitalWrite(motor_left[0], LOW); 
            digitalWrite(motor_left[1], LOW); 
             
            digitalWrite(motor_right[0], LOW); 
            digitalWrite(motor_right[1], LOW);
           
          }
           
          void drive_forward(){
            
            digitalWrite(motor_left[1], HIGH); 
            digitalWrite(motor_left[0], LOW); 
             
            digitalWrite(motor_right[1], HIGH); 
            digitalWrite(motor_right[0], LOW); 
             
          }
           
          void drive_backward(){
            
            digitalWrite(motor_left[1], LOW); 
            digitalWrite(motor_left[0], HIGH); 
             
            digitalWrite(motor_right[1], LOW); 
            digitalWrite(motor_right[0], HIGH); 
             
          }
           
          void turn_left(){
            
            digitalWrite(motor_left[0], LOW); 
            digitalWrite(motor_left[1], HIGH); 
             
            digitalWrite(motor_right[0], HIGH); 
            digitalWrite(motor_right[1], LOW);
             
          }
           
          void turn_right(){
            
            digitalWrite(motor_left[0], HIGH); 
            digitalWrite(motor_left[1], LOW); 
             
            digitalWrite(motor_right[0], LOW); 
            digitalWrite(motor_right[1], HIGH); 
             
          }
