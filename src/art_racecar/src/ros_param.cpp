#include <ros/ros.h>
#include <dynamic_reconfigure/server.h>
#include "art_racecar/ros_testConfig.h"


void callback(art_racecar::ros_testConfig &config,uint32_t level)
{
    ROS_INFO("Reconfigure request : %d ",config.baseSpeed);
	ROS_INFO("hellow!");


}


int main(int argc,char **argv)
{
	ros::init(argc,argv,"dynamic_node");
	
	dynamic_reconfigure::Server<art_racecar::ros_testConfig> server;
	dynamic_reconfigure::Server<art_racecar::ros_testConfig>::CallbackType f;
	
	f = boost::bind(&callback,_1,_2);
	server.setCallback(f);

	ROS_INFO("Spinning node");
	ros::spin();

	return 0;
	
}



