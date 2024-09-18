const int HPF = A0;
const int LPF = A1;
const int SAMPLE_DURATION_MS = 100;
const int SAMPLE_FREQUENCY_HZ = 500;
const int REPEAT_INTERVAL_MS = 1000;
const int PWM_PIN = 9;
const int PWM_VALUE = 128;  // Duty cycle of 50% (128 out of 255)
const int MAX_ANALOG_READ = 1023;

unsigned long lastSampleTime = 0;
unsigned long lastRepeatTime = 0;
int hpfMax = 0;
int lpfMax = 0;
int scaledValue = 0;
unsigned long lastScaledTime = 0;

void setup() {
  pinMode(HPF, INPUT);
  pinMode(LPF, INPUT);
  pinMode(PWM_PIN, OUTPUT);  // Set PWM pin as output
  Serial.begin(9600);
}

void sample() {
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

      int hpfCurrentValue = analogRead(HPF);
      int lpfCurrentValue = analogRead(LPF);

      // Update max value if needed
      hpfMax = max(hpfMax, hpfCurrentValue);
      lpfMax = max(lpfMax, lpfCurrentValue);
    }

    // Serial.print("Maximum HPF value: ");
    // Serial.println(hpfMax);
    // Serial.print("Maximum LPF value: ");
    // Serial.println(lpfMax);
  }
}

int scaled(int value) {
  return map(value, 0, MAX_ANALOG_READ, 0, 4);
}

void generatePWM() {
  // Generate PWM signal on PWM pin
  analogWrite(PWM_PIN, PWM_VALUE);  // Duty cycle of 50% (128 out of 255)
}

void loop() {
  sample();
  unsigned long currentTime = millis();
  if (currentTime - lastScaledTime >= 1000) {
    lastScaledTime = currentTime;
    int currentValue = (lpfMax == 0) ? hpfMax : lpfMax;
    scaledValue = scaled(currentValue);
    Serial.println(scaledValue);
  }
  generatePWM();
}