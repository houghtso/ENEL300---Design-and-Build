#include <Wire.h>
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>

// Define the LCD address
const int lcdAddress = 0x27;  // or 0x3F, depending on the backpack
bool levelOne = true;
int score = 0;
// Create an instance of the LCD class
hd44780_I2Cexp lcd(lcdAddress);

void setup() {
  // Initialize the LCD
  lcd.begin(20, 4);
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

  // Calculate the middle row
  int freqIn = 2;  // Assuming 4 rows total

  // Write "_" character one pixel to the right every 25ms
  static int xPosition = 0;  // Start at the beginning of the first column
  static int goalHeight = 2;
  static unsigned long lastUpdate = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= 5) {
    for (int i = 0; i <= xPosition; i++) {
      lcd.setCursor(i, freqIn);
      lcd.print("-");
    }

    // Increment xPosition
    xPosition++;
    // Wrap around to the beginning if we reach the end of the screen
    if (xPosition >= 20) {
      lcd.clear();
      if (freqIn == goalHeight) {
        score = 1;
        lcd.setCursor(2, 1);
        lcd.print("Level Complete!");
        lcd.setCursor(4, 2);
        lcd.print("Score: ");
        lcd.print(score);
        lcd.setCursor(12, 2);
        lcd.print("/10");
        delay(200);
        levelOne = false;
      } else {
        xPosition = 0;
        score = 0;
        failScreen();
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

  // Calculate the middle row
  int freqIn = 3;  // Assuming 4 rows total

  // Write "_" character one pixel to the right every 25ms
  static int xPosition = 0;  // Start at the beginning of the first column
  static int goalHeight = 0;
  static unsigned long lastUpdate = 0;
  unsigned long currentTime = millis();
  if (currentTime - lastUpdate >= 5) {
    for (int i = 0; i <= xPosition; i++) {
      lcd.setCursor(i, freqIn);
      lcd.print("-");
    }

    // Increment xPosition
    xPosition++;
    // Wrap around to the beginning if we reach the end of the screen
    if (xPosition >= 20) {
      lcd.clear();
      if (freqIn == goalHeight) {
        levelTwoWin();
      } else {
        xPosition = 0;
        score = 1;
        failScreen();
      }
    }
    // Update last update time
    lastUpdate = currentTime;
  }
}

void levelTwoWin() {
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("Level Complete!");
  lcd.setCursor(4, 2);
  lcd.print("Score: 2/10 ");
  delay(200);
}

void failScreen() {
  lcd.clear();
  lcd.setCursor(4, 1);
  lcd.print("Fail, loser");
  lcd.setCursor(5, 2);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.setCursor(10, 2);
  lcd.print("/10");
  delay(600);
  lcd.clear();  // Clear the LCD
}


void loop() {
  if (levelOne) {
    levelOnePlay();
  } else {
    levelTwoPlay();
  }
  delay(1000);  // Wait for 1 second between full clears
}
