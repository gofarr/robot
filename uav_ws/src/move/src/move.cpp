#include   <stdio.h>     
#include   <stdlib.h>   
#include   <unistd.h>    
#include   <sys/types.h>
#include   <sys/stat.h>  
#include   <fcntl.h>    
#include   <termios.h>  
#include   <errno.h>    
#include   <string.h> 
#include   <poll.h>
#include   <ros/ros.h>  
#include   <std_msgs/Int16.h>
#include   <iostream>
#include   <ros/ros.h>
#include   <ros/forwards.h>
#include   <ros/single_subscriber_publisher.h>
#include   <apriltags/AprilTagDetection.h>
#include   <apriltags/AprilTagDetections.h>
#include   "pid_set.h"

using namespace std;

int k=-1;
float pos_x=1.0;
float pos_y=1.0;
float pos_z=1.0;
float ori_x=1.0;
float ori_y=1.0;
float ori_z=1.0;
float ori_w=1.0;
float yaw=1.0;
float pitch=1.0;
float roll=1.0;

struct pid_x pid_x;
struct pid_y pid_y;
struct pid_h pid_h;
struct pid_yaw pid_yaw;

void PID_x_realize(float input1)	//PID algorithm
{
	pid_x.error=input1;
	if(pid_x.v>0.0&&pid_x.error<0.0)
	{
		pid_x.integral+=pid_x.error;
	}
	else if(pid_x.v<0.0&&pid_x.error>0.0)
	{
		pid_x.integral+=pid_x.error;
	}
	else
	{
	}
	pid_x.v=pid_x.kp*pid_x.error+pid_x.ki*pid_x.integral+pid_x.kd*(pid_x.error-pid_x.error_last);
	pid_x.error_last=pid_x.error;
}

void PID_y_realize(float input2)	
{
	pid_y.error=input2;
	if(pid_y.v>0.0&&pid_y.error<0.0)
	{
		pid_y.integral+=pid_y.error;
	}
	else if(pid_y.v<0.0&&pid_y.error>0.0)
	{
		pid_y.integral+=pid_y.error;
	}
	else
	{
	}
	pid_y.v=pid_y.kp*pid_y.error+pid_y.ki*pid_y.integral+pid_y.kd*(pid_y.error-pid_y.error_last);
	pid_y.error_last=pid_y.error;
}

void PID_h_realize(float input3)	
{
	pid_h.error=input3;
	if(pid_h.v>0.0&&pid_h.error<0.0)
	{
		pid_h.integral+=pid_h.error;
	}
	else if(pid_h.v<0.0&&pid_h.error>0.0)
	{
		pid_h.integral+=pid_h.error;
	}
	pid_h.v=pid_h.kp*pid_h.error+pid_h.ki*pid_h.integral+pid_h.kd*(pid_h.error-pid_h.error_last);
	pid_h.error_last=pid_h.error;
}

void PID_yaw_realize(float input4)		//yaw_PID realize
{
	double a=(double)pid_x.error;
	double b=(double)pid_y.error;
	float c=(float)atan2(b,a);
	float angle=c-input4;
	
	if(angle> 3.14)
	{ 
		pid_yaw.error=angle-6.28;
	}
	else if(angle< -3.14)
	{
		pid_yaw.error=angle+6.28;
	}
	else
	{
		pid_yaw.error=angle;
	}

	pid_yaw.integral+=pid_yaw.error;
	pid_yaw.v=(pid_yaw.kp*pid_yaw.error+pid_yaw.ki*pid_yaw.integral+pid_yaw.kd*(pid_yaw.error- pid_yaw.error_last))*57.3;
	pid_y.error_last=pid_y.error;
}


void Quaternion_To_Euler(float q0,float q1,float q2,float q3, float& yaw,float& pitch,float& roll)
{
		float r11,r12,r21,r31,r32,r1,r2,r3;
		
    	r11 = 2.0f *(q1 * q2 + q0 * q3);
    	r12 = q0 * q0 + q1 * q1 - q2 * q2  - q3 * q3 ;
    	r21 = -2.0f * (q1 * q3 - q0 * q2);
    	r31 = 2.0f *( q2 * q3 + q0  * q1);
    	r32 = q0 * q0 - q1 * q1  - q2 * q2 + q3 * q3 ;
    	
    	yaw= atan2( r11, r12 );
        pitch = asin( r21 );
        roll = atan2( r31, r32 );
}

void MessageRecieved(const apriltags::AprilTagDetections& msg)
{
   	//m=msg.header.seq;
	k=printf("=====%d\n",msg.detections[0]);
	//printf("k=%d\n",k);
	if (k==13)
	{
		printf("wowenle\n");
		pos_x=msg.detections[0].pose.position.x;
		pos_y=msg.detections[0].pose.position.y;
		pos_z=msg.detections[0].pose.position.z;
		
		ori_x=msg.detections[0].pose.orientation.x;
		ori_y=msg.detections[0].pose.orientation.y;
		ori_z=msg.detections[0].pose.orientation.z;
		ori_w=msg.detections[0].pose.orientation.w;
		
		Quaternion_To_Euler(ori_w,ori_x,ori_y,ori_z,yaw,pitch,roll);
			
		//printf("%f\n",yaw);
		
	}

	else printf("wobile\n");

}

int main(int argc, char **argv)
{
       
	ros::init (argc, argv, "move");
	ros::NodeHandle nh;   
	ros::Subscriber sub = nh.subscribe("/apriltags/detections",1000,&MessageRecieved);  
	PID_set(&pid_x,&pid_y,&pid_h,&pid_yaw);
	
	while(ros::ok())
	{
		ros::spinOnce();
	}
        
	

}
