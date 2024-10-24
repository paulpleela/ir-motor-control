# IR Motor Control
This Arduino program can be used for controlling two DC motors (or winches) via an IR signal. The motors can be controlled to wind in both directions and at two speed settings.

## Getting Started
### Components used
- Arduino Uno R4 Minima
- Two 5-12V DC Motors
- L293D Motor Shield
- IR Receiver and Remote
- 12V/2A Power Supply
- Jumper Wires

### Wiring diagram
![Wiring Diagram](https://github.com/user-attachments/assets/841539ba-1a62-41a4-807e-327b98a12615)


### Running the program
1. Connect the hardware following the circuit diagram shown above
2. Clone this repository:
    ```bash
    git clone https://github.com/paulpleela/ir-motor-control.git
    ```
3. Set up the Arduino IDE:
    - Open the Arduino IDE and connect the Arduino board to your computer using a USB cable
    - Select the correct board (`Tools > Board > Arduino AVR Boards > Arduino Uno`) and port (`Tools > Port > COMx`)
4. Install the required libraries
    - Go to `Sketch > Include Library > Manage Libraries`
    - Search and install the [IRremote library](https://github.com/Arduino-IRremote/Arduino-IRremote) (version 4.4.1)
6. Press the `->` icon in the top left of the IDE to upload the code to the Arduino board

### Controlling the motors
![Remote Controls](https://github.com/user-attachments/assets/b2a90abd-4b37-4bad-8e87-bf3259e33128)


The motors can be controlled using the remote control buttons as shown in the image above. Pressing buttons A-H will cause the winch motor to reel its cable in or out for a short duration before stopping.

## UML Sequence Diagram

```mermaid
sequenceDiagram
    participant IR_Remote
    participant Arduino
    participant Motor_Control

    IR_Remote ->> Arduino: Send Command
    Arduino ->> Arduino: Decode Command
    Arduino ->> Arduino: Determine which motor to control
    Arduino ->> Motor_Control: Activate motor with specified action
    Motor_Control ->> Motor_Control: Set motor direction and state
    Motor_Control ->> Motor_Control: Adjust PWM for speed control
    Motor_Control ->> Motor_Control: Perform motor action (reel in/out)
    Motor_Control ->> Arduino: Notify action completion
    Arduino ->> Arduino: Wait for 500 ms (debounce)
    Arduino ->> Motor_Control: Stop motors completely
    Arduino ->> Arduino: Prepare to receive next command

```

## Acknowledgements
- Part of this code was taken and modified from the Test Code provided in the [Duinotech L203D Motor Control Shield XC4472 Datasheet](https://www.electusdistribution.com.au/dbdocument/701175/xc4472_manual_14833.pdf)
- Learned from examples provided at the [ENGG1100 Arduino Demo](https://github.com/aburo8/engg1100-demo)
