#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <goal.h>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char **argv) {
    ros::init(argc, argv, "simple_navigation_goals");

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
            goal_pose.target_pose.pose.position.x = 3.6366;
            goal_pose.target_pose.pose.position.y = 4.85801;
            goal_pose.target_pose.pose.orientation.z = 0.9495;
            goal_pose.target_pose.pose.orientation.w = -0.1795;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS2_1 succeed");
                status = PS2_2;
            } else
                ROS_INFO("PS2_1 failed");
        } else if (status == PS2_2) {
            goal_pose.target_pose.pose.position.x = 0.66159;
            goal_pose.target_pose.pose.position.y = 4.85801;
            goal_pose.target_pose.pose.orientation.z = 0.9495;
            goal_pose.target_pose.pose.orientation.w = -0.1795;
            ROS_INFO("Sending goal");
            ac.sendGoal(goal_pose);
            ac.waitForResult();
            if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
                ROS_INFO("PS2_1 succeed");
                status = PS2_2;
            } else
                ROS_INFO("PS2_1 failed");
        } else if (status == PS3) {

        } else if (status == PS4) {

        }

    }


    move_base_msgs::MoveBaseGoal ps3;
    //we'll send a goal to the robot to move 1 meter forward
    ps3.target_pose.header.frame_id = "map";
    ps3.target_pose.header.stamp = ros::Time::now();
    ps3.target_pose.pose.position.x = 2.52164;
    ps3.target_pose.pose.position.y = 0.297645;
    ps3.target_pose.pose.orientation.w = 1.0;

    ROS_INFO("Sending goal");
    ac.sendGoal(ps3);
    ac.waitForResult();
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward");
    else
        ROS_INFO("The base failed to move forward 1 meter for some reason");

    move_base_msgs::MoveBaseGoal ps4;
    //we'll send a goal to the robot to move 1 meter forward
    ps4.target_pose.header.frame_id = "map";
    ps4.target_pose.header.stamp = ros::Time::now();
    ps4.target_pose.pose.position.x = 6.2153;
    ps4.target_pose.pose.position.y = 2.3197;
    ps4.target_pose.pose.orientation.w = 1.0;


    ROS_INFO("Sending goal");
    ac.sendGoal(ps4);
    ac.waitForResult();
    if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        ROS_INFO("Hooray, the base moved 1 meter forward");
    else
        ROS_INFO("The base failed to move forward 1 meter for some reason");
    return 0;
}