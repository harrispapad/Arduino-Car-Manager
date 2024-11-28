#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution for your motor

Stepper myStepper(stepsPerRevolution, 5,6,9,10); 

const int alarmButton = 2;      // Alarm button
const int rightFlashButton = 8; // Right flash button
const int leftFlashButton = 14; // Left flash button

// Define the digital pins that the L298N driver boards are connected to (Arduino Outputs - Drivers Inputs)
const int rightFlashIn_1_IN1 = 3; // Right flash pin
const int rightFlashIn_1_IN2 = 4; // Right flash pin
const int leftFlashIn_1_IN3 = 5;  // Left flash pin
const int leftFlashIn_1_IN4 = 6;  // Left flash pin
const int ENA = 13;               // Enable pin for wiper

//status flags for the buttons
volatile bool rightFlash = false;
volatile bool leftFlash = false;
volatile bool alarm = false;

//status flags for the lights
volatile bool rightFlashLights = false;
volatile bool leftFlashLights = false;
volatile bool alarmLights = false;

// Debounce timers
volatile unsigned long lastAlarmPress = 0;
volatile unsigned long lastRightFlashPress = 0;
volatile unsigned long lastLeftFlashPress = 0;

const unsigned long debounceDelay = 100; // 100ms debounce delay

void setup()
{
  Serial.begin(9600);
  Serial.println("Debugging started"); // Print a debug message
  myStepper.setSpeed(60);

  // Internal timer interrupt
  TCCR1A = 0;               // Clear Timer/Counter Control Register A
  TCCR1B = 0;               // Clear Timer/Counter Control Register B
  TCNT1 = 0;                // Initialize counter value to 0
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  OCR1A = 7812;             // 0.5s
  TIMSK1 |= (1 << OCIE1A);

  pinMode(rightFlashIn_1_IN1, OUTPUT);
  pinMode(rightFlashIn_1_IN2, OUTPUT);
  pinMode(leftFlashIn_1_IN3, OUTPUT);
  pinMode(leftFlashIn_1_IN4, OUTPUT);

  setup_interrupts();
}

void setup_interrupts()
{
  pinMode(alarmButton, INPUT_PULLUP);        // Alarm button
  pinMode(rightFlashButton, INPUT_PULLUP);  // Right flash button
  pinMode(leftFlashButton, INPUT_PULLUP);   // Left flash button

  PCICR |= (1 << PCIE0);                     // Enable PCINT for PORTB
  PCICR |= (1 << PCIE1);                    // Enable PCINT for PORTC
  PCICR |= (1 << PCIE2);                     // Enable PCINT for PORTD

  PCMSK0 |= (1 << PCINT0);                   // Enable interrupt for D8 (PORTB)
  PCMSK1 |= (1 << PCINT8);                   // Enable interrupt for A0 (PORTC)
  PCMSK2 |= (1 << PCINT18);                 // Enable interrupt for D2 (PORTD)

  sei();
  Serial.println("Enabled Interrupts");
}

//wiper code
void loop()
{
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(3000);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(3000);
}

// Alarm button control
ISR(PCINT0_vect)
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastAlarmPress > debounceDelay) {
    alarm = !alarm;
    alarmLights = alarm;
    lastAlarmPress = currentMillis;
    Serial.println("Changed Alarm Status");
  }
}

ISR(PCINT1_vect)
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastRightFlashPress > debounceDelay) {
    rightFlash = !rightFlash;
    rightFlashLights = rightFlash;
    lastRightFlashPress = currentMillis;
    Serial.println("Changed Right Flash Status");
  }
}

ISR(PCINT2_vect)
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastLeftFlashPress > debounceDelay) {
    leftFlash = !leftFlash;
    leftFlashLights = leftFlash;
    lastLeftFlashPress = currentMillis;
    Serial.println("Changed Left Flash Status");
  }
}

ISR(TIMER1_COMPA_vect)
{
  if (alarm) {
    if (!alarmLights)
      turn_on_alarm();
    else
      turn_off_alarm();
    alarmLights = !alarmLights;
  } else if (rightFlash) {
    if (!rightFlashLights)
      turn_on_right_flash();
    else
      turn_off_right_flash();
    rightFlashLights = !rightFlashLights;
  } else if (leftFlash) {
    if (!leftFlashLights)
      turn_on_left_flash();
    else
      turn_off_left_flash();
    leftFlashLights = !leftFlashLights;
  }
}

//placeholder code
void turn_on_alarm() { Serial.println("Alarm ON"); }
void turn_off_alarm() { Serial.println("Alarm OFF"); }
void turn_on_right_flash() { Serial.println("Right Flash ON"); }
void turn_off_right_flash() { Serial.println("Right Flash OFF"); }
void turn_on_left_flash() { Serial.println("Left Flash ON"); }
void turn_off_left_flash() { Serial.println("Left Flash OFF"); }
