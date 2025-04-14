#include <LiquidCrystal_I2C.h>

// Initialize the LCD (I2C address: 0x27, 16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define TRIG_PIN 9
#define ECHO_PIN 8
#define BUZZER_PIN 7
#define BULB_PIN 6
#define ADD_BUTTON_PIN 5
#define RESET_BUTTON_PIN 4

int distanceLimit = 50; // Distance input in mm

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BULB_PIN, OUTPUT);
  pinMode(ADD_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Set Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distanceLimit);
  lcd.print(" mm");
}

void loop() {
  // Button handling
  if (digitalRead(ADD_BUTTON_PIN) == LOW) {
    distanceLimit += 10; // Increment by 10 mm
    delay(200); // Debounce delay
    updateDisplay();
  }

  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    distanceLimit = 50; // Reset to 50 mm
    delay(200); // Debounce delay
    updateDisplay();
  }

  // Ultrasonic distance measurement
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  int distanceMeasured = duration * 0.034 / 2;

  lcd.setCursor(0, 0);
  lcd.print("Measured: ");
  lcd.print(distanceMeasured);
  lcd.print(" mm   ");

  // Compare measured distance with limit
  if (distanceMeasured > distanceLimit && distanceLimit > 0) {
    digitalWrite(BUZZER_PIN, HIGH); // Beep
    digitalWrite(BULB_PIN, HIGH);  // Light up bulb
    delay(200); // Beep duration
    digitalWrite(BUZZER_PIN, LOW); // Stop beep
    digitalWrite(BULB_PIN, LOW);   // Turn off bulb
    delay(200); // Pause between beeps
  }
  delay(100); // Small delay for stability
}

void updateDisplay() {
  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear previous text
  lcd.setCursor(0, 1);
  lcd.print(distanceLimit);
  lcd.print(" mm");
}

