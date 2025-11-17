#include <AFMotor.h>

// Define the pins the push buttons and limit switch are connected to
#define pushButtonSlow A2
#define pushButtonFast A3

bool pushButtonStateSlow = HIGH;
bool pushButtonStateFast = HIGH;

// Connect the DC motor to M1 on the motor control board
AF_DCMotor DCmotor(2);
AF_DCMotor pump(4);
AF_DCMotor ppump(1);
AF_DCMotor cfpump(3);

void setup() {
  // Set up serial communication to print things to the serial monitors
  Serial.begin (9600);
  pinMode(pushButtonSlow, INPUT_PULLUP);
  pinMode(pushButtonFast, INPUT_PULLUP);

  // Set the speed of the Pumps. The speed input can be 0-255.
  pump.setSpeed(255);
  ppump.setSpeed(255);
  cfpump.setSpeed(255);

  // Start the pumps forward
  ppump.run(FORWARD);
  cfpump.run(FORWARD);
  pump.run(FORWARD);

  // Configure the pins to to be input pins
  pinMode(pushButtonSlow, INPUT_PULLUP);
  pinMode(pushButtonFast, INPUT_PULLUP);
}

void loop() {
  pushButtonStateSlow=digitalRead(pushButtonSlow);
  pushButtonStateFast=digitalRead(pushButtonFast);

  // Turn the motor slowly or quickly depending on which button is pressed
  if (pushButtonStateSlow==LOW){
    pump.setSpeed(0);
    ppump.setSpeed(0);
    cfpump.setSpeed(0);

  }
  else if (pushButtonStateFast==LOW){
    pump.setSpeed(255);
    ppump.setSpeed(255);
    cfpump.setSpeed(255);
  }


  // Wait 50 milliseconds before starting the void loop over again
  delay(50);
}
