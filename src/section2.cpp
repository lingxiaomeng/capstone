#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <section2.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <sound_play.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char **argv) {
    ros::init(argc, argv, "simple_navigation_goals");
    ros::NodeHandle n;

    sound_play::SoundClient sc;

    //tell the action client that we want to spin a thread by default
    MoveBaseClient ac("move_base", true);

    //wait for the action server to come up
    while (!ac.waitForServer(ros::Duration(5.0))) {
        ROS_INFO("Waiting for the move_base action server to come up");
    }


    int status = PS2_1;

    move_base_msgs::MoveBaseGoal goal_pose;
    //we'll send a goal to the robot to move 1 meter forward
    goal_pose.target_pose.header.frame_id = "map";
    goal_pose.target_pose.header.stamp = ros::Time::now();
    goal_pose.target_pose.pose.position.x = 0.66159;
    goal_pose.target_pose.pose.position.y = 3.9322;
    goal_pose.target_pose.pose.orientation.w = 1.0;


    while (ros::ok()) {
        if (status == PS2_1) {
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 3.8497;
            goal_pose.target_pose.pose.position.y = 4.9141;
            goal_pose.target_pose.pose.orientation.z = 0.9347;
            goal_pose.target_pose.pose.orientation.w = -0.3557;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS2_1 succeed");
                status = PS2_2;
            } else
                ROS_INFO("PS2_1 failed");
        } else if (status == PS2_2) {
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 0.65;
            goal_pose.target_pose.pose.position.y = 3.92;

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
                goal_pose.target_pose.pose.position.x = 1.6948;
                goal_pose.target_pose.pose.position.y = 4.35448;
                goal_pose.target_pose.pose.orientation.z = 0.2935;
                goal_pose.target_pose.pose.orientation.w = 0.9557;
            } else {
                ROS_INFO("PS2_2 failed");
                status = PS2_1;
            }

        } else if (status == PS3) {
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 2.66;
            goal_pose.target_pose.pose.position.y = 0.24;
            goal_pose.target_pose.pose.orientation.z = 0.87098;
            goal_pose.target_pose.pose.orientation.w = 0.49268;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS3 succeed");
                sc.say("PS3");

                status = MARKER2;
            } else
                ROS_INFO("PS3 failed");
        }
        else if (status == MARKER1) {
            boost::shared_ptr<ar_track_alvar_msgs::AlvarMarkers const> msg;
            msg = ros::topic::waitForMessage<ar_track_alvar_msgs::AlvarMarkers>("/ar_pose_marker", ros::Duration(1.0));
            ROS_INFO("receive msg");
            int n = 0;
            if (msg) {
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
            goal_pose.target_pose.header.stamp = ros::Time::now();
            goal_pose.target_pose.pose.position.x = 2.1785;
            goal_pose.target_pose.pose.position.y = 0.9973;
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
                for (auto &marker : msg->markers) {
                    int id = marker.id;
                    double dis = marker.pose.pose.position.z;
                    ROS_INFO("find marker id: %d, distance %f", id, dis);
                    if (dis < 2) {
                        //            say = false;
                        std::string words = std::to_string(id);
                        sc.say(words);
                        status = PS2_2;
                    }
                }
            }
        }

        ros::spinOnce();
    }

    return 0;
}