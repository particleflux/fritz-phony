/*
 * Project fritz-phony
 *
 *
 * testing:
 * nc -lp 1012 localhost
 *
 *
 */

#include "LiquidCrystal.h"


#define BACKLIGHT_PIN D6
#define FRITZBOX_HOST "fritz.box"

typedef enum {
    UNKNOWN,
    CALL,
    RING,
    CONNECT,
    DISCONNECT
} MsgType;

LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);
TCPClient client;
char buffer[255];
char line1[16];
char line2[16];
int size = 0;


void setup() {
    pinMode(BACKLIGHT_PIN, OUTPUT);

    lcd.begin(16,2);

    // backlight ON
    digitalWrite(BACKLIGHT_PIN, LOW);

    // Print a message to the LCD.
    lcd.print("Fritz Phony");
    lcd.setCursor(0, 1);
    lcd.print("Starting...");

    delay(2000);
    // backlight OFF
    digitalWrite(BACKLIGHT_PIN, HIGH);
    lcd.noDisplay();

}

void loop() {
  if (!client.connected()) {
      client.connect(FRITZBOX_HOST, 1012);
  }

  if (client.available()) {
      delay(100); // ensure all data is available
      size = client.read((uint8_t *) buffer, sizeof buffer);
      buffer[size] = '\0';
      Serial.println("got data:");
      Serial.println(buffer);
      parseMessage();
  }
}

void parseMessage() {
    MsgType type = UNKNOWN;
    int i = 0, seconds = 0;
    char *part = NULL, *date = NULL;
    char *parameters[4];


    part = strtok(buffer, ";");
    while (part != NULL) {
        Serial.printlnf("part: %s", part);
        switch (i) {
            case 0:
                date = part;
                break;

            case 1:
                if (strcmp(part, "CALL") == 0) {
                    type = CALL;
                } else if (strcmp(part, "RING") == 0) {
                    type = RING;
                } else if (strcmp(part, "CONNECT") == 0) {
                    type = CONNECT;
                } else if (strcmp(part, "DISCONNECT") == 0) {
                    type = DISCONNECT;
                }
                break;

            default:
                if (type != UNKNOWN) {
                    parameters[i - 2] = part;
                }
                break;
        }

        ++i;
        part = strtok(NULL, ";");
    }

    // handle shit
    switch (type) {
        case CALL:
            sprintf(line1, "OUT %s", date);
            sprintf(line2, "%s", parameters[2]);
            break;
        case RING:
            sprintf(line1, "IN %s", date);
            sprintf(line2, "%s", parameters[1]);
            break;
        case CONNECT:
            break;
        case DISCONNECT:
            seconds = atoi(parameters[1]);
            sprintf(line1, "duration %u:%u", seconds / 60, seconds % 60);
            line2[0] = '\0';
            //sprintf(line2, "%s", parameters[1]);
            break;
        default:
            break;
    }

    display();

}

void display() {
    digitalWrite(BACKLIGHT_PIN, LOW);
    lcd.display();
    lcd.clear();
    lcd.print(line1);
    lcd.setCursor(0, 1);
    lcd.print(line2);
}