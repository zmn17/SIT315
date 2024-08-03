// Define the pins for sensors and LEDs
const int soil = 7;
const int pir = 3;
const int switchPin = 2;

const int tmp = A5;

// LEDs
const int pirLed = 6;
const int soilLed = 5;
const int switchLed = 4;
const int piezo = 10;

// Variables for storing LEDs' states
volatile bool pirLedState = false;
volatile bool soilLedState = false;
volatile bool switchLedState = false;

// Variable for Timer Delay
int x = 2;

void starttimer(){
  // clear timer counter
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  // set the timer compare value for 2ms
  OCR1A =  31249;
  
  // set the prescaler to 1024
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
  
  // enable the compare match mode on register A
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect){
  // Print the current LED timer value in milliseconds
  Serial.print("Timer: ");
  Serial.print(x);
  Serial.println(" milliseconds");
  Serial.println();
  
  int analogValue = analogRead(tmp);
  float voltage = analogValue * (5.0 / 1023.0);
  float tmpC = (voltage - 0.5) * 100.0;
  
  if(tmpC < 30){
    // Toggle the timer piezo state
  	digitalWrite(piezo, digitalRead(piezo) ^ 1);
    Serial.println("Piezo activated");
  }
  Serial.print("Temperature: ");
  Serial.print(tmpC);
  Serial.println(" °C");
  Serial.println();
  
  // Increment the timer LED delay
  x = x + 2;
}

// ISR for pin change interruptions on PCINT0 (port B), 
// PCINT1 (port C), PCINT2 (port D)
ISR(PCINT2_vect){
  // check the soil moisture sensor state
  if(digitalRead(soil) == HIGH){
    soilLedState = !soilLedState;
    digitalWrite(soilLed, soilLedState);
	Serial.println("Soil moisture detected.");
  }
  
  // check the PIR sensor state
  if(digitalRead(pir) == HIGH){
    pirLedState = !pirLedState;
    digitalWrite(pirLed, pirLedState);
    Serial.println("Motion detected");
  }
  
  // check the switch state
  if(digitalRead(switchPin) == LOW){
    switchLedState = !switchLedState;
    digitalWrite(switchLed, switchLedState);
    Serial.println("Slide switch activated");
  }
}


void setup()
{
  // set up the LED and sensors pins as output and input
  pinMode(pirLed, OUTPUT);
  pinMode(soilLed, OUTPUT);
  pinMode(switchLed, OUTPUT);
  pinMode(piezo, OUTPUT);
  
  pinMode(soil, INPUT);
  pinMode(pir, INPUT);
  pinMode(switchPin, INPUT_PULLUP);
  
  // Initialize the serial communication
  Serial.begin(9600);
  
  starttimer();
  
  // disable interrupt
  noInterrupts();
  
  // Enable pin change interrupts for the relevant pins
  PCICR |= (1 << PCIE2); // Enable pin change interrupt for PORTD (PCINT16 to PCINT23)
  
  // enable pin change interrupts for pins
  // 7 - soil,3 - pir and 12 - switch
  // PCINT23, PCINT19, PCINT18
  PCMSK2 |= (1 << PCINT23);
  PCMSK2 |= (1 << PCINT19);
  PCMSK2 |= (1 << PCINT18);
  
  // enable global interrupts
  interrupts();
}

void loop()
{
  
}