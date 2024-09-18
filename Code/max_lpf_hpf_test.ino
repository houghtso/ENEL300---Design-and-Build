const int LPF_PIN = A1;
const int HPF_PIN = A0;
const int PWM_PIN = 9;

const unsigned long REPEAT_INTERVAL_MS = 1000;
const unsigned long SAMPLE_DURATION_MS = 50;   // Reduced from 100ms

unsigned long lastRepeatTime = 0;
unsigned long lastLPFTime = 0;
int hpfMax = 0;
int lpfMax = 0;

void setup() {
  Serial.begin(9600);
  pinMode(HPF_PIN, INPUT);
  pinMode(LPF_PIN, INPUT);
  pinMode(PWM_PIN, OUTPUT);
  
  Serial.println("Setup completed");
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastRepeatTime >= REPEAT_INTERVAL_MS) {
    Serial.print("Entering repeat interval\n");
    lastRepeatTime = currentTime;
    lastLPFTime = currentTime;

    // Sample HPF
    unsigned long startTime = currentTime;
    while (currentTime - startTime < SAMPLE_DURATION_MS) {
      currentTime = millis();
      
      int hpfCurrentValue = analogRead(HPF_PIN);
      hpfMax = max(hpfMax, hpfCurrentValue);
    }

    // Sample LPF
    while (currentTime - lastLPFTime >= SAMPLE_DURATION_MS) {
      currentTime = millis();
      
      int lpfCurrentValue = analogRead(LPF_PIN);
      lpfMax = max(lpfMax, lpfCurrentValue);
    }

    Serial.print("End of Sampling\n");

    // Print values as voltages
    float voltageHpf = map(hpfMax, 0, 1023, 0, 5.0);
    float voltageLpf = map(lpfMax, 0, 1023, 0, 5.0);

    Serial.print("HPF Maximum value (voltage): ");
    Serial.println(voltageHpf);

    Serial.print("LPF Maximum value (voltage): ");
    Serial.println(voltageLpf);

    Serial.print("Generating PWM\n");
    analogWrite(PWM_PIN, 128);
    Serial.println("PWM Generated");
  }
}
