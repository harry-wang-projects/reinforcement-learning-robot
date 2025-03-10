//main code for learning robot

#include <WiFi.h>
#include "VL53L1X.h"


//sensor
VL53L1X sensor;

void sensor_init(VL53L1X::DistanceMode range_mode, bool high_speed) {
  Wire.begin();
  sensor.setTimeout(500);
  sensor.init();
  sensor.setDistanceMode(range_mode);  
  int budget = high_speed ? 33000 : 140000;
  sensor.setMeasurementTimingBudget(budget);
}

//motors
int aone = 2;
int atwo = 4;
int bone = 5;
int btwo = 15;


int doiblink = 0;
int blinktime = 0;

void quickblink(){
  digitalWrite(23, HIGH);
  doiblink = 1;
  blinktime = millis() + 200;
}

void drive_motors(int aspeed, int bspeed)
{
  if(aspeed > 0){
    analogWrite(aone, aspeed);
    analogWrite(atwo, 0);
  }else if(aspeed < 0){
    analogWrite(aone, 0);
    analogWrite(atwo, -aspeed);
  }else{
    digitalWrite(aone, LOW);
    digitalWrite(atwo, LOW);
  }
  if(bspeed > 0){
    analogWrite(bone, bspeed);
    analogWrite(btwo, 0);
  }else if(bspeed < 0){
    analogWrite(bone, 0);
    analogWrite(btwo, -bspeed);
  }else{
    digitalWrite(bone, LOW);
    digitalWrite(btwo, LOW);
  }
}

//servo

float maxduty = 8200.0;
float minduty = 1800.0;
float angle2servo(int angle){
  return (float)angle * (maxduty - minduty) / 180.0 + minduty;
}

int angle = 0;

//server
NetworkServer server(80);

void setup() {
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);
  delay(500);
  digitalWrite(23, LOW);
  delay(500);

  pinMode(aone, OUTPUT);
  pinMode(atwo, OUTPUT);
  pinMode(bone, OUTPUT);
  pinMode(btwo, OUTPUT);

  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 3 * 770 + 1638); 

  Serial.begin(115200);

  delay(10);

  sensor_init(VL53L1X::Long, false);   

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  /*
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  */
  WiFi.softAP("harry2", NULL);

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  digitalWrite(23, HIGH);
  delay(500);
  digitalWrite(23, LOW);
  delay(500);
}

int last_check = 0;
//time for each small scan
int wait_milliseconds = 20;

//(180/5)+1
int distances[37] = {0};

//0 = increasing, 1 = decreasing
int spin_direction = 0;

void loop() {
  if (doiblink == 1 && millis() >= blinktime){
    doiblink = 0;
    digitalWrite(23, LOW);
  }
  /*
  if(millis() - last_check > wait_milliseconds){
    last_check = millis();

    int dist = sensor.readRangeSingleMillimeters();
    distances[angle / 5] = dist;

    if(spin_direction == 0){
      angle += 5;
      if(angle == 180){
        spin_direction = 1;
      }
    }else{
      angle -= 5;
      if(angle == 0){
        spin_direction = 0;
      }
    }
    ledcWrite(18, (int)angle2servo(angle)); 
  }
  */

  NetworkClient client = server.accept();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine = "";        // make a String to hold incoming data from the client
    while (client.connected()) {    // loop while the client's connected
      if (client.available()) {     // if there's bytes to read from the client,
        char c = client.read();     // read a byte, then
        Serial.write(c);            // print it out the serial monitor
        if (c == '\n') {            // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            // print radar scanning data
            /*
            for(int i = 0; i < 37; i++){
              client.print(distances[i]);
              client.print(" ");
            }
            client.println();
            */
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        int dutycycle = 150;
        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /Q")) {
          drive_motors(0, dutycycle);
        }
        if (currentLine.endsWith("GET /W")) {
          drive_motors(dutycycle, dutycycle);  
        }
        if (currentLine.endsWith("GET /E")) {
          drive_motors(dutycycle, 0);
        }
        if (currentLine.endsWith("GET /A")) {
          drive_motors(-dutycycle, dutycycle);
        }
        if (currentLine.endsWith("GET /S")) {
          drive_motors(0, 0);
        }
        if (currentLine.endsWith("GET /D")) {
          drive_motors(dutycycle, -dutycycle);
        }
        if (currentLine.endsWith("GET /Z")) {
          drive_motors(0, -dutycycle);
        }
        if (currentLine.endsWith("GET /X")) {
          drive_motors(-dutycycle, -dutycycle);
        }
        if (currentLine.endsWith("GET /C")) {
          drive_motors(-dutycycle, 0);
        }
      }
      quickblink();
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

}
