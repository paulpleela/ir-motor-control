#include <IRremote.h>

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
#define REEL_IN 1
#define REEL_OUT 2
#define BRAKE 3
#define RELEASE 4

// IR receiver pin
#define RECV_PIN 9
IRrecv irReceiver(RECV_PIN);

// Motor speeds [0, 255]
#define NO_SPEED 0
#define LOW_SPEED 70
#define MEDIUM_SPEED 150
#define HIGH_SPEED 230

void setup() {
  Serial.begin(9600);
  irReceiver.enableIRIn(); // Start the IR Receiver
}

void loop() {
  if (irReceiver.decode()) {
    // Process received IR signals
    switch(irReceiver.decodedIRData.command) {
      // Motor 1
      case 0: // POWER button
        motor(1, REEL_OUT, LOW_SPEED);
        break;
      case 4: // BACK button
        motor(1, REEL_OUT, MEDIUM_SPEED);
        break;
      case 8: // DOWN button
        motor(1, REEL_OUT, HIGH_SPEED);
        break;
      case 2: // FUNC/STOP button
        motor(1, REEL_IN, LOW_SPEED);
        break;
      case 6: // NEXT button
        motor(1, REEL_IN, MEDIUM_SPEED);
        break;
      case 10: // UP button
        motor(1, REEL_IN, HIGH_SPEED);
        break;
      case 1: // VOL+ button
      case 5: // PLAY/PAUSE button
      case 9: // VOL- button
        motor(1, RELEASE, NO_SPEED);
        break;
      
      // Motor 2 
      case 16: // 1 button
        motor(2, REEL_OUT, LOW_SPEED);
        break;
      case 20: // 4 button
        motor(2, REEL_OUT, MEDIUM_SPEED);
        break;
      case 24: // 7 button
        motor(2, REEL_OUT, HIGH_SPEED);
        break;
      case 18: // 3 button
        motor(2, REEL_IN, LOW_SPEED);
        break;
      case 22: // 6 button
        motor(2, REEL_IN, MEDIUM_SPEED);
        break;
      case 26: // 9 button
        motor(2, REEL_IN, HIGH_SPEED);
        break;
      case 17: // 2 button
      case 21: // 5 button
      case 25: // 8 button
        motor(2, RELEASE, NO_SPEED);
        break;

      default:
        break; // Do nothing
    }
    irReceiver.resume(); // Receive the next value
  }
}

//////////////////////////////////////////////////////////////////////////////////

void motor(int motorNumber, int command, int speed) {
  int motorA, motorB;

  if (motorNumber == 1) {
    motorA = MOTOR1_A;
    motorB = MOTOR1_B;
  } else if (motorNumber == 2) {
    motorA = MOTOR2_A;
    motorB = MOTOR2_B;
  } else {
    return; // Invalid motor, do nothing
  }

  switch (command) {
    case REEL_IN:
      setMotor(motorA, HIGH, speed);
      setMotor(motorB, LOW, -1);
      break;
    case REEL_OUT:
      setMotor(motorA, LOW, speed);
      setMotor(motorB, HIGH, -1);
      break;
    case BRAKE:
      setMotor(motorA, LOW, 255);
      setMotor(motorB, LOW, -1);
      break;
    case RELEASE:
      setMotor(motorA, LOW, 0);
      setMotor(motorB, LOW, -1);
      break;
  }
}

// Function to set motor speed and direction via PWM
void setMotor(int output, int state, int speed) {
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
  delayMicroseconds(5); 
  digitalWrite(MOTORLATCH, HIGH);
  delayMicroseconds(5);
  digitalWrite(MOTORLATCH, LOW);
}
