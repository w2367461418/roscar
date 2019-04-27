#include "ros/ros.h"


int main(int argc,char ** argv)
{
	int number = 0;
	ros::init(argc,argv,"get_param");
	ros::NodeHandle n("~");

	ros::Rate loop(1);
	while(ros::ok())
	{

		n.getParam("/dynamic_node/baseSpeed",number);
		ROS_INFO("param: %d\n",number);
	//	ROS_INFO("hellow!");
		loop.sleep();	
	}

	
	
	

	return 0;



}
