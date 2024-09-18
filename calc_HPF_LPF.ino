const int HPF = A0;
const int LPF = A1;
const int SAMPLE_DURATION_MS = 100;
const int SAMPLE_FREQUENCY_HZ = 500;
const int REPEAT_INTERVAL_MS = 1000;

unsigned long lastSampleTime = 0;
unsigned long lastRepeatTime = 0;
int hpfMax = 0;
int lpfMax = 0;

void setup() {
  pinMode(HPF, INPUT);
  pinMode(LPF, INPUT);
  pinMode(9, OUTPUT);  // Set pin 9 as output for PWM
  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();

  // Check if it's time to start a new sample period
  if (currentTime - lastRepeatTime >= REPEAT_INTERVAL_MS) {
    lastRepeatTime = currentTime;
    lastSampleTime = currentTime;

    // Reset max values
    hpfMax = 0;
    lpfMax = 0;

    // Start sampling
    unsigned long startTime = currentTime;

    while (currentTime - startTime < SAMPLE_DURATION_MS) {
      currentTime = millis();

      int hpfcurrentValue = analogRead(HPF);
      int lpfcurrentValue = analogRead(LPF);

      // Update max value if needed
      hpfMax = max(hpfMax, hpfcurrentValue);
      lpfMax = max(lpfMax, lpfcurrentValue);
    }

    Serial.print("Maximum HPF value: ");
    Serial.println(hpfMax);
    Serial.print("Maximum LPF value: ");
    Serial.println(lpfMax);

    // Generate PWM signal on pin 9 at 490 Hz
    analogWrite(9, 128);  // Duty cycle of 50% (128 out of 255)
  }
}