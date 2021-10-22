
#include "main.hpp"
#define MOVINGAVERAGE 7
BluetoothSerial ESP_BT;
int val=0;
int tmpval[MOVINGAVERAGE] = {0};
int val_sensorbas=0;
int val_sensorhaut=0;
int Received_char=0,receivedValue=0;
long time_up=10400;
long time_down=6300;
long time_s=900;
int k=0;
bool safeState = false;
int temp_threshold=0;
int thresholdJour=1000;
int thresholdNuit=400;
boolean use_sensors = false;
boolean jour=true;
boolean firstTime=false;
long i=0,j=0;
boolean door_open=false;



void setup() 
{

  Serial.begin(115200);
  ESP_BT.begin("PoulePorte"); //Name of your Bluetooth Signal
  initSensor();

  initNVS();
  init_all_param();
  pinMode(PIN_LED, OUTPUT);
  pinMode (H_A_Pin, OUTPUT);
  pinMode (H_B_Pin, OUTPUT);
  adc1_config_width(ADC_WIDTH_BIT_12); // capteur lumière
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
}
void use_sensor_toggle()
{
  if(use_sensors)
  {
    use_sensors = false;
    nvs_set("use_sensors",0);

  }
  else
  {
    use_sensors = true;
    nvs_set("use_sensors",1);
  }
  if(use_sensors)
  {
    ESP_BT.println("Start using sensors!");
  }
  else
  {
    ESP_BT.println("Sensors are not used!");
  }

}
void changeDoorState()
{

  ESP_BT.println("Changing Door state");
  if(door_open)
  {
    door_open = false;
    nvs_set("door_open",0);

  }
  else
  {
    door_open = true;
    nvs_set("door_open",1);
  }
  ESP_BT.print("New door state: ");
  if(door_open)
  {
    ESP_BT.println("Door open");
  }
  else
  {
    ESP_BT.println("Door closed");
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
  //   esp_sleep_enable_timer_wakeup(10 * 1000000);
  // esp_deep_sleep_start();

  int sum = 0;
  val_sensorbas = getVal(CAPTEUR_DU_BAS);
  val_sensorhaut = getVal(CAPTEUR_DU_HAUT);


  Serial.println(door_open);
 

  delay(500);
  for(int i =1; i < (MOVINGAVERAGE - 1); i++){ // moving average on last 7 values
    tmpval[i] = tmpval[i-1];
  }
  tmpval[MOVINGAVERAGE-1] = adc1_get_raw(ADC1_CHANNEL_0);
  for(int i = 0; i < MOVINGAVERAGE; i++)
  {
    sum += tmpval[i];
  }
  val = sum/MOVINGAVERAGE;
  if(ESP_BT.available()) // Manage the bluetooth serial port
  {
    if(jour)
    {
      // ESP_BT.println("Il fait jour");
      ESP_BT.println("Jour");
      ESP_BT.print("Capteur lum moy:");
      ESP_BT.println(val);
      ESP_BT.print("Capteur lum raw:");
      ESP_BT.println(tmpval[MOVINGAVERAGE-1]);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print(thresholdJour);
      ESP_BT.print("/");
      ESP_BT.println(thresholdNuit);
      ESP_BT.print("Temps U/D: ");
      ESP_BT.print(time_up);
      ESP_BT.print("/");
      ESP_BT.println(time_down);
      if(door_open)
      {
        ESP_BT.println("Door is opened for me");
      }
      else
      {
          ESP_BT.println("Door is closed for me");
      }
      if((time_s-i)/2 != time_down)
        {
          ESP_BT.print((time_s-i)/2); ESP_BT.println(" secondes restantes avant fermeture");
        }
    if (use_sensors)
    {
      if( val_sensorbas >= 3000)
      {
        ESP_BT.println("capteur bas obstrue");
      }
      else
      {
        ESP_BT.println("capteur bas non obstrue");
      }
      if( val_sensorhaut >= 3000)
      {
        ESP_BT.println("capteur haut obstrue");
      }
      else
      {
        ESP_BT.println("capteur haut non obstrue");
      }
      if(door_open && (val_sensorbas <= 3000 || val_sensorhaut <= 3000))
      {
        ESP_BT.println("Y'a peut-etre un lezard car pour moi la porte est pas forcement bien ouverte");
      }
      if(!door_open && (val_sensorbas >= 3000 || val_sensorhaut >= 3000))
      {
        ESP_BT.println("Y'a peut-etre un lezard car pour moi la porte est pas forcement bien fermee!");
      }
    }
    else
    {
        ESP_BT.println("sensors are not used!");
    }

    }
    else
    {
      ESP_BT.println("Nuit");
      ESP_BT.println(val);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print(thresholdJour);
      ESP_BT.print("/");
      ESP_BT.println(thresholdNuit);
      ESP_BT.print("Tj/Tn: ");
      ESP_BT.print("temps U/D: ");
      ESP_BT.print(time_up);
      ESP_BT.print("/");
      ESP_BT.println(time_down);
      if(door_open)
      {
        ESP_BT.println("Door is opened for me");
      }
      else
      {
          ESP_BT.println("Door is closed for me");
      }
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
          upCmd(door_open, time_up, use_sensors);
          door_open=true;
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
              int err = downCmd(door_open, time_down, use_sensors);
              door_open = false;
              if (err != 0)
              {
                // eclairage led si pb
                digitalWrite(PIN_LED, HIGH);
                safeState=true;
                nvs_set("safeState",1);
              }

            }
          }
        }

    }
    }
    else if(Received_char==68) //D
    {
        if(ESP_BT.read() == 79) //O
        {
          if(ESP_BT.read() == 79) //O
          {
            if(ESP_BT.read() == 82) //R
            {
              changeDoorState();
            }

          }
        }

    }
    else if(Received_char==86) //V
    {
        if(ESP_BT.read() == 85) //U
        {
          if(ESP_BT.read() == 85) //U
          {
              digitalWrite(PIN_LED, LOW);
              ESP_BT.println("Reset LED");
              safeState =false;
              nvs_set("safeState",0);

          }
        }

    }
    else if(Received_char==85) //U
    {
        if(ESP_BT.read() == 83) //S
        {
            ESP_BT.println("Changing sensors state");
            use_sensor_toggle();
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
        nvs_set("time_s",time_s);
        }
        
      }
      else if(Received_char==74 || Received_char==78) // J or N for threshold
      {
        temp_threshold= parseSerial(4,0,4000);
        if (temp_threshold!=-1)
        {
          if(Received_char==74)
        {

            thresholdJour=temp_threshold;
            nvs_set("thresholdJour", thresholdJour);
        }
        else if(Received_char==78)
        {
          thresholdNuit=temp_threshold;
            nvs_set("thresholdNuit", thresholdNuit);
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
          nvs_set("time_up", time_up);
        }
        else if(Received_char==68) //D
          {
          time_down+=50;
          ESP_BT.print("New time_down: ");
          ESP_BT.println(time_down);
          nvs_set("time_down", time_down);
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
          nvs_set("time_up", time_up);

        }
        else if(Received_char==68) //D
          {
          time_down-=50;
          ESP_BT.print("New time_down: ");
          ESP_BT.println(time_down);
          nvs_set("time_down", time_down);

          }

      }
    }
  Serial.println(val);
  if (jour)
  {
    if(firstTime)
    {
      
      upCmd(door_open, time_up, use_sensors);
      door_open=true;
      nvs_set("firstTime",0);
      firstTime=false;
    }
    Serial.println("Au turbin les poulax: jour");
      if(val<thresholdNuit)
      {
        i++;
        if (i>time_s)
        {
          i=0;
          nvs_set("firstTime",1);
          nvs_set("jour", 0);
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
    // if firstTime entering night mode then you close the door
    if(firstTime)
    {
      
      downCmd(door_open, time_down, use_sensors);
      door_open=false;
      nvs_set("firstTime",0);
      firstTime=false;
    }
    Serial.println("Dodo les poulish : nuit");
          if(val > thresholdJour)
      {
        i++;
        if (i > time_s)
        {
          i=0;
          nvs_set("firstTime", 1);
          nvs_set("jour", 1);
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


void init_all_param()
{
  int door_open_int,safeState_int, use_sensors_int, jour_int, firstTime_int;
  time_up= nvs_get("time_up");
  time_down=nvs_get("time_down");
  time_s=nvs_get("time_s");
  thresholdJour=nvs_get("thresholdJour");
  thresholdNuit=nvs_get("thresholdNuit");
  door_open_int=nvs_get("door_open");
  safeState_int=nvs_get("safeState");
  use_sensors_int = nvs_get("use_sensors");
  jour_int = nvs_get("jour");
  firstTime_int = nvs_get("firstTime");


  if (use_sensors_int == 1)
  {
    use_sensors = true;
  }
  else
  {
      use_sensors = false;
  }
  if(safeState_int==1)
  {
    safeState=true;
  }
  else
  {
    safeState=false;
  }
  if(door_open_int==0)
  {
    door_open=false;
  }
  else
  {
    door_open=true;
  }
  if (jour_int == 0)
  {
    jour = false;
  }
  else
  {
    jour = true;
  }
  if(firstTime_int == 1)
  {
    firstTime = true;
  }
  else
  {
    firstTime = false;
  }
  if (time_up==0xDEADBEEF)
  {
    Serial.println("First boot I guess");
    // for first time, we initialise to default values
    time_up=10400;
    time_down=6300;
    time_s=900;
    thresholdJour=1000;
    thresholdNuit=400;
    door_open=false;
    nvs_set("time_up",time_up);
    nvs_set("time_down",time_down);
    nvs_set("time_s",time_s);
    nvs_set("thresholdJour",thresholdJour);
    nvs_set("thresholdNuit",thresholdNuit);
    nvs_set("jour", 1);
    nvs_set("firstTime", 0);
    if(safeState)
    {
    nvs_set("safeState",1);
    }
    else
    {
      nvs_set("safeState",0);
    }
    
    if(door_open)
    {
      nvs_set("door_open",1);
    }
    else
    {
      nvs_set("door_open",0);
    }
  }
}