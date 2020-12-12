#!/usr/bin/env python

import rospy
from turtlesim_cleaner.srv import * 
def move_circle_client():
    rospy.wait_for_service('move_circle')
    try:
        radius = float(input("Enter the radius of the circle : "))
        speed = float(input("Enter the speed of the turtle : "))
        move_circle = rospy.ServiceProxy('move_circle', MoveCircle)
        move_circle(speed,radius)
        #return resp1.sum
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

if __name__ == "__main__":
    print "Enter the radius of the circle and the velocity"
    move_circle_client()
