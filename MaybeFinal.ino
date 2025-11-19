#include <AFMotor.h>

// Define the pins the push buttons and limit switch are connected to
//cf pump is actually supposed to be d pump
#define emergencyStopButton A2
#define startButton A3
#define startTurbiditySensor A0
#define endTurbiditySensor A1
#define ppump 53//Moves clean to clean tank

#define fastSpeedImpeller 175
#define slowSpeedImpeller 80

// Connect the DC motor to M1 on the motor control board
AF_DCMotor DCmotor1(2);//Runs threaded rod
AF_DCMotor DCmotor2(1);//Runs impeller
AF_DCMotor pump(4);//Moves dirty to coagulation
AF_DCMotor cfpump(3);//slurry to coagulation tank

void everything_to_coagulation(){
  cfpump.setSpeed(255);//Moving the slurry
  pump.setSpeed(255);//Moving the dirty water
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, LOW);
}

void coagulation_to_clean(){
  cfpump.setSpeed(0);
  pump.setSpeed(0);
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, HIGH);//Moving the water for this task
}

bool coagulation(){
  cfpump.setSpeed(0);
  pump.setSpeed(0);
  DCmotor1.setSpeed(0);//Runs the press
  DCmotor2.setSpeed(slowSpeedImpeller);//Runs impeller
  digitalWrite(ppump, LOW);

  DCmotor1.run(BACKWARD);
  if(wait(1800))//18 secs
    return true;
  DCmotor2.setSpeed(fastSpeedImpeller);
  if(wait(1000))
    return true;
  DCmotor2.setSpeed(slowSpeedImpeller);
  if(wait(9000))//90 secs //TODO MAKE IT 18000
    return true;
  DCmotor2.setSpeed(0);
  if(wait(9000))//90 second delay
      return true;  
  DCmotor1.setSpeed(250);
  if(wait(180))
    return true;
  DCmotor1.setSpeed(0);

  return false;
}

bool wait(int time){//time is in 10 miliseconds. This waits and returns ture if emergency stop button is pressed
  for(int i = 0; i < time; i++){
    bool emergencyStopButtonState = digitalRead(emergencyStopButton);
    Serial.println((float(time)-float(i))/100);
    if(emergencyStopButtonState == LOW)
      return true;
    
    delay(10);
  }
  return false;
}

void stop_all(){
  cfpump.setSpeed(0);
  pump.setSpeed(0);
  DCmotor1.setSpeed(0);
  DCmotor2.setSpeed(0);
  digitalWrite(ppump, LOW);
}

float read_turbidity(int a_pin){
  int sensor = analogRead(a_pin);// read the input on analog pin 0
  float voltage = sensor * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V)
  Serial.println("Voltage: ");
  Serial.println(voltage); 
  return voltage;
}

void setup() {
  // Set up serial communication to print things to the serial monitors
  Serial.begin (9600);
  pinMode(emergencyStopButton, INPUT_PULLUP);
  pinMode(startButton, INPUT_PULLUP);
  pinMode(ppump, OUTPUT);

  pump.run(FORWARD);
  cfpump.run(FORWARD);
  DCmotor1.run(BACKWARD);
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
  while(startButtonState == LOW){
    
    Serial.print("Initial Velocity: ");
    float initial_turbidity = read_turbidity(startTurbiditySensor);

    everything_to_coagulation();
    if(wait(9000))//30 second delay  TODO MAKE THIS 9000
      break;

    if(coagulation())
      break;

    coagulation_to_clean();
    if(wait(4500))//30 second delay
      break;
      
    Serial.print("Final ");
    float final_turbidity = read_turbidity(endTurbiditySensor);

    Serial.print("Voltage Difference: ");
    Serial.println(initial_turbidity - final_turbidity);
  }
  stop_all();
  startButtonState = digitalRead(startButton);
}
