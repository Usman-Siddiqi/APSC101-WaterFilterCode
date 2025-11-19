#include <AFMotor.h>

// Define the pins the push buttons and limit switch are connected to
//cf pump is actually supposed to be d pump
#define emergencyStopButton A2
#define startButton A3
#define startTurbiditySensor A0
#define endTurbiditySensor A1
#define ppump 53//Moves clean to clean tank

#define fastSpeedImpeller 80
#define slowSpeedImpeller 175

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
  DCmotor1.setSpeed(0);//Runs the mixing bar
  DCmotor2.setSpeed(fastSpeedImpeller);//Runs impeller
  digitalWrite(ppump, LOW);

  DCmotor1.run(BACKWARD);
  if(wait(1000))//10 secs
    return true;
  DCmotor2.setSpeed(slowSpeedImpeller);
  if(wait(1000))//180 secs
    return true;
  DCmotor2.setSpeed(0);
  DCmotor1.setSpeed(150);
  if(wait(350))
    return true;
  DCmotor1.setSpeed(0);

  return false;
}

bool wait(int time){//time is in 10 miliseconds. This waits and returns ture if emergency stop button is pressed
  for(int i = 0; i < time; i++){
    bool emergencyStopButtonState = digitalRead(emergencyStopButton);
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
    float initial_turbidity = read_turbidity(startTurbiditySensor);

    everything_to_coagulation();
    if(wait(3000))//30 second delay  TODO MAKE THIS 3000
      break;

    if(coagulation())
      break;
    if(wait(3000))//30 second delay
      break;

    coagulation_to_clean();
    if(wait(3000))//30 second delay
      break;
      
    float final_turbidity = read_turbidity(endTurbiditySensor);
    Serial.print("Voltage: ");
    Serial.println(initial_turbidity - final_turbidity);
  }
  stop_all();
}
