#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <Servo.h>

const char* ssid = "BAPS Boat";
const char* password = "12345678";

String command; // String to store app command state.
int speedCar = 150; // Initial speed (0 to 255)
const int minPulseWidth = 1000; // Minimum pulse width in microseconds
const int maxPulseWidth = 2000; // Maximum pulse width in microseconds
const int neutralPulseWidth = 1500; // Neutral throttle position in microseconds
int LED = D4;

Servo LeftESC, RightESC; // Create servo objects to control the ESCs
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);
  LeftESC.attach(D5, minPulseWidth, maxPulseWidth);
  RightESC.attach(D6, minPulseWidth, maxPulseWidth);
  pinMode(LED, OUTPUT);
  // Initialize ESCs
  LeftESC.writeMicroseconds(neutralPulseWidth);
  RightESC.writeMicroseconds(neutralPulseWidth);

  // Wait for ESC initialization
  delay(2000); // 2 seconds
  
  // Connect to WiFi
  WiFi.mode(WIFI_AP); // Only Access Point
  WiFi.softAP(ssid, password); // Start Hotspot, removing password will disable security
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Start web server
  server.on("/", HTTP_handleRoot);
  server.onNotFound(HTTP_handleRoot);
  server.begin();   
  digitalWrite(LED, HIGH); 
  delay(1000);
}

void loop() {
  digitalWrite(LED, HIGH); 
  server.handleClient();
  command = server.arg("State");
  Serial.println(command);

  if (command == "F") goForward();
  else if (command == "B") goBack();
  else if (command == "L") goLeft();
  else if (command == "R") goRight();
  else if (command == "S") stopRobot();
  else if (command.length() == 1 && command[0] >= '0' && command[0] <= '9') {
    speedCar = map(command.toInt(), 0, 9, 0, 255);
    Serial.print("Speed set to: ");
    Serial.println(speedCar);
  }
}

void HTTP_handleRoot(void) {
  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
  server.send(200, "text/html", "");
  delay(1);
}

void goForward() { 
  int pulseWidth = map(speedCar, 0, 255, minPulseWidth, maxPulseWidth);
  LeftESC.writeMicroseconds(pulseWidth);
  RightESC.writeMicroseconds(pulseWidth); 
}

void goBack() { 
  int pulseWidth = map(speedCar, 0, 255, minPulseWidth, maxPulseWidth);
  LeftESC.writeMicroseconds(map(speedCar, 0, 255, maxPulseWidth, minPulseWidth));
  RightESC.writeMicroseconds(map(speedCar, 0, 255, maxPulseWidth, minPulseWidth));
}

void goRight() { 
  int leftPulseWidth = map(speedCar, 0, 255, maxPulseWidth, minPulseWidth);
  int rightPulseWidth = map(speedCar, 0, 255, minPulseWidth, maxPulseWidth);
  LeftESC.writeMicroseconds(leftPulseWidth);
  RightESC.writeMicroseconds(rightPulseWidth);
}

void goLeft() { 
  int leftPulseWidth = map(speedCar, 0, 255, minPulseWidth, maxPulseWidth);
  int rightPulseWidth = map(speedCar, 0, 255, maxPulseWidth, minPulseWidth);
  LeftESC.writeMicroseconds(leftPulseWidth);
  RightESC.writeMicroseconds(rightPulseWidth);
}

void stopRobot() {  
  LeftESC.writeMicroseconds(neutralPulseWidth);
  RightESC.writeMicroseconds(neutralPulseWidth);
}
