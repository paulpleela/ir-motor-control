  #include <IRremote.hpp>

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
  #define IR_RECEIVE_PIN 9

  // Motor speeds [0, 255]
  #define NO_SPEED 0
  #define LOW_SPEED 40
  #define HIGH_SPEED 60

  void setup() {
    Serial.begin(9600);

    // Ensure motors are initially released
    motor(1, RELEASE, NO_SPEED);
    motor(2, RELEASE, NO_SPEED);

    // Start IR receiver
    IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  }

  void loop() {
    if (IrReceiver.decode()) {
      // Process received IR signals
      switch(IrReceiver.decodedIRData.command) {
        // Motor 1
        case 0: // POWER button
          motor(1, REEL_IN, HIGH_SPEED);
          break;
        case 4: // BACK button
          motor(1, REEL_OUT, HIGH_SPEED);
          break;
        case 12: // 0 button
          motor(1, REEL_IN, LOW_SPEED);
          break;
        case 16: // 1 button
          motor(1, REEL_OUT, LOW_SPEED);
          break;
        
        // Motor 2 
        case 2: // FUNC/STOP button
          motor(2, REEL_IN, HIGH_SPEED);
          break;
        case 6: // NEXT button
          motor(2, REEL_OUT, HIGH_SPEED);
          break;
        case 14: // ST/REPT button
          motor(2, REEL_IN, LOW_SPEED);
          break;
        case 18: // 3 button
          motor(2, REEL_OUT, LOW_SPEED);
          break;

        // Stop all
        case 24: // 7 button
        case 25: // 8 button
        case 26: // 9 button
          motor(1, RELEASE, NO_SPEED);
          motor(2, RELEASE, NO_SPEED);
          break;

        default:
          break; // Do nothing
      }

      // Stop motors after button release
      delay(500);
      motor(1, RELEASE, NO_SPEED);
      motor(2, RELEASE, NO_SPEED);
      
      IrReceiver.resume(); // Receive the next value
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
      case REEL_OUT:
        setMotor(motorA, HIGH, speed);
        setMotor(motorB, LOW, -1);
        break;
      case REEL_IN:
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
      return;
    }

    // Update shift register for direction control
    shiftWrite(output, state);

    // Set PWM if speed is valid and within the range
    if (speed >= 0 && speed <= 255) {
      analogWrite(motorPWM, speed);
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
