/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA2 encryption. For insecure
 WEP or WPA, change the Wifi.begin() call and use Wifi.setMinSecurity() accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32
31.01.2017 by Jan Hendrik Berlin

 */

#include <WiFi.h>

const char *ssid = "gamma";
const char *password = "8b5bjz4zns";

NetworkServer server(80);

int aone = 2;
int atwo = 4;
int bone = 5;
int btwo = 15;

void drive_motors(int aspeed, int bspeed)
{
  if(aspeed > 0){
    digitalWrite(aone, HIGH);
    digitalWrite(atwo, LOW);
  }else if(aspeed < 0){
    digitalWrite(aone, LOW);
    digitalWrite(atwo, HIGH);
  }else{
    digitalWrite(aone, LOW);
    digitalWrite(atwo, LOW);
  }
  if(bspeed > 0){
    digitalWrite(bone, HIGH);
    digitalWrite(btwo, LOW);
  }else if(bspeed < 0){
    digitalWrite(bone, LOW);
    digitalWrite(btwo, HIGH);
  }else{
    digitalWrite(bone, LOW);
    digitalWrite(btwo, LOW);
  }
}

float maxduty = 7500.0;
float minduty = 2000.0;
float angle2servo(int angle){
  return (float)angle * (maxduty - minduty) / 180.0 + minduty;
}

int angle = 0;

void setup() {

  pinMode(aone, OUTPUT);
  pinMode(atwo, OUTPUT);
  pinMode(bone, OUTPUT);
  pinMode(btwo, OUTPUT);

  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 3 * 770 + 1638); 

  Serial.begin(115200);

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

int last_check = 0;
//time for each small scan
int wait_milliseconds = 10;

//(180/5)+1
int distances[37] = {0};

//0 = increasing, 1 = decreasing
int spin_direction = 0;

void loop() {
  if(millis() - last_check > wait_milliseconds){
    last_check = millis();

    if(move_direction == 0){
      angle += 5;
      if(angle == 180){
        move_direction = 1;
      }
    }else{
      angle -= 5;
      if(angle == 0){
        move_direction = 0;
      }
    }
    ledcWrite(18, (int)angle2servo(angle)); 
  }

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
            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");

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

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /Q")) {
          drive_motors(1, 0)
        }
        if (currentLine.endsWith("GET /W")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /E")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /A")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /S")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /D")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /Z")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /X")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
        if (currentLine.endsWith("GET /C")) {
          digitalWrite(5, LOW);  // GET /L turns the LED off
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }

}
