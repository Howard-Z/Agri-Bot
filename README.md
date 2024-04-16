# Agri-Bot
This is the ROS package branch of the Agri-Bot Repo. To see the source code for the ESP32 microcontrollers, switch branches to the `ESP32-Code` branch.

## Note:
This repository assumes that you have combed through the intro tutorials of the [ROS Wiki](https://wiki.ros.org/). And you know how the structure of a ROS package works

## Prereqs
- The ROS MQTT bridge
- A hotspot running on the Jetson

## Running it
The first thing that is run is `start.sh`. A closer inspection shows that we start up the roscore and then run the launch file for our project.

Navigating to `./launch/agri-bot.launch` we can see that we start the MQTT bridge and the motor controller python script.

The python script is located at `./src/motor_control/scripts/motor_control.py`

What the python script does is receive data from the remote controller through the RC topic list, and process the data as either motor contoll signals or sprayer signals. It then pushes out the commands back through a ROS topic.

The ROS topics are bridged using the MQTT bridge so that the ESP32's can receive their signals over wifi.