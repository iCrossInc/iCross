const int BUZZER_OUT = 9;     // Output digital pin for buzzer that will sound if pedestrian was detected

void setup() {
  // put your setup code here, to run once:
  pinMode(BUZZER_OUT, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
//  tone(BUZZER_OUT, 450, 500);

  digitalWrite(BUZZER_OUT, HIGH);

  delay(1000);
}
