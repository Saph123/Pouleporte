#include <Arduino.h>
#include <driver/adc.h>
// #include <WiFi.h>
// #include "esp_wifi.h"
// #include <WebServer.h>
#include "BluetoothSerial.h"


const char* ssidAP     = "PoulePorte";
const char* ssid     = "Livebox-4E0F";
const char* passwordAP = "hahaha123";
const char* password = "besson74500";
BluetoothSerial ESP_BT;
// bool serverStarted=false;
// WiFiServer server(80);
int val=0;
int Received_char=0,receivedValue=0;
// Variable to store the HTTP request
String SendHTML(uint8_t upCmd,uint8_t downCmd);
long time_up=10400;
long time_down=6300;
long time_s=900;
int k=0;
int temp_threshold=0;
int threshold_jour=1000;
int thresholdNuit=400;
const int H_A_Pin = 16;
const int H_B_Pin = 21;
boolean jour=true;
boolean firstTime=false;
long i=0,j=0;
boolean door_open=true;
void upCmd();
void downCmd();
void tuningUp();
void tuningDown();

// void handle_OnConnect() {

//   Serial.println("Welcome darling!");
//   server.send(200, "text/html", SendHTML(false,false)); 
// }
// void handle_upCmd() {

//   Serial.println("Opening the door!");
//   server.send(200, "text/html", SendHTML(true,false)); 
//   upCmd();
// }
// void handle_downCmd() {

//   Serial.println("Closing the door!");
//   server.send(200, "text/html", SendHTML(false,true)); 
//   downCmd();
// }
void setup() 
{

      Serial.begin(115200);
ESP_BT.begin("PoulePorte"); //Name of your Bluetooth Signal
    // IPAddress servaddr(192,168,1,23);
    // IPAddress gw(192,168,1,1);
    // IPAddress subnetmask(255,255,255,0);
    // WiFi.config(servaddr,gw,subnetmask);
    // WiFi.begin(ssid,password);
    // WiFi.softAPConfig(servaddr,gw,subnetmask);
    // server.on("/", handle_OnConnect);
    // server.on("/UP", handle_upCmd);
    // server.on("/DOWN", handle_downCmd);
    // server.begin();

  pinMode (H_A_Pin, OUTPUT);
  pinMode (H_B_Pin, OUTPUT);
  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_11);
}

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
void upCmd(){
  if(!door_open)
  {
    Serial.println("Opening the door");
  digitalWrite (H_B_Pin, HIGH);	// turn on 
  digitalWrite (H_A_Pin, LOW);	// turn on 
  delay(time_up);
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
  
//   handle_OnConnect();
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
void downCmd(){
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
  
//   handle_OnConnect();
}
void loop() {
    // Serial.println(WiFi.status());
    // if(WiFi.status()==WL_CONNECTED)
    // {
    //     server.begin();
    //     serverStarted=true;
    // }
    // else
    // {
    //     Serial.println(WiFi.status());
    // }
    
//     if(serverStarted)
//     {
//       Serial.println(WiFi.localIP());
//         WiFiClient client = server.available();
//         if(client)
//         {
//             Serial.println("new client");
//             String currentLine;
//             while (client.connected()) {            // loop while the client's connected

//             if (client.available()) {             // if there's bytes to read from the client,
//         char c = client.read();             // read a byte, then
//         Serial.write(c);                    // print it out the serial monitor
//         if (c == '\n') {                    // if the byte is a newline character

//           // if the current line is blank, you got two newline characters in a row.
//           // that's the end of the client HTTP request, so send a response:
//           if (currentLine.length() == 0) {
//             // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
//             // and a content-type so the client knows what's coming, then a blank line:
//             client.println("HTTP/1.1 200 OK");
//             client.println("Content-type:text/html");
//             client.println();

//             // the content of the HTTP response follows the header:
//             client.print("<a href=\"/UP\">Ouvrir la porte</a><br>");
//             client.print("<a href=\"/DOWN\">Fermer la porte</a><br>");
//             client.print("<a href=\"/DOWN\">Fermer la porte</a><br>");
//             client.print("<a href=\"VAL\">Get val </a><br>");
//             // The HTTP response ends with another blank line:
//             client.println();
//             // break out of the while loop:
//             break;  
//           } else {    // if you got a newline, then clear currentLine:
//             currentLine = "";
//           }
//         } else if (c != '\r') {  // if you got anything else but a carriage return character,
//           currentLine += c;      // add it to the end of the currentLine
//         }

//         if (currentLine.endsWith("GET /UP")) {
//            upCmd();
//         }
//         if (currentLine.endsWith("GET /DOWN")) {
//           downCmd();
// }
//         if (currentLine.endsWith("GET /VAL")) {
//           client.print(val);
// }

        
//             }
// }
//         }
//     }
//    server.available();
  
  delay(500);
   val = adc1_get_raw(ADC1_CHANNEL_0);
    
  if(ESP_BT.available()) // Manage the bluetooth serial port
  {
    if(jour)
    {
      // ESP_BT.println("Il fait jour");
      ESP_BT.println("Porte ouverte");
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
      ESP_BT.print((time_s-i)/2); ESP_BT.println(" secondes restantes avant fermeture");
    }
    else
    {
      ESP_BT.println("Porte fermee");
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
      ESP_BT.print((time_s-i)/2);ESP_BT.println(" secondes restantes avant ouverture");
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
          upCmd();
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
              downCmd();

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
      
      upCmd();
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
      
      downCmd();
      firstTime=false;
    }
    Serial.println("Dodo les poulish : nuit");
          if(val>threshold_jour)
      {
        i++;
        if (i>time_s)
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

    

// String SendHTML( uint8_t upCmd,uint8_t downCmd )
// {

//   String ptr="<!DOCTYPE html><html>\n<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n<title>Pouloulouporte</title>\n<h1 style=\"text-align: center;\">POULE PORTE V0</h1>\n \
// <h2 style=\"text-align: center;\">Welcome to Poule porte manager bitchesss</h2></head>\n<body>\n \
// <p style=\"text-align: center;\"><a class=\"button button-on\" href=\"/DOWN\">HODOR</a>\n\"</p>\n \
// <p style=\"text-align: center;\"><a class=\"button button-on\" href=\"/UP\">LATER BITCHES</a>\n\"</p>\n</body>\n</html>\n";
// // if(jour)
// // {
// //   ptr+="Pour moi il fait jour";
// // }
// // else
// // {
// //   ptr+="Pour moi il fait nuit";
// // }
// return ptr;
// }
