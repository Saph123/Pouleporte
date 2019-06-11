#include "motor.hpp"

void tuningUp(){
  digitalWrite (H_B_Pin, HIGH);	// turn on 
  digitalWrite (H_A_Pin, LOW);	// turn off 
  delay(500);
  digitalWrite (H_A_Pin, LOW);	// turn off 
  digitalWrite (H_B_Pin, LOW);	// turn off 
  Serial.println("Fine tuning UP");
}
void tuningDown(){
  Serial.println("Fine tuning DOWN");
  digitalWrite (H_B_Pin, LOW);	// turn off 
  digitalWrite (H_A_Pin, HIGH);	// turn on 
  delay(250);
  digitalWrite (H_A_Pin, LOW);	// turn off 
  digitalWrite (H_B_Pin, LOW);	// turn off 
}
void upCmd(bool door_open, int time_up){
  int stepCnt = 0;
  if(!door_open)
  {
    Serial.println("Opening the door");
  digitalWrite (H_B_Pin, HIGH);	// turn on 
  digitalWrite (H_A_Pin, LOW);	// turn on

  while( stepCnt * 100 <= time_up) 
  {
    stepCnt += 1;
    delay(100);
    if(getVal(1)>1000)
    {
      break;
    }
    Serial.println("Openin");
  }
  digitalWrite (H_A_Pin, LOW);	// turn on 
  digitalWrite (H_B_Pin, LOW);	// turn on 
  Serial.println("Turning off engine");
  delay(500);
  door_open=true;
  }
  else
  {
    Serial.println("Door already opened");
  }
}
void downCmd(bool door_open, int time_down){
  if (door_open)
  {
    door_open=false;
    Serial.println("Shutting the door");
  digitalWrite (H_B_Pin, LOW);	// turn on 
  digitalWrite (H_A_Pin, HIGH);	// turn on 
  delay(time_down);
  digitalWrite (H_A_Pin, LOW);	// turn on 
  digitalWrite (H_B_Pin, LOW);	// turn on 
  delay(500);
  }
  else
  {
    Serial.println("Door already shut");
  }
  

}