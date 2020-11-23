#include <ros/ros.h>
#include <ledcontrol/OpenInterface.h>
#include <std_msgs/Int16MultiArray.h>
#include <string>

std::string port;
irobot::OpenInterface * myobj;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "embedded_light_node");

	ROS_INFO("embedded for ROS %.2f", 1.0);
		
	std_msgs::Int16MultiArray msg;

	ros::NodeHandle n;

	XmlRpc::XmlRpcValue Opcode_List;
	int Opcode_Length;
	int Opcode_Buffer_Size;

	n.getParam("Opcode_List", Opcode_List);
	n.getParam("Opcode_Length", Opcode_Length);
	n.getParam("Opcode_Buffer_Size", Opcode_Buffer_Size);

//end of modification
	
	n.param<std::string>("embedded/port", port, "/dev/ttyUSB0");
	
	myobj = new irobot::OpenInterface(port.c_str());
	
	ros::Publisher analog_pub = n.advertise<std_msgs::Int16MultiArray>("analog_data", 50);
	
	if( myobj->openSerialPort(true) == 0) ROS_INFO("Connected to embedded base robot.");
	else
	{
		ROS_FATAL("Could not connect to embedded base robot.");
		ROS_BREAK();
	}

	ros::Time current_time, last_time;
	current_time = ros::Time::now();
	last_time = ros::Time::now();
	
	myobj->setStreamRate(500.0);
	myobj->flushSerialPort();
	myobj->flushSerialPort();
	myobj->startStream();

	ros::Rate r(10.0);

	while(n.ok())
	{
		current_time = ros::Time::now();
		
		myobj->streamSensorPackets();
		     
	     msg.data.clear();
	     for (int i=0;i<8;i++)
	    	    	msg.data.push_back(myobj->analog_data[i]);
	    	    		    	    		
		//publish the message
		analog_pub.publish(msg);

		ros::spinOnce();
		r.sleep();
	}
	myobj->stopStream();
	myobj->closeSerialPort();
}

// EOF
