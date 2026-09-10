/*
  SMART GLOVES FOR VISUALLY IMPAIRED
  ------------------------------------
  Components:
    - Arduino Uno/Nano
    - HC-SR04 Ultrasonic Sensor (obstacle detection)
    - Buzzer (audio alert)
    - Vibration Motor (haptic alert)
    - LDR / Light sensor (optional - detects low light for extra caution)

  How it works:
    - Ultrasonic sensor continuously measures distance to nearest obstacle.
    - If an obstacle is closer than SAFE_DISTANCE, buzzer + vibration motor
      turn ON to alert the user.
    - Alert intensity increases (faster beeping) as the obstacle gets closer.
*/

// ---- Pin definitions ----
const int TRIG_PIN = 9;
const int ECHO_PIN = 10;
const int BUZZER_PIN = 6;
const int VIBRATION_PIN = 7;
const int LDR_PIN = A0;

// ---- Settings ----
const int SAFE_DISTANCE_CM = 100;   // distance considered "safe"
const int DANGER_DISTANCE_CM = 30;  // distance considered "very close"

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(VIBRATION_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Smart Gloves Initialized...");
}

long getDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;

  long distance = duration * 0.034 / 2;
  return distance;
}

void alertUser(int beepDelay) {
  digitalWrite(BUZZER_PIN, HIGH);
  digitalWrite(VIBRATION_PIN, HIGH);
  delay(beepDelay);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(VIBRATION_PIN, LOW);
  delay(beepDelay);
}

void loop() {
  long distance = getDistanceCM();
  int lightLevel = analogRead(LDR_PIN);

  if (distance == -1) {
    Serial.println("No obstacle detected (out of range).");
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
  }
  else if (distance <= DANGER_DISTANCE_CM) {
    Serial.print("DANGER! Obstacle very close: ");
    Serial.print(distance);
    Serial.println(" cm");
    alertUser(100);
  }
  else if (distance <= SAFE_DISTANCE_CM) {
    Serial.print("Caution: Obstacle at ");
    Serial.print(distance);
    Serial.println(" cm");
    alertUser(400);
  }
  else {
    Serial.print("Path clear. Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(VIBRATION_PIN, LOW);
  }

  if (lightLevel < 200) {
    Serial.println("Low light detected around user.");
  }

  delay(200);
}
