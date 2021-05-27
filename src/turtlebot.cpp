/** MIT License
Copyright (c) 2017 Sudarshan Raghunathan
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
*
*@copyright Copyright 2017 Sudarshan Raghunathan
*@file turtlebot.cpp
*@author Sudarshan Raghunathan
*@brief  Functions definitions for turtlebot class
*/

#include <geometry_msgs/Twist.h>
#include <vector>
#include "ros/ros.h"
#include "ros/console.h"
#include "turtlebot.hpp"
#include "line_follower_turtlebot/pos.h"
#include <string>

void turtlebot::dir_sub(line_follower_turtlebot::pos msg) {
    turtlebot::dir = msg.direction;
    turtlebot::dx = msg.dx;
}

void turtlebot::get_pose(ar_track_alvar_msgs::AlvarMarkers req) {
    timer++;
    if (!req.markers.empty()) {
        ROS_INFO("find marker");
        double min_distance = 1e7;
        int last_marker_id = marker_id;
        for (auto &marker : req.markers) {
            int id = marker.id;
            double dis = marker.pose.pose.position.z;
            ROS_INFO("find marker id: %d, distance %f", id, dis);
            if (dis < min_distance) {
                min_distance = dis;
//                if (min_distance < 5) {
                marker_id = id;
                marker_distance = min_distance;
//                }
            }
        }
        if (timer > 30) {
            if (min_distance < 2) {
//            ROS_INFO("find marker id %d", marker_id);
                turtlebot::say = true;
                timer = 0;
            }
        } else {
            turtlebot::say = false;
        }
    }
//    turtlebot::say = false;
//    marker_id = -1;
}

void turtlebot::vel_cmd(geometry_msgs::Twist &velocity,
                        ros::Publisher &pub, ros::Rate &rate) {

    // If direction is left
    double stop_distance = 0.13;
    if (turtlebot::marker_id == 0 && turtlebot::marker_distance < 0.7 && turtlebot::marker_distance > stop_distance) {
        velocity.angular.z = turtlebot::dx / 210.0 * 1;
        velocity.linear.x = (1 - abs(turtlebot::dx) / 350) * 0.2;

        turtlebot::marker_distance -= velocity.linear.x * 1 / 15;

        pub.publish(velocity);
//            rate.sleep();
        ROS_INFO("STOP distance %f", turtlebot::marker_distance);
    } else if (turtlebot::marker_id == 0 && turtlebot::marker_distance < stop_distance) {
        velocity.angular.z = 0;
        velocity.linear.x = 0;
        pub.publish(velocity);
//            rate.sleep();
        ROS_INFO("STOP distance %f", turtlebot::marker_distance);
    } else {
        if (abs(dx) < 50) {
            velocity.angular.
                    z = turtlebot::dx / 210.0 * 1;
            velocity.linear.
                    x = (1 - abs(turtlebot::dx) / 500) * 0.22;
        } else if (abs(dx) < 250) {
            velocity.angular.
                    z = turtlebot::dx / 210.0 * 1.7;
            velocity.linear.
                    x = (1 - abs(turtlebot::dx) / 500) * 0.22;
        } else {
            velocity.angular.z = 0;
            velocity.linear.x = 0;
        }
        ROS_INFO("vx:%f, vz:%f", velocity.linear.x, velocity.angular.z);
        pub.publish(velocity);
//            rate.sleep();
    }
}


