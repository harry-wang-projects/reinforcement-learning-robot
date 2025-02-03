
int aone = 2;
int atwo = 4;
int bone = 5;
int btwo = 15;

void setup() {
  // put your setup code here, to run once:
  pinMode(aone, OUTPUT);
  pinMode(atwo, OUTPUT);
  pinMode(bone, OUTPUT);
  pinMode(btwo, OUTPUT);
  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 3 * 770 + 1638); 
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(aone, HIGH);
  digitalWrite(atwo, LOW);
  digitalWrite(bone, HIGH);
  digitalWrite(btwo, LOW);
  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 2000); 
  delay(1000);
  digitalWrite(aone, LOW);
  digitalWrite(atwo, LOW);
  digitalWrite(bone, LOW);
  digitalWrite(btwo, LOW);
  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 5000); 
  delay(1000);
  digitalWrite(aone, LOW);
  digitalWrite(atwo, HIGH);
  digitalWrite(bone, LOW);
  digitalWrite(btwo, HIGH);
  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 7500); 
  delay(1000);
  digitalWrite(aone, LOW);
  digitalWrite(atwo, LOW);
  digitalWrite(bone, LOW);
  digitalWrite(btwo, LOW);
  ledcAttach(18, 50, 16); // channel 1, 50 Hz, 16-bit width
  ledcWrite(18, 5000); 
  delay(1000);
}
