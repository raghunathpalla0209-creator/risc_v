/**
 * @brief Simple UART echo responder
 * Listens for "test" and replies with a message
 * Test file to test the UART application, Serial trasmits if valid message is recived from host
 */


/* This Application runs in Arduino Platform which acts as Target Hardware */

String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
}

/**
 * Robust UART command handler
 */


void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    // Ignore noise: if the string is empty or just 1-2 chars of junk, do nothing
    if (cmd.length() < 4) { 
        return; 
    }

    if (cmd == "test") {
      Serial.println("hello from arduino!!");
    } else {
      Serial.println("unknown command");
    }
  }
}

/**
 * @brief Serial event handler (called automatically)
 */
void serialEvent() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();

        if (inChar == '\n') {
            stringComplete = true;
        } else {
            inputString += inChar;
        }
    }
}
