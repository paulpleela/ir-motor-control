#include <SoftwareSerial.h>
#include <RemoteXY.h>

// RemoteXY GUI configuration  
#pragma pack(push, 1)  
uint8_t RemoteXY_CONF[] =   // 37 bytes
  { 255,4,0,0,0,30,0,18,0,0,0,31,1,200,84,1,1,2,0,5,
  15,9,60,60,32,2,26,31,5,130,9,60,60,32,2,26,31 };
  
// This structure defines all the variables and events of the control interface 
struct {
  // Input variables
  int8_t unused1; // from -100 to 100
  int8_t speed1; // from -100 to 100
  int8_t unused2; // from -100 to 100
  int8_t speed2; // from -100 to 100

  uint8_t connect_flag;  // =1 if wire connected, else =0
} RemoteXY;   
#pragma pack(pop)

// Control pins that operate the shift register
#define MOTORLATCH 12
#define MOTORCLK 4
#define MOTORENABLE 7
#define MOTORDATA 8

// Pin definitions for motor direction control
#define MOTOR1_A 2
#define MOTOR1_B 3
#define MOTOR2_A 1
#define MOTOR2_B 4

// PWM pins for motor speed control
#define MOTOR1_PWM 11
#define MOTOR2_PWM 3

// Motor control commands
#define FORWARD 1
#define BACKWARD 2
#define BRAKE 3
#define RELEASE 4

CRemoteXY *remotexy;

void setup() {
  remotexy = new CRemoteXY (
    RemoteXY_CONF_PROGMEM, 
    &RemoteXY, 
    new CRemoteXYStream_SoftSerial (
      A1,     // REMOTEXY_SERIAL_RX
      A0,     // REMOTEXY_SERIAL_TX
      9600    // REMOTEXY_SERIAL_SPEED
    )
  );
}

void loop() {
  remotexy->handler();

  if (RemoteXY.speed1 == 0) {
    motor(1, RELEASE, 0);
  } else if (RemoteXY.speed1 > 0) {
    motor(1, FORWARD, RemoteXY.speed1);
  } else {
    motor(1, BACKWARD, RemoteXY.speed1);
  }

  if (RemoteXY.speed2 == 0) {
    motor(2, RELEASE, 0);
  } else if (RemoteXY.speed2 > 0) {
    motor(2, FORWARD, RemoteXY.speed2);
  } else {
    motor(2, BACKWARD, RemoteXY.speed2);
  }
}

void motor(int motorNumber, int command, int speed) {
  int motorA, motorB;

  if (motorNumber == 1) {
    motorA = MOTOR1_A;
    motorB = MOTOR1_B;
  } else if (motorNumber == 2) {
    motorA = MOTOR2_A;
    motorB = MOTOR2_B;
  } else {
    // Invalid motor, do nothing
    return;
  }

  switch (command) {
    case FORWARD:
      set_motor(motorA, HIGH, speed);
      set_motor(motorB, LOW, -1);
      break;
    case BACKWARD:
      set_motor(motorA, LOW, speed);
      set_motor(motorB, HIGH, -1);
      break;
    case BRAKE:
      set_motor(motorA, LOW, 255);
      set_motor(motorB, LOW, -1);
      break;
    case RELEASE:
      set_motor(motorA, LOW, 0);
      set_motor(motorB, LOW, -1);
      break;
  }
}

// Function to set motor speed and direction via PWM
void set_motor(int output, int state, int speed) {
  int motorPWM;

  // Select PWM pin based on the motor's output pin
  if (output == MOTOR1_A || output == MOTOR1_B) {
    motorPWM = MOTOR1_PWM;
  } else if (output == MOTOR2_A || output == MOTOR2_B) {
    motorPWM = MOTOR2_PWM;
  } else {
    // Invalid case, do nothing
    speed = -3333;
  }

  if (speed != -3333) {
    // Update shift register for direction control
    shiftWrite(output, state);
    // set PWM if it is valid
    if (speed >= 0 && speed <= 255)
    {
      analogWrite(motorPWM, speed);
    }
  }
}

// Function to write the motor's state to the shift register
void shiftWrite(int output, int state) {
  static int latch_copy;
  static int shift_register_initialized = false;

  // Initialise the shift register on first use
  if (!shift_register_initialized) {
    // Set pins for shift register to output
    pinMode(MOTORLATCH, OUTPUT);
    pinMode(MOTORENABLE, OUTPUT);
    pinMode(MOTORDATA, OUTPUT);
    pinMode(MOTORCLK, OUTPUT);
    // Set pins for shift register to LOW by default
    digitalWrite(MOTORDATA, LOW);
    digitalWrite(MOTORLATCH, LOW);
    digitalWrite(MOTORCLK, LOW);
    // Enable the shift register, set Enable pin to LOW
    digitalWrite(MOTORENABLE, LOW);

    // Initialise with all outputs LOW
    latch_copy = 0;

    shift_register_initialized = true;
  }

  // Update the shift register with the new output state
  bitWrite(latch_copy, output, state);
  shiftOut(MOTORDATA, MOTORCLK, MSBFIRST, latch_copy);
  RemoteXY_delay(1);    
  digitalWrite(MOTORLATCH, HIGH);
  RemoteXY_delay(1);    
  digitalWrite(MOTORLATCH, LOW);
}
