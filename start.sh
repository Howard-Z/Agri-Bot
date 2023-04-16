#!/usr/bin/env bash

# Function to clean up ROS nodes on exit
cleanup() {
  echo "Cleaning up ROS nodes..."
  rosnode kill -a
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

# Launch robot launch file
roslaunch agri-bot agri-bot.launch
