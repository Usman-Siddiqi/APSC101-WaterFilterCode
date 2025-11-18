#include <AFMotor.h>

// Define the pins the push buttons and limit switch are connected to
#define pushButtonSlow A2
#define pushButtonFast A3
#define ppump 53//Moves clean to clean tank

bool pushButtonStateSlow = HIGH;
bool pushButtonStateFast = HIGH;

// Connect the DC motor to M1 on the motor control board
AF_DCMotor DCmotor1(2);//Runs threaded rod
AF_DCMotor DCmotor2(1);//Runs impeller
AF_DCMotor pump(4);//Moves dirty to coagulation
AF_DCMotor cfpump(3);//slurry to coagulation tank

void dirty_to_coagulation(){

}

void slurry_to_coagulation(){

}

void coagulation(){

}

void coagulation_to_clean(){

}

void setup() {
  // Set up serial communication to print things to the serial monitors
  Serial.begin (9600);
  pinMode(pushButtonSlow, INPUT_PULLUP);
  pinMode(pushButtonFast, INPUT_PULLUP);
  pinMode(ppump, OUTPUT);

  pump.run(FORWARD);
  cfpump.run(FORWARD);

  // Set the speed of the Pumps. The speed input can be 0-255.
  cfpump.setSpeed(255);
  pump.setSpeed(255);
  digitalWrite(ppump, HIGH);
}

void loop() {
  dirty_to_coagulation();
  delay(10*1000);
  slurry_to_coagulation();
  delay(10*1000);
  coagulation();
  delay(10*1000);
  coagulation_to_clean();
}
