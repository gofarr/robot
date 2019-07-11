 #include <ros/ros.h>
 #include <geometry_msgs/Twist.h> // For geometry_msgs:: Twist

 int main ( int argc , char ** argv ) 
 {
 // Initialize the ROS system and become a node .
 	ros::init ( argc , argv ,"publish_velocity" ) ;
 	ros::NodeHandle nh ;

 // Create a publisher obj ect .
 	ros::Publisher pub = nh.advertise <geometry_msgs::Twist>("/cmd_vel" , 1000 ) ;


 // Loop at 2Hz until the node is shut down.
 	ros::Rate rate ( 30 ) ;
 	while ( ros::ok ( ) ) 
 	{
 // Create and fill in the message . The other four
 // fields , which are ignored by turtl esim , default to 0.
 		geometry_msgs::Twist msg ;
 		msg.angular.z = -0.1 ;

 // Publish the message .
 		pub.publish ( msg ) ;

 // Send a message to rosout with the details .
 		//ROS_INFO_STREAM( "Sending random velocity command : "<< "linear=" << msg.linear.x<< "angular=" << msg.angular.z) ;

 // Wait untilit's time for another iteration .
 		rate.sleep ( ) ;
 	}
}
