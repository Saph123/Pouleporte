#include <Arduino.h>
#include <driver/adc.h>
#include "BluetoothSerial.h"
#include "motor.hpp"

#define PIN_LED 5
BluetoothSerial ESP_BT;
int val=0;
int val_sensor1=0;
int Received_char=0,receivedValue=0;
// Variable to store the HTTP request
long time_up=10400;
long time_down=6300;
long time_s=900;
int k=0;
int temp_threshold=0;
int threshold_jour=1000;
int thresholdNuit=400;

boolean jour=true;
boolean firstTime=false;
long i=0,j=0;
boolean door_open=true;



void setup() 
{

  Serial.begin(115200);
  ESP_BT.begin("PoulePorte"); //Name of your Bluetooth Signal
  adc2_config_channel_atten(ADC2_CHANNEL_0,ADC_ATTEN_DB_11); // configure the ADC2 ch0
  pinMode(PIN_LED, OUTPUT);
  pinMode (H_A_Pin, OUTPUT);
  pinMode (H_B_Pin, OUTPUT);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
}

void changeDoorState()
{
  if(door_open)
  {
    door_open = false;
  }
  else
  {
    door_open = true;
  }
  
}

  

int parseSerial(int digits,int limitDown,int limitUp)
{
  int localValue=0;
  for (k=0;k<digits;k++)
        {
          receivedValue=ESP_BT.read();
          if (receivedValue>=48 && receivedValue<=57)
          {
            if (k==0)
            localValue=(receivedValue-48)*1000;
            else if(k==1)
            localValue+=(receivedValue-48)*100;
            else if(k==2)
            localValue+=(receivedValue-48)*10;
            else if(k==3)
            localValue+=receivedValue-48;
          }
          else
          {
            ESP_BT.print("This is not a number: "); ESP_BT.println(receivedValue);
          }
        }
        if(localValue<=limitUp && localValue>=limitDown)
        {
          ESP_BT.print("Putting this value to the threshold: "); ESP_BT.println(localValue);
          return localValue;
        }
        else
        {
          ESP_BT.print("Threshold must be between 0 and 4000: "); ESP_BT.println(localValue);
          return -1;
        }

        

}

void loop() {

  adc2_get_raw(ADC2_CHANNEL_0,ADC_WIDTH_BIT_12, &val_sensor1);
  Serial.print(val_sensor1);
  Serial.println(" --> this is sensor");
  if(val_sensor1 < 500 || door_open)
  {
  digitalWrite (PIN_LED, LOW);

  }
  else if (val_sensor1 >= 500 && !door_open)
  {

  digitalWrite (PIN_LED, HIGH);
  }
  
  delay(500);
   val = adc1_get_raw(ADC1_CHANNEL_0);
    
  if(ESP_BT.available()) // Manage the bluetooth serial port
  {
    if(jour)
    {
      // ESP_BT.println("Il fait jour");
      ESP_BT.println("Jour");
      ESP_BT.println(val);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print(threshold_jour);
      ESP_BT.print("/");
      ESP_BT.println(thresholdNuit);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print("temps U/D: ");
      ESP_BT.print(time_up);
      ESP_BT.print("/");
      ESP_BT.println(time_down);
      if((time_s-i)/2 != time_down)
        {
          ESP_BT.print((time_s-i)/2); ESP_BT.println(" secondes restantes avant fermeture");
        }

    }
    else
    {
      ESP_BT.println("Nuit");
      ESP_BT.println(val);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print(threshold_jour);
      ESP_BT.print("/");
      ESP_BT.println(thresholdNuit);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print("temps U/D: ");
      ESP_BT.print(time_up);
      ESP_BT.print("/");
      ESP_BT.println(time_down);
      if((time_s-i)/2 != time_up)
      {
        ESP_BT.print((time_s-i)/2);ESP_BT.println(" secondes restantes avant ouverture");
      }
    }
    Received_char=ESP_BT.read();
    Serial.print("Received"); Serial.println(Received_char);
    if(Received_char==67) // UP on serial
    {
      Received_char=ESP_BT.read();
      if(Received_char==85)
      {
        if(ESP_BT.read()==80)
        {
          ESP_BT.println("UP CMD Received!");
          upCmd(door_open, time_up);
        }
      }
      else if(Received_char==68)
        {
          if(ESP_BT.read()==79)
        {
          if(ESP_BT.read()==87)
          {
            if(ESP_BT.read()==78)
            {
              ESP_BT.println("DOWN CMD Received!");
              downCmd(door_open, time_down);

            }
          }
        }

    }
    }

    
    else if(Received_char==83) // S
    {
      Received_char=ESP_BT.read();
      if(Received_char==84) //T
      {
        temp_threshold= parseSerial(4,0,2000);
        if (temp_threshold!=-1)
        {
        time_s =temp_threshold;
        }
        
      }
      else if(Received_char==74 || Received_char==78) // J or N for threshold
      {
        temp_threshold= parseSerial(4,0,4000);
        if (temp_threshold!=-1)
        {
          if(Received_char==74)
        {

            threshold_jour=temp_threshold;
        }
        else if(Received_char==78)
        {
          thresholdNuit=temp_threshold;
        }
        }
      }
          
      }
      else if(Received_char==84) //T
      {
          Received_char=ESP_BT.read();
          if(Received_char==85) //U
          {
            ESP_BT.println("FineTuning UP");
            tuningUp();

          }
          else if(Received_char==68) //D
          {
            ESP_BT.println("FineTuning DOWN");
            tuningDown();

          }
      }
      else if(Received_char==65)//A
      {
        Received_char=ESP_BT.read();
        if(Received_char==85) //U
        {
          
          time_up+=50;
          ESP_BT.print("New time_up: ");
          ESP_BT.println(time_up);
        }
        else if(Received_char==68) //D
          {
          time_down+=50;
          ESP_BT.print("New time_down: ");
          ESP_BT.println(time_down);

          }

      }
      else if(Received_char==77)//M
      {
        Received_char=ESP_BT.read();
        if(Received_char==85) //U
        {
          
          time_up-=50;
          ESP_BT.print("New time_up: ");
          ESP_BT.println(time_up);
        }
        else if(Received_char==68) //D
          {
          time_down-=50;
          ESP_BT.print("New time_down: ");
          ESP_BT.println(time_down);

          }

      }
    }
  Serial.println(val);
  if (jour)
  {
    if(firstTime)
    {
      
      upCmd(door_open, time_up);
      firstTime=false;
    }
    Serial.println("Au turbin les poulax: jour");
      if(val<thresholdNuit)
      {
        i++;
        if (i>time_s)
        {
          i=0;
          firstTime=true;
          jour=false;
        }
      }
      else
      {
        i=0;
      }
      
  }
  //Nuit mode
   else
  {
    // if firsttime entering night mode then you close the door
    if(firstTime)
    {
      
      downCmd(door_open, time_down);
      firstTime=false;
    }
    Serial.println("Dodo les poulish : nuit");
          if(val > threshold_jour)
      {
        i++;
        if (i > time_s)
        {
          i=0;
          firstTime=true;
          jour=true;
        }
      }
      else
      {
        i=0;
      }
      
  }
  Serial.println(i);
}
