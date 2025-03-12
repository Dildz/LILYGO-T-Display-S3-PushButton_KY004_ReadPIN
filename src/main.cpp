/*********************************************************************************************************
* LILYGO T-Display-S3 KY-004 Push Button Module Project
*
* This code reads the digital signal from the KY-004 module and demonstrates debouncing of the
   push button signal.
* The debounced signal is displayed on the built-in screen of the LilyGO T-Display-S3 using the
   TFT_eSPI library.
* The screen is only updated if there is a change in the button state.

* Pin Connections:
*  - KY-004 VCC  -> 3.3V
*  - KY-004 GND  -> GND
*  - KY-004 D0   -> GPIO1
*
* KY-004 Specifications:
*  - Protocol: Digital
*  - Operating Voltage: 3.3V to 5.5V
*  - Operating Temperature: -25°C to 105°C
*
* Notes:
*  - Signal on GPIO1 is active low, this means that when the button is pressed there is 0V on the pin.
      If the button is released there is 3.3V on the pin. We start with button state HIGH.
   - Try not to touch the 4 button pins on the underside of the module as this will cause the button to
      switch. Mount or hold the module on the edges when pressing the button.
**********************************************************************************************************/

/*************************************************************
******************* INCLUDES & DEFINITIONS *******************
**************************************************************/

#include <Arduino.h>
#include <TFT_eSPI.h>

// TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

// Pin definitions
const int buttonPin = 1;   // digital pin for push button

// Variables for timing
unsigned long lastDebounceTime = 0; // last time the button state was toggled
const long debounceInterval = 10;   // debounce interval (ms)

// Global variables
int buttonState = HIGH;      // current button state (active low, so default is HIGH)
int lastButtonState = HIGH;  // previous button state
bool redrawRequired = true;  // flag to redraw the screen


/*************************************************************
********************** HELPER FUNCTIONS **********************
**************************************************************/

// Function to draw static elements on the TFT screen
void drawStaticElements() {
  // Clear the screen and set text properties
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Reset cursor to the top-left corner
  tft.setCursor(0, 0);

  // Display header
  tft.println("---------------------------");
  tft.println("KY004 Push Button Module");
  tft.println("---------------------------");

  // Display static labels
  tft.setCursor(0, 70); // position for button state label
  tft.print("Button State:");
}

// Function to update dynamic elements on the TFT screen
void updateDynamicElements() {
  // Update button state
  tft.setCursor(0, 90);      // position for button state value (below the label)
  tft.print("            "); // clear previous value (12 spaces for padding)
  tft.setCursor(0, 90);      // reset cursor
  tft.print(buttonState == HIGH ? "PRESSED" : "RELEASED");
}


/*************************************************************
*********************** MAIN FUNCTIONS ***********************
**************************************************************/

// SETUP
void setup() {
  // Set pin modes
  pinMode(buttonPin, INPUT);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0);                     // adjust rotation (0 & 2 portrait | 1 & 3 landscape)
  tft.fillScreen(TFT_BLACK);              // black background
  tft.setTextFont(2);                     // set the font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // set text colour (white) and background colour (black)

  // Draw static elements once
  drawStaticElements();

  // Initial display update
  redrawRequired = true; // update display
}


// MAIN LOOP
void loop() {
  // Read the current button state
  int reading = digitalRead(buttonPin);

  // Check if the button state has changed (due to noise or pressing)
  if (reading != lastButtonState) {
    // Reset the debounce timer
    lastDebounceTime = millis();
  }

  // Check if the debounce interval has passed
  if ((millis() - lastDebounceTime) > debounceInterval) {
    // If the button state has changed, update the button state
    if (reading != buttonState) {
      buttonState = reading;
      redrawRequired = true; // flag to update the display
    }
  }

  // Save the current reading as the last button state
  lastButtonState = reading;

  // Update the display if the button state has changed
  if (redrawRequired) {
    updateDynamicElements();
    redrawRequired = false; // reset the flag
  }
  delay(1); // slow down the loop to save CPU cycles
}
