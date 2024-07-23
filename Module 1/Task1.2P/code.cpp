// Define the pin for the button
const uint8_t BTN_PIN = 2;
// Define the pin for the LED
const uint8_t LED_PIN = 13;

// Variable to store the current state of the LED
uint8_t ledState = LOW;

void handleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  Serial.print("LED state: ");
  Serial.println(ledState);
}

void setup() {
  // Set the button pin as input with an internal pull-up resistor
  pinMode(BTN_PIN, INPUT_PULLUP);
  // Set the LED pin as output
  pinMode(LED_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BTN_PIN), handleLED, CHANGE);

  // Initialize serial communication at a baud rate of 9600
  Serial.begin(9600);
}

void loop() {}
