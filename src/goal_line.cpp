#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <goal.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <sound_play.h>
#include <linedetect.hpp>
#include <geometry_msgs/Twist.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

double distance = 0;

void vel_cmd(geometry_msgs::Twist &velocity,
             ros::Publisher &pub, double dx) {
    if (abs(dx) < 50) {
        velocity.angular.
                z = dx / 210.0 * 1;
        velocity.linear.
                x = (1 - abs(dx) / 500) * 0.22;
    } else if (abs(dx) < 250) {
        velocity.angular.
                z = dx / 210.0 * 1.7;
        velocity.linear.
                x = (1 - abs(dx) / 500) * 0.22;
    } else {
        velocity.angular.z = 0;
        velocity.linear.x = 0;
    }
    ROS_INFO("vx:%f, vz:%f", velocity.linear.x, velocity.angular.z);
    pub.publish(velocity);
    distance += velocity.linear.x * 1 / 15;
//            rate.sleep();
}


int main(int argc, char **argv) {
    ros::init(argc, argv, "simple_navigation_goals");
    ros::NodeHandle n;

//    ros::Subscriber marker_sub = n.subscribe("/ar_pose_marker", 10, get_pose);
    sound_play::SoundClient sc;

    //tell the action client that we want to spin a thread by default
    LineDetect det;

    ros::Subscriber image_sub = n.subscribe("/raspicam_node/image/compressed",
                                            15, &LineDetect::imageCallback, &det);

    ros::Publisher cmd_pub = n.advertise<geometry_msgs::Twist>
            ("/cmd_vel", 10);

    // Creating Publisher and subscriber


    MoveBaseClient ac("move_base", true);

    //wait for the action server to come up
    while (!ac.waitForServer(ros::Duration(5.0))) {
        ROS_INFO("Waiting for the move_base action server to come up");
    }


    int status = PS2_1;
//    status = LINE_FOLLOW;

    move_base_msgs::MoveBaseGoal goal_pose;
    //we'll send a goal to the robot to move 1 meter forward
    goal_pose.target_pose.header.frame_id = "map";
    goal_pose.target_pose.header.stamp = ros::Time::now();
    goal_pose.target_pose.pose.position.x = 0.66159;
    goal_pose.target_pose.pose.position.y = 3.9322;
    goal_pose.target_pose.pose.orientation.w = 1.0;
    ros::Rate rate(15);

    geometry_msgs::Twist velocity;

    while (ros::ok()) {
        if (status == PS2_1) {
//            goal_pose.target_pose.header.stamp = ros::Time::now();
//            goal_pose.target_pose.pose.position.x = 4.16894;
//            goal_pose.target_pose.pose.position.y = 5.95852;
//            goal_pose.target_pose.pose.orientation.z = 0.1795;
//            goal_pose.target_pose.pose.orientation.w = 0.9495;
//
//            ROS_INFO("Sending goal");
//            ac.sendGoal(goal_pose);
//            ac.waitForResult();
//            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
//                ROS_INFO("PS2_1 succeed");
//                status = PS2_2;
//            } else
//                ROS_INFO("PS2_1 failed");
//

            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 3.8452;
            goal_pose.target_pose.pose.position.y = 4.9177;
            goal_pose.target_pose.pose.orientation.z = 0.9586;
            goal_pose.target_pose.pose.orientation.w = -0.2846;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS2_1 succeed");
                status = LINE_FOLLOW;
            } else
                ROS_INFO("PS2_1 failed");
        } else if (status == PS2_2) {
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 0.566 + 0.04 * 0.769588413377896;
            goal_pose.target_pose.pose.position.y = 3.898 + 0.04 * 0.6385402681072606;
            goal_pose.target_pose.pose.orientation.z = 0.2935;
            goal_pose.target_pose.pose.orientation.w = 0.9557;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();

            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {

                sc.say("PS2");

                ROS_INFO("PS2_2 succeed");
                status = MARKER1;
                goal_pose.target_pose.header.stamp = ros::Time::now();
                goal_pose.target_pose.pose.position.x = 0.566;
                goal_pose.target_pose.pose.position.y = 3.898;
                goal_pose.target_pose.pose.orientation.z = 0.2935;
                goal_pose.target_pose.pose.orientation.w = 0.9557;
            } else {
                ROS_INFO("PS2_2 failed");
                status = PS2_1;
            }

        } else if (status == PS3) {
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 2.6322;
            goal_pose.target_pose.pose.position.y = 0.2232;
            goal_pose.target_pose.pose.orientation.z = 0.2935;
            goal_pose.target_pose.pose.orientation.w = 0.9557;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS3 succeed");
                sc.say("PS3");

                status = MARKER2;
            } else
                ROS_INFO("PS3 failed");
        } else if (status == PS4) {
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 6.22;
            goal_pose.target_pose.pose.position.y = 2.32;
            goal_pose.target_pose.pose.orientation.z = 0.2935;
            goal_pose.target_pose.pose.orientation.w = 0.9557;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS4 succeed");
                sc.say("PS4");
                break;
            } else
                ROS_INFO("PS4 failed");
        } else if (status == MARKER1) {
            boost::shared_ptr<ar_track_alvar_msgs::AlvarMarkers const> msg;
            msg = ros::topic::waitForMessage<ar_track_alvar_msgs::AlvarMarkers>("/ar_pose_marker", ros::Duration(1.0));
            ROS_INFO("receive msg");
            int n = 0;
            if (msg) {
//                ROS_INFO("%d",msg->markers.at(0).id);
                for (auto &marker : msg->markers) {
                    int id = marker.id;
                    double dis = marker.pose.pose.position.z;
                    ROS_INFO("find marker id: %d, distance %f", id, dis);
                    if (dis < 5) {
                        //            say = false;
                        std::string words = std::to_string(id);
                        sc.say(words);
                        n++;
                        status = PS3;
                    }
                }
//                status = PS3;
            }
            if (n == 0) {
                ROS_INFO("no marker");
                goal_pose.target_pose.header.stamp = ros::Time::now();
                goal_pose.target_pose.pose.position.x += 0.4 * 0.769588413377896;
                goal_pose.target_pose.pose.position.y += 0.4 * 0.6385402681072606;
                ac.sendGoal(goal_pose);
                ac.waitForResult();
                if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                    ROS_INFO("marker move");
                } else
                    ROS_INFO("marker move failed");
            }
        } else if (status == MARKER2) {
//2.4744 1.1521 0.3529 0.9356

            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 2.1785 - 0.0 * 0.769588413377896;
            goal_pose.target_pose.pose.position.y = 0.9973 - 0.0 * 0.6385402681072606;
            goal_pose.target_pose.pose.orientation.z = 0.3529;
            goal_pose.target_pose.pose.orientation.w = 0.9356;

            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("marker move");
            } else
                ROS_INFO("marker move failed");

            boost::shared_ptr<ar_track_alvar_msgs::AlvarMarkers const> msg;
            msg = ros::topic::waitForMessage<ar_track_alvar_msgs::AlvarMarkers>("/ar_pose_marker", ros::Duration(1.0));
            ROS_INFO("receive msg");
            if (msg) {
//                ROS_INFO("%d",msg->markers.at(0).id);
                for (auto &marker : msg->markers) {
                    int id = marker.id;
                    double dis = marker.pose.pose.position.z;
                    ROS_INFO("find marker id: %d, distance %f", id, dis);
                    if (dis < 2) {
                        //            say = false;
                        std::string words = std::to_string(id);
                        sc.say(words);
                        status = PS4;
                    }
                }
//                status = PS3;
            }
        } else if (status == LINE_FOLLOW) {
            if (!det.img.empty()) {
                // Perform image processing
//            ROS_INFO("read image");
                det.img_filt = det.Gauss(det.img);
                det.colorthresh(det.img_filt);
                double dx = det.dx;
                // Publish direction message
//                dirPub.publish(msg);
                if (dx > 50) {
                    dx = 0;
                }
                vel_cmd(velocity, cmd_pub, dx);
            }
            if (distance > 0.5) {
                distance = 0;
                status = PS2_2;
            }
            rate.sleep();
        }

        ros::spinOnce();
//        if (say) {
//            ROS_INFO("say");
//            say = false;
//            std::string words = std::to_string(marker_id);
////            std::thread th1(say_word, std::ref(words), std::ref(sc));
//            sc.say(words);
//        }
    }

    return 0;
}