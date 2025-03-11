//main code for learning robot

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
int dutycycle = 150;

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

  // We start by connecting to a WiFi network
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


  drive_motors(dutycycle, dutycycle);
  delay(1000);
  drive_motors(dutycycle, 0);
  delay(1000);
  drive_motors(dutycycle, -dutycycle);
  delay(1000);
  drive_motors(dutycycle, dutycycle);
  delay(1000);
  drive_motors(0, dutycycle);
  delay(1000);
  drive_motors(-dutycycle, dutycycle);
  delay(1000);

}
