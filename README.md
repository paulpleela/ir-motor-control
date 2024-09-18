# Bluetooth Motor Control
This Arduino program can be used for controlling the speed and direction of two DC motors (or winches) via a Bluetooth connection and the RemoteXY mobile app.

## Getting Started
### What you will need
- Arduino Uno
- DC Motors
- Motor Shield
- HC-05 Bluetooth Module
- Power Source

### Circuit diagram
![Circuit diagram image](https://github.com/user-attachments/assets/6764eee1-2715-4105-98bd-ebf9b82373c0)

### Running the program
1. Connect the hardware following the circuit diagram shown above
2. Clone this repository:
    ```bash
    git clone https://github.com/paulpleela/bluetooth-motor-control.git
    ```
3. Set up the Arduino IDE:
    - Open the Arduino IDE and connect the Arduino board to your computer using a USB cable
    - Select the correct board (`Tools > Board > Arduino AVR Boards > Arduino Uno`) and port (`Tools > Port > COMx`)
4. Install the required libraries
    - Go to `Sketch > Include Library > Manage Libraries`
    - Search and install the [RemoteXY library](https://remotexy.com/en/library) (version 3.1.14)
6. Press the `->` icon in the top left of the IDE to upload the code to the Arduino board

### Controlling the motors
1. Install the [RemoteXY app](https://remotexy.com/en/download/) on your mobile device
2. Pair the HC-05 Bluetooth module with your mobile device
3. Use the graphical interface (joysticks) on the RemoteXY app to control the motors

## Acknowledgements
- Part of this code was taken and modified from the Test Code provided in the [Duinotech L203D Motor Control Shield XC4472 Datasheet](https://www.electusdistribution.com.au/dbdocument/701175/xc4472_manual_14833.pdf)
- The source code of the graphical user interface was generated automatically by the [RemoteXY editor](https://remotexy.com/en/editor) and was modified and used under the terms of the GNU Lesser General Public License (version 2.1) as published by the Free Software Foundation
- Learned from examples provided at the [ENGG1100 Arduino Demo](https://github.com/aburo8/engg1100-demo)
