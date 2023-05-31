#!/usr/bin/env python
import rospy
import time
import numpy as np
import math
from std_msgs.msg import String, Int8

ch = []
last_call_time = 0
wait_time = .01 #in seconds

def channel(i):
    return ch[i - 1]

def callback(input_data):
    global ch
    global last_call_time
    current_time = time.time()
    if current_time - last_call_time > wait_time:
      #last_call_time = current_time
      ch = input_data.data.split(',') #stupid ROS msg stuff
      if channel(5) == "1000":
          manual_control()
      else:
          return
    return

#Bless https://www.instructables.com/Joystick-to-Differential-Drive-Python/
def joystickToDiff(x, y, minJoystick, maxJoystick, minSpeed, maxSpeed): # If x and y are 0, then there is not much to calculate...
	if x == 0 and y == 0:
		return (0, 0)
    

	# First Compute the angle in deg
	# First hypotenuse
	z = math.sqrt(x * x + y * y)

	# angle in radians
	rad = math.acos(math.fabs(x) / z)

	# and in degrees
	angle = rad * 180 / math.pi

	# Now angle indicates the measure of turn
	# Along a straight line, with an angle o, the turn co-efficient is same
	# this applies for angles between 0-90, with angle 0 the coeff is -1
	# with angle 45, the co-efficient is 0 and with angle 90, it is 1

	tcoeff = -1 + (angle / 90) * 2
	turn = tcoeff * math.fabs(math.fabs(y) - math.fabs(x))
	turn = round(turn * 100, 0) / 100

	# And max of y or x is the movement
	mov = max(math.fabs(y), math.fabs(x))

	# First and third quadrant
	if (x >= 0 and y >= 0) or (x < 0 and y < 0):
		rawLeft = mov
		rawRight = turn
	else:
		rawRight = mov
		rawLeft = turn

	# Reverse polarity
	if y < 0:
		rawLeft = 0 - rawLeft
		rawRight = 0 - rawRight

	# minJoystick, maxJoystick, minSpeed, maxSpeed
	# Map the values onto the defined rang
	rightOut = map(rawRight, minJoystick, maxJoystick, minSpeed, maxSpeed)
	leftOut = map(rawLeft, minJoystick, maxJoystick, minSpeed, maxSpeed)

	return (rightOut, leftOut)

def map(v, in_min, in_max, out_min, out_max):
	# Check that the value is at least in_min
	if v < in_min:
		v = in_min
	# Check that the value is at most in_max
	if v > in_max:
		v = in_max
	return (v - in_min) * (out_max - out_min) // (in_max - in_min) + out_min


#totally can do better since the motor speeds depends on the width of the robot...
#this is a temp fix for now
def manual_control():
    global ch
    FL = rospy.Publisher('/Motors/FL', Int8, queue_size=1)
    FR = rospy.Publisher('/Motors/FR', Int8, queue_size=1)
    BL = rospy.Publisher('/Motors/BL', Int8, queue_size=1)
    BR = rospy.Publisher('/Motors/BR', Int8, queue_size=1)
    ch1 = int(channel(1)) - 1500
    ch2 = int(channel(2)) - 1500
    throttle = joystickToDiff(ch1, ch2, -511, 511, -127, 127)
    #print(throttle)
    FL.publish(int(throttle[0]))
    BL.publish(int(throttle[0]))
    FR.publish(int(throttle[1]))
    BR.publish(int(throttle[1]))
    #FIX DEBOUNCING ISSUE USING MILLIS()
    # unsigned long lastExecuted = 0;
    # unsigned long debounceDelay = 1000; // debounce delay in milliseconds

    
def main():

  # In ROS, nodes are uniquely named. If two nodes with the same
  # name are launched, the previous one is kicked off. The
  # anonymous=True flag means that rospy will choose a unique
  # name for our 'listener' node so that multiple listeners can
  # run simultaneously.
  rospy.init_node('motor_control')

  rospy.Subscriber("/RC/CH", String, callback)
  # spin() simply keeps python from exiting until this node is stopped
  rospy.spin()
if __name__ == '__main__':
  main()