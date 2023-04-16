#!/usr/bin/env bash

# Function to clean up ROS nodes and terminate roscore
cleanup() {
  echo "Cleaning up ROS nodes and terminating roscore..."
  rosnode cleanup
  pkill -f roscore
}

# Trap SIGINT signal (Ctrl-C) and call cleanup function
trap cleanup SIGINT

# Start roscore in the background
roscore &

# Wait for roscore to initialize
while ! rostopic list > /dev/null 2>&1; do
  echo "Waiting for roscore to start..."
  sleep 1
done

echo sourcing files...
source devel/setup.bash
# Launch robot launch file
roslaunch agri-bot /home/bale-bot/Agri-bot/launch/agri-bot.launch
