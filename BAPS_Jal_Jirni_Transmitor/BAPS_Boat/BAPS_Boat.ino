#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "BAPS Boat";
const char* password = "12345678";

String command;     //String to store app command state.

Servo LeftESC, RightESC ; // create servo object to control a servo 
ESP8266WebServer server(80);

void setup() {
 
  Serial.begin(115200);
  LeftESC.attach(D5, 1000, 2000);
  RightESC.attach(D6, 1000, 2000);
 
  LeftESC.writeMicroseconds(1500);
  RightESC.writeMicroseconds(1500);
  //myservoSeed.write(160); //On Startups Default Location If not OK Then CHANGE1 160 to 0
  //myservoSoil.write(0); //On Startups Default Location If not OK Then CHANGE2 0 to 160

// Wait for ESC initialization
  delay(2000); // 2 seconds
  
// Connecting WiFi
  WiFi.mode(WIFI_AP);           //Only Access point
  WiFi.softAP(ssid, password);  //Start HOTspot removing password will disable security
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 // Starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void loop() {
    server.handleClient();
      command = server.arg("State");
      Serial.println(command);
           if (command == "F") goForword();
      else if (command == "B") goBack();
      else if (command == "L") goLeft();
      else if (command == "R") goRight();
      else if (command == "0") speedCar = 100;
      else if (command == "1") speedCar = 120;
      else if (command == "2") speedCar = 140;
      else if (command == "3") speedCar = 160;
      else if (command == "4") speedCar = 180;
      else if (command == "5") speedCar = 200;
      else if (command == "6") speedCar = 215;
      else if (command == "7") speedCar = 230;
      else if (command == "8") speedCar = 240;
      else if (command == "9") speedCar = 255;
      else if (command == "S") stopRobot();
}

void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}

  void goForword(){ 
      LeftESC.writeMicroseconds(2000);
      RightESC.writeMicroseconds(2000); 
   }

  void goBack(){ 
      LeftESC.writeMicroseconds(1000);
      RightESC.writeMicroseconds(1000);
   }

  void goRight(){ 
     LeftESC.writeMicroseconds(2000);
     RightESC.writeMicroseconds(1000);
    }

  void goLeft(){
     LeftESC.writeMicroseconds(1000);
     RightESC.writeMicroseconds(2000);
    }

  void stopRobot(){  
     LeftESC.writeMicroseconds(1500);
     RightESC.writeMicroseconds(1500);
    }
