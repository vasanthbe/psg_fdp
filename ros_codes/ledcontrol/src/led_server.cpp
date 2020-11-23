#include "ros/ros.h"
#include "ledcontrol/ledsrv.h"
#include <ledcontrol/OpenInterface.h>
#include <iostream>

std::string port;
irobot::OpenInterface * hw;

bool led_switch(ledcontrol::ledsrv::Request  &req, ledcontrol::ledsrv::Response &res)
{

  if(req.driveled == 1)
  {
	hw->setLedOn();
	ROS_INFO("LED On");	
  }
  else
  {
	hw->setLedOff();
	ROS_INFO("LED Off");	
  }

  res.drivestat = req.driveled;
  ROS_INFO("request: x=%ld", (long int)res.drivestat);
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "led_server");
  ros::NodeHandle n;

  n.param<std::string>("embedded/port", port, "/dev/ttyUSB0");
	
  hw = new irobot::OpenInterface(port.c_str());

  if( hw->openSerialPort(true) == 0) ROS_INFO("Connected to led board");
  else
  {
	ROS_FATAL("Could not connect to hardware.");
	ROS_BREAK();
  }

// %Tag(SERVICE_SERVER)%
  ros::ServiceServer service = n.advertiseService("led", led_switch);
// %EndTag(SERVICE_SERVER)%

  ros::spin();
  
  hw->closeSerialPort();

  return 0;
}
