#include "ros/ros.h"
#include "ledcontrol/ledsrv.h"
#include <cstdlib>


int main(int argc, char **argv)
{
  ros::init(argc, argv, "led_client");
  if (argc != 2)
  {
    ROS_INFO("usage: led_client 1/0");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<ledcontrol::ledsrv>("led");
  ledcontrol::ledsrv srv;
  srv.request.driveled = atoi(argv[1]);

  if (client.call(srv))
  {
    ROS_INFO("Data send: %ld", (long int)srv.request.driveled);
  }
  else
  {
    ROS_ERROR("Failed to call service led control");
    return 1;
  }

  return 0;
}

