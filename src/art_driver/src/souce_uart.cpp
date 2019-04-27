#include <ros/ros.h>
#include <serial/serial.h>
#include <iostream>
#include "../include/art_racecar_driver.h"
#include <ros/package.h>
#include <geometry_msgs/Twist.h>

serial::Serial sp;


void scale_turn(uint16_t data_line,uint16_t data_angle,uint8_t goal[4])
{
        goal[1] = uint8_t(data_line);
     
        goal[0] = uint8_t(data_line>>8);

        goal[3] = uint8_t(data_angle);

        goal[2] = uint8_t(data_angle>>8);

}
uint8_t check_data(uint8_t goal[4],uint8_t check)
{   
        
        int i = 0;
        for(i = 0;i < 4;i++)
        {
            check += goal[i];            
        }
        //ROS_INFO("%d %02X\n",check,check);
        return check;

}
 void TwistCallback(const geometry_msgs::Twist& twist)
{
    ROS_INFO("hellow:");
    double angle;
    int count = 0;
    uint8_t start[2] = {0x01,0xaa};  //起始位
    uint8_t data[4] = {0};      //数据位
    uint8_t check = 0;          //校验位 
    uint8_t finish = 0x11;      //结束位
    //ROS_INFO("x= %f", twist.linear.x);
    //ROS_INFO("z= %f", twist.angular.z);
   // angle = 2500.0 - twist.angular.z * 2000.0 / 180.0;
   //   ROS_INFO("angle= %d",uint16_t(angle));
   
    
    scale_turn(twist.linear.x,1024,data);      //进制转换
    check = check_data(data,check);             //计算校验位         

    sp.write(start,2);
    sp.write(data,4);
    sp.write(&check,1);  
    sp.write(&finish,1);
    
    ROS_INFO("data:\n");
    for(count = 0;count < 2;count++)
    {
        ROS_INFO("%02X\t",start[count]);
    }
    
    for(count = 0;count < 4;count++)
    {
        ROS_INFO("%02X\t",data[count]);

    }
    ROS_INFO("%02X\t",check);
    ROS_INFO("%02X\n",finish);

  //  send_cmd(uint16_t(1039),uint16_t(1039));
}

int main(int argc, char** argv)
{
    uint8_t buffer = 0;
    ros::init(argc, argv, "serial_port");
    //创建句柄（虽然后面没用到这个句柄，但如果不创建，运行时进程会出错）
    ros::NodeHandle n;

    //订阅函数
   ros::Subscriber turn = n.subscribe("/car/cmd_vel",1,TwistCallback);
   

    
    //创建一个serial类
  
    //创建timeout
    serial::Timeout to = serial::Timeout::simpleTimeout(100);
    //设置要打开的串口名称
    sp.setPort("/dev/ttyUSB0");
    //设置串口通信的波特率
    sp.setBaudrate(115200);
    //串口设置timeout
    sp.setTimeout(to);
 
    try
    {
        //打开串口
        sp.open();
    }
    catch(serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port.");
        return -1;
    }
    
    //判断串口是否打开成功
    if(sp.isOpen())
    {
        ROS_INFO_STREAM("/dev/ttyUSB0 is opened.");
    }
    else
    {
        return -1;
    }
    
   // ros::Rate loop_rate(1);

    while(ros::ok())
    {
 
        
        //loop_rate.sleep();
        ros::spin();
    }
    
    //关闭串口
    sp.close();
 
    return 0;
}
