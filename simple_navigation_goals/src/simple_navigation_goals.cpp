#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;
void move_to_goal(float,float);
int main(int argc, char** argv){
ros::init(argc, argv, "simple_navigation_goals");
 float x,y;
 std::cout<<"Enter the co-ordinates to which you want to move the robot (x-coordinates and then y-coordinates)"<<std::endl;
 std::cin>>x;
 std::cin>>y;
 move_to_goal(x,y);
 return 0;
}
void move_to_goal(float a,float b){ 
MoveBaseClient ac("move_base", true);
while(!ac.waitForServer(ros::Duration(5.0))){
ROS_INFO("Waiting for the move_base action server to come up");
}
move_base_msgs::MoveBaseGoal goal;
goal.target_pose.header.frame_id = "map";
goal.target_pose.header.stamp = ros::Time::now();
goal.target_pose.pose.position.x = a;
goal.target_pose.pose.position.y = b;
goal.target_pose.pose.position.z =  0.0;
goal.target_pose.pose.orientation.x = 0.0;
goal.target_pose.pose.orientation.y = 0.0;
goal.target_pose.pose.orientation.z = 0.0;
goal.target_pose.pose.orientation.w = 1.0;
ROS_INFO("Sending goal");
ac.sendGoal(goal);
ac.waitForResult();
if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
ROS_INFO("Robot has moved to the location");
else
ROS_INFO("Failure in trying to move the robot to the location");
}
