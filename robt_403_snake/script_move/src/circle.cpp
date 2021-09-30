
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit_msgs/DisplayRobotState.h>
#include <moveit_msgs/DisplayTrajectory.h>
#include <moveit_msgs/AttachedCollisionObject.h>
#include <moveit_msgs/CollisionObject.h>
#include <math.h>


// Main moveit libraries are included
int main(int argc, char **argv)
{
  ros::init(argc, argv, "move_group_interface_tutorial");
  ros::NodeHandle node_handle;
  ros::AsyncSpinner spinner(0);
  spinner.start(); 
  
  static const std::string PLANNING_GROUP = "snake_arm"; 
  moveit::planning_interface::MoveGroupInterface move_group(PLANNING_GROUP); // loading move_group

  const robot_state::JointModelGroup *joint_model_group = move_group.getCurrentState()->getJointModelGroup(PLANNING_GROUP); //For joint control
  geometry_msgs::PoseStamped current_pose;
  geometry_msgs::PoseStamped target_pose; 
  geometry_msgs::PoseStamped start_pose; 
  
  current_pose = move_group.getCurrentPose(); /* Retrieving the information about the current position and orientation of the end effector*/
  target_pose = current_pose;

  // Start position
  start_pose.pose.position.x = 2.25;
  start_pose.pose.position.y = 0.00;
  
  // Rectangle move
  float rad = 1;
  float inc = 0.01;
  float x = 0.0;
  float y = 0.0;
  float Ox = start_pose.pose.position.x-rad;
 
  ros::Rate loop_rate(100); //Frequency
  
  while (ros::ok()){
  	target_pose = start_pose;
 	ROS_INFO("Start position");
 	move_group.setApproximateJointValueTarget(target_pose); // To calculate the trajectory
    	move_group.move(); // Move the robot

  	for (int i = 0; i <= 200; i++ ){
  		ROS_INFO("Move %i", i);
  		x = target_pose.pose.position.x - Ox;
       	ROS_INFO("x %f", x);
       	ROS_INFO("y %f", y);
       	target_pose.pose.position.x = target_pose.pose.position.x - inc;
       	if (i <= 100) {
       		y = sqrt(rad*rad - x*x);
       		target_pose.pose.position.y = y;
       	} else {
       		y = sqrt(x*x - rad*rad);
       		target_pose.pose.position.y = -y;
       	}
       	
       	move_group.setApproximateJointValueTarget(target_pose); 
    		move_group.move(); // Move the robot
    		current_pose = move_group.getCurrentPose();
    	if (x == 0.0 || x == 0.5 || x == -0.5) {
    		if (abs(current_pose.pose.position.x - target_pose.pose.position.x) < 0.1 && 	abs(current_pose.pose.position.y - target_pose.pose.position.y) < 0.1){
      		continue; // Basically, check if we reached the desired position
    		} else {
    		ROS_INFO("Current x: %f", current_pose.pose.position.x);
    		ROS_INFO("Target x: %f", target_pose.pose.position.x);
    		ROS_INFO("Current y: %f", current_pose.pose.position.y);
    		ROS_INFO("Target y: %f", target_pose.pose.position.y);
    		ROS_INFO("Error");
  		ros::shutdown();
    		}
    	}
  	}

    loop_rate.sleep();

  }

  ROS_INFO("Done");
  ros::shutdown();
  return 0;
}
