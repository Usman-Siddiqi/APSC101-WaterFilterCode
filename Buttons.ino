#include <AFMotor.h>

// Define the pins the push buttons and limit switch are connected to
#define pushButtonSlow A2
#define pushButtonFast A3

// Connect the DC motor to M1 on the motor control board
AF_DCMotor DCmotor(2);

// Define variables to store the states of the push buttons
bool pushButtonStateSlow = HIGH;
bool pushButtonStateFast = HIGH;

int speed;
int direction;//FORWARD 2 BACKWARD 1

void setup() {
  // Set up serial communication to print things to the serial monitors
  Serial.begin (9600);

  // Set the speed of the DC motor. The speed input can be 0-255.
  DCmotor.setSpeed(125);

  // Start the motor off not moving
  DCmotor.run(RELEASE);

  // Configure the pins to to be input pins
  pinMode(pushButtonSlow, INPUT_PULLUP);
  pinMode(pushButtonFast, INPUT_PULLUP);

  speed = 25;
  direction = 1;
}

void loop() {
  // Read the value of the pin as either HIGH or LOW
  pushButtonStateSlow=digitalRead(pushButtonSlow);
  pushButtonStateFast=digitalRead(pushButtonFast);

  // Turn the motor slowly or quickly depending on which button is pressed
  if (pushButtonStateSlow==LOW){
    //Serial.println("Slow");
    DCmotor.run(direction);
    DCmotor.setSpeed(0);
    speed = 25;
    if(direction == 1){
      direction = 2;
    }
    else{
      direction = 1;
    }

  }
  else if (pushButtonStateFast==LOW){
    //Serial.println("Fast");
    DCmotor.run(direction);
    DCmotor.setSpeed(speed);
    Serial.println(speed);
    speed = speed + 5;
  }


  // Wait 50 milliseconds before starting the void loop over again
  delay(50);
}
  
