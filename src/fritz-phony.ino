/*
 * Project fritz-phony
 */

#include "LiquidCrystal.h"

LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);

void setup() {
    lcd.begin(16,2);
    // Print a message to the LCD.
    lcd.print("Fritz Phony");
    lcd.setCursor(0, 1);
    lcd.print("Starting...");
    delay(500);
    lcd.noDisplay();
    delay(500);
    lcd.display();
}

void loop() {
  // The core of your code will likely live here.

}