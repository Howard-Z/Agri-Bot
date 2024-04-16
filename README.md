# Agri-Bot

## ESP32 code

This is the code for the different types of functions that ESP32s serve on the robot.

There are a few functions that they serve:
1. Radiomaster Receiver (x1)
2. Motor Controller (x2 one for front one for back)
3. Sprayer Controller (x1)

The source code for each can be found in the following areas:
1. RadioMaster: ./radiomaster/src/receiver/receiver/receiver.ino
2. Motor Controller: ./radiomaster/src/motor_controller/motor_controller/motor_controller.ino
3. Sprayer: ./radiomaster/src/sprayer/sprayer.ino

The basic_crsf folder is just there as ground truth testing to make sure that the radio works in the first place.

### The Receiver
The whole job of this is to read the signals from the remote through CRSF serial and pass the information straight to MQTT

### The Motor Controller
This program just receives commands from MQTT as speed settings and applies them immediately to the motor

### The Sprayer
This one also receives commands from MQTT and toggles the appropriate relay based on the state.