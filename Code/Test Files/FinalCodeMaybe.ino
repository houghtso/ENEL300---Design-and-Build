#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

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

const int lcdAddress = 0x27;  // or 0x3F, depending on the backpack
bool levelOne = true;
bool levelTwo = false;
bool levelThree = false;
int score = 0;

hd44780_I2Cexp lcd(lcdAddress);

void setup() {
  pinMode(HPF, INPUT);
  pinMode(LPF, INPUT);
  pinMode(PWM_PIN, OUTPUT);  // Set PWM pin as output
  Serial.begin(9600);
  lcd.begin(20, 4);
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
  }
}

int scaled(int value) {
  return map(value, 0, MAX_ANALOG_READ, 0, 3);
}

void generatePWM() {
  // Generate PWM signal on PWM pin
  analogWrite(PWM_PIN, PWM_VALUE);  // Duty cycle of 50% (128 out of 255)
}

void levelOnePlay() {
  lcd.clear();

  // Draw rectangles (keep existing code)
  lcd.setCursor(18, 0);   // Set cursor to far right top corner
  lcd.write((char)0xFF);  // Draw top left pixel of rectangle
  lcd.setCursor(19, 0);   // Move cursor to right
  lcd.write((char)0xFF);  // Draw top right pixel of rectangle
  lcd.setCursor(18, 1);   // Move cursor down
  lcd.write((char)0xFF);  // Draw bottom left pixel of rectangle
  lcd.setCursor(19, 1);   // Move cursor to right
  lcd.write((char)0xFF);  // Draw bottom right pixel of rectangle

  lcd.setCursor(18, 3);   // Set cursor to bottom right corner
  lcd.write((char)0xFF);  // Draw left pixel of rectangle
  lcd.setCursor(19, 3);   // Move cursor to right
  lcd.write((char)0xFF);  // Draw right pixel of rectangle

  // Write "_" character one pixel to the right every 25ms
  static int xPosition = 0;  // Start at the beginning of the first column
  static int goalHeight = 2;
  static unsigned long lastUpdate = 0;
  static int heights[20];  // Array to store the heights of each "-" character
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= 5) {
    heights[xPosition] = scaledValue;  // Store the current scaledValue
    for (int i = 0; i <= xPosition; i++) {
      lcd.setCursor(i, heights[i]);
      lcd.print("-");
    }

    // Increment xPosition
    xPosition++;
    // Wrap around to the beginning if we reach the end of the screen
    if (xPosition >= 20) {
      lcd.clear();
      if (heights[xPosition - 1] == goalHeight) {
        score++;
        WinScreen(score);
        levelOne = false;
        levelTwo = true;
      } else {
        xPosition = 0;
        score = 0;
        failScreen(score);
      }
    }
    // Update last update time
    lastUpdate = currentTime;
  }
}

void levelTwoPlay() {
  lcd.clear();
  for (int i = 1; i <= 3; i++) {
    lcd.setCursor(18, i);   // Set cursor to two pixels from the right edge
    lcd.write((char)0xFF);  // Draw left pixel of rectangle
    lcd.setCursor(19, i);   // Move cursor to right
    lcd.write((char)0xFF);  // Draw right pixel of rectangle
  }

  // Write "_" character one pixel to the right every 25ms
  static int xPosition = 0;  // Start at the beginning of the first column
  static int goalHeight = 0;
  static unsigned long lastUpdate = 0;
  static int heights[20];  // Array to store the heights of each "-" character
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= 5) {
    heights[xPosition] = scaledValue;  // Store the current scaledValue
    for (int i = 0; i <= xPosition; i++) {
      lcd.setCursor(i, heights[i]);
      lcd.print("-");
    }

    // Increment xPosition
    xPosition++;
    // Wrap around to the beginning if we reach the end of the screen
    if (xPosition >= 20) {
      lcd.clear();
      if (heights[xPosition - 1] == goalHeight) {
        score++;
        WinScreen(score);
        levelTwo = false;
        levelThree = true;
      } else {
        xPosition = 0;
        failScreen(score);
      }
    }
    // Update last update time
    lastUpdate = currentTime;
  }
}

void WinScreen(int number) {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Level Complete!");
  lcd.setCursor(4, 2);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print("/10");
  delay(200);
}

void levelThreePlay() {
  lcd.clear();

  // Draw rectangles
  lcd.setCursor(18, 0);   // Set cursor to far right top corner
  lcd.write((char)0xFF);  // Draw top left pixel of rectangle
  lcd.setCursor(19, 0);   // Move cursor to right
  lcd.write((char)0xFF);  // Draw top right pixel of rectangle

  lcd.setCursor(18, 2);   // Set cursor to middle left corner
  lcd.write((char)0xFF);  // Draw left pixel of rectangle
  lcd.setCursor(19, 2);   // Move cursor to right
  lcd.write((char)0xFF);  // Draw right pixel of rectangle
  lcd.setCursor(18, 3);   // Move cursor down
  lcd.write((char)0xFF);  // Draw bottom left pixel of rectangle
  lcd.setCursor(19, 3);   // Move cursor to right
  lcd.write((char)0xFF);  // Draw bottom right pixel of rectangle

  // Write "_" character one pixel to the right every 25ms
  static int xPosition = 0;  // Start at the beginning of the first column
  static int goalHeight = 1;
  static unsigned long lastUpdate = 0;
  static int heights[20];  // Array to store the heights of each "-" character
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= 5) {
    heights[xPosition] = scaledValue;  // Store the current scaledValue
    for (int i = 0; i <= xPosition; i++) {
      lcd.setCursor(i, heights[i]);
      lcd.print("-");
    }

    // Increment xPosition
    xPosition++;
    // Wrap around to the beginning if we reach the end of the screen
    if (xPosition >= 20) {
      lcd.clear();
      if (heights[xPosition - 1] == goalHeight) {
        score++;
        levelThree = false;
        WinScreen(score);
      } else {
        xPosition = 0;
        failScreen(score);
      }
    }
    // Update last update time
    lastUpdate = currentTime;
  }
}

void failScreen(int number) {
  lcd.clear();
  lcd.setCursor(4, 1);  // Move the cursor to the second row
  lcd.print("Fail, loser");
  lcd.setCursor(3, 2);  // Move the cursor to the third row
  lcd.print("Score: ");
  lcd.print(number);
  lcd.print("/10");
  delay(600);
  lcd.clear();  // Clear the LCD
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
  if (levelOne) {
    levelOnePlay();
  } else if (levelTwo) {
    levelTwoPlay();
  } else if (levelThree) {
    levelThreePlay();
  }
  delay(1000);  // Wait for 1 second between full clears
}
