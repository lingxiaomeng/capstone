////
//// Created by turtlepc-01 on 2021/3/30.
////
//
//#include <cv_bridge/cv_bridge.h>
//#include <cstdlib>
//#include <string>
//#include <opencv2/highgui/highgui.hpp>
//#include <sound_play.h>
//#include "opencv2/opencv.hpp"
//#include "ros/ros.h"
//#include "ros/console.h"
//#include "turtlebot.hpp"
//#include <thread>
//
//void say(std::string &word, sound_play::SoundClient &sc) {
//    sc.say(word);
//};
//
//
//int main(int argc, char **argv) {
//    // Initializing node and object
//    ros::init(argc, argv, "Speaker_node");
//    ros::NodeHandle n;
//
//    // Creating subscriber and publisher
//
//
//    ros::Subscriber sound_sub = n.subscribe("/speaker", 1, say);
//    sound_play::SoundClient sc;
//    ros::Rate rate(10);
//
//    ros::spin();
//    return 0;
//}
