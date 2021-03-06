cmake_minimum_required(VERSION 2.8.3)
project(line_follower_turtlebot)

add_definitions(-std=c++11)

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS cv_bridge roscpp rospy std_msgs genmsg geometry_msgs actionlib_msgs actionlib
        sensor_msgs)
find_package(OpenCV REQUIRED)

## Declare ROS messages and services
add_message_files(FILES pos.msg sound.msg)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

catkin_package()

## Executables
include_directories(include ${catkin_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(section2 src/section2.cpp src/linedetect.cpp)
target_link_libraries(section2 ${catkin_LIBRARIES} ${OpenCV_LIBS})
add_dependencies(section2 line_follower_turtlebot_generate_messages_cpp)

add_executable(section1 src/section1.cpp src/linedetect.cpp)
target_link_libraries(section1 ${catkin_LIBRARIES} ${OpenCV_LIBS})
add_dependencies(section1 line_follower_turtlebot_generate_messages_cpp)



add_executable(detect src/detect.cpp src/linedetect.cpp)
target_link_libraries(detect ${catkin_LIBRARIES} ${OpenCV_LIBS})
add_dependencies(detect line_follower_turtlebot_generate_messages_cpp)

add_executable(navig src/motion_node.cpp src/turtlebot.cpp)
target_link_libraries(navig ${catkin_LIBRARIES})
add_dependencies(navig line_follower_turtlebot_generate_messages_cpp)

