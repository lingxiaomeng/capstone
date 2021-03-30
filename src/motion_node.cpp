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
*@file motion_node.cpp
*@author Sudarshan Raghunathan
*@brief  Ros node to read direction to move in and publish velocity to turtlebot
*/
#include <cv_bridge/cv_bridge.h>
#include <cstdlib>
#include <string>
#include <opencv2/highgui/highgui.hpp>
#include <sound_play.h>
#include "opencv2/opencv.hpp"
#include "ros/ros.h"
#include "ros/console.h"
#include "turtlebot.hpp"
#include "line_follower_turtlebot/pos.h"
#include <thread>

/**
*@brief Main function that reads direction from the detect node and publishes velocity to the turtlebot at a given rate
*@param argc is the number of arguments of the main function
*@param argv is the array of arugments
*@return 0
*/

void say_word(std::string &words, sound_play::SoundClient &sc) {
    sc.say(words);
}


int main(int argc, char **argv) {
    // Initializing node and object
    ros::init(argc, argv, "Velocity");
    ros::NodeHandle n;
    turtlebot bot;
    geometry_msgs::Twist velocity;
    // Creating subscriber and publisher
    ros::Subscriber sub = n.subscribe("/direction",
                                      1, &turtlebot::dir_sub, &bot);

    ros::Subscriber marker_sub = n.subscribe("/ar_pose_marker", 1, &turtlebot::get_pose, &bot);
    sound_play::SoundClient sc;

    ros::Publisher pub = n.advertise<geometry_msgs::Twist>
            ("/cmd_vel", 10);
    ros::Rate rate(10);

    while (ros::ok()) {
        ros::spinOnce();
        if (bot.say) {
            std::string words = std::to_string(bot.marker_id);
            std::thread th1(say_word, std::ref(words), std::ref(sc));
        }
        // Publish velocity commands to turtlebot
//        bot.vel_cmd(velocity, pub, rate);
        rate.sleep();
    }
    return 0;
}
