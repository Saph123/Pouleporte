#include "motor.hpp"
#include "safeState.hpp"

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
    Serial.println("Openin");
    if(getVal(CAPTEUR_DU_HAUT)>4000)
    {
      break;
    }
  }
    Serial.println("Fin de course montee");
  digitalWrite (H_A_Pin, LOW);	// turn on 
  digitalWrite (H_B_Pin, LOW);	// turn on 
  Serial.println("Turning off engine");
  delay(500);
  door_open=true;
  nvs_set("door_open", 1);
  }
  else
  {
    Serial.println("Door already opened");
  }
}
int downCmd(bool door_open, int time_down){
int stepCnt = 0;
  if (door_open)
  {
    
    Serial.println("Shutting the door");
  digitalWrite (H_B_Pin, LOW);	// turn on 
  digitalWrite (H_A_Pin, HIGH);	// turn on 
  while( stepCnt * 100 <= time_down) 
  {
    stepCnt += 1;
    delay(100);
    Serial.println("Closing");
    if(getVal(CAPTEUR_DU_BAS)<500)
    {
      break;
    }
  }
    Serial.println("Fin de course descente");
  door_open=false;
  nvs_set("door_open",0);
  // delay(time_down);
  digitalWrite (H_A_Pin, LOW);	// turn on 
  digitalWrite (H_B_Pin, LOW);	// turn on 
  delay(500);
  }
  else
  {
    Serial.println("Door already shut");
  }
    if(getVal(CAPTEUR_DU_BAS) > 3000)
  {
    // The sensor did not see the door ? Either not working or door stuck
    return 0xFF;
  }
  else
  {
    return 0;
  }

}