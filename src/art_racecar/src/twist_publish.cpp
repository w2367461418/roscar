#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>

int main(int argc,char ** argv)
{
	ros::init(argc,argv,"odom_publish");
	ros::NodeHandle p;

	ros::Publisher odom_pub = p.advertise<nav_msgs::Odometry>("odom",5); //发布信息
	tf::TransformBroadcaster odom_broadcaster;

	//起始坐标 
	double x = 0.0;
	double y = 0.0;
	double th = 0.0;

	//线速度
	double vx = 0.1;//double vx = 0.1;
	double vy = -0.2;//double vy = 0.1;
	//角速度
	
	double vth = 0.1;

	ros::Time current_time,last_time;
	current_time = ros::Time::now();
	last_time = ros::Time::now();

	ros::Rate r(1);   //设置发送频率
	ROS_INFO("hellow");

	while(p.ok())
	{
		ros::spinOnce();
		//计算一次 所走过的距离 与 角度    //以逆时针旋转为正方向
		double dt = 1;//(current_time - last_time).toSec();
		/*double delta_x = -(vx + vy)/2 * sin(vth);
		double delta_y = (vx + vy)/2 * cos(vth);*/

        double delta_x = (vx * cos(th) - vy * sin(th)) * dt;
		double delta_y = (vx * sin(th) + vy * cos(th)) * dt;
	    double delta_th = vth * dt;
 
		//与上次相加
        x += delta_x;
        y += delta_y;
        th += delta_th;
		
        //since all odometry is 6DOF we'll need a quaternion created from yaw
        geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(th);
        //first, we'll publish the transform over tf
        geometry_msgs::TransformStamped odom_trans;
        odom_trans.header.stamp = current_time;
        odom_trans.header.frame_id = "odom";
        odom_trans.child_frame_id = "base_link";
        odom_trans.transform.translation.x = x;
        odom_trans.transform.translation.y = y;
        odom_trans.transform.translation.z = 0.0;
        odom_trans.transform.rotation = odom_quat;

        //send the transform
        odom_broadcaster.sendTransform(odom_trans);

        //next, we'll publish the odometry message over ROS
        nav_msgs::Odometry odom;
        odom.header.stamp = current_time;
        odom.header.frame_id = "odom";

        //set the position
        odom.pose.pose.position.x = x;
        odom.pose.pose.position.y = y;
        odom.pose.pose.position.z = 0.0;
        odom.pose.pose.orientation = odom_quat;

        //set the velocity
        odom.child_frame_id = "base_link";
        odom.twist.twist.linear.x = vx;//delta_x;//
        odom.twist.twist.linear.y = vy;//delta_y;//
        odom.twist.twist.angular.z = vth;

        //publish the message
        odom_pub.publish(odom);
        last_time = current_time;
        r.sleep();
		
		



	}


	return 0;
}
