#include <AFMotor.h>

// Define the pins the push buttons and limit switch are connected to
#define emergencyStopButton A2
#define startButton A3
#define ppump 53//Moves clean to clean tank

// Connect the DC motor to M1 on the motor control board
AF_DCMotor DCmotor1(2);//Runs threaded rod
AF_DCMotor DCmotor2(1);//Runs impeller
AF_DCMotor pump(4);//Moves dirty to coagulation
AF_DCMotor cfpump(3);//slurry to coagulation tank

void dirty_to_coagulation(){
  cfpump.setSpeed(0);
  pump.setSpeed(255);//Moving the water for this task
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, LOW);
}

void slurry_to_coagulation(){
  cfpump.setSpeed(255);//Moving the water for this task
  pump.setSpeed(0);
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, LOW);
}

void coagulation(){
  cfpump.setSpeed(0);
  pump.setSpeed(0);
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, LOW);
}

void coagulation_to_clean(){
  cfpump.setSpeed(0);
  pump.setSpeed(0);
  DCmotor1.setSpeed(167);//Runs the mixing bar
  DCmotor2.setSpeed(167);//Runs impeller
  digitalWrite(ppump, HIGH);//Moving the water for this task

  int mixing_loops = 1;
  for(int i = 0; i < mixing_loops; i++){
      DCmotor1.run(FORWARD);
      wait(100);
      DCmotor1.run(BACKWARD);
      wait(100);
  }
}

bool wait(int time){//time is in 10 miliseconds. This waits and returns ture if emergency stop button is pressed
  for(int i = 0; i < time; i++){
    bool emergencyStopButtonState = digitalRead(emergencyStopButton);
    if(emergencyStopButtonState)
      return true;
    
    delay(10);
  }
  return false;
}

void setup() {
  // Set up serial communication to print things to the serial monitors
  Serial.begin (9600);
  pinMode(emergencyStopButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(ppump, OUTPUT);

  pump.run(FORWARD);
  cfpump.run(FORWARD);
  DCmotor1.run(FORWARD);
  DCmotor2.run(FORWARD);

  // Set the speed of the Pumps. The speed input can be 0-255.
  cfpump.setSpeed(0);
  pump.setSpeed(0);
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, LOW);
}

void loop() {
  bool startButtonState = digitalRead(startButton);
  while(startButtonState){
    dirty_to_coagulation();
    if(wait(3000))//30 second delay
      break;
    slurry_to_coagulation();
    if(wait(3000))//30 second delay
      break;
    coagulation();
    if(wait(3000))//30 second delay
      break;
    coagulation_to_clean();
    if(wait(10))//30 second delay
      break;
  }
}
