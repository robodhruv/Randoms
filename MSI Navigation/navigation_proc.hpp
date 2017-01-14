//-- Common include files --//
#include <ros/ros.h>
#include <ctime>

//-- These files are present in ros_ws/include of the git repository. Have a look at them. --//
#include <math/math.hh>
#include <miscellaneous.hpp>

//-- Message include files. Include correct message headers for messages to work --//
#include <msi_rover/NavigationDirectives.h>
#include <msi_rover/RoverState.h>
#include <msi_rover/ObstacleScan.h>
#include <geometry_msgs/Pose2D.h>
#include <std_msgs/String.h>
#include <sensor_msgs/LaserScan.h>

//-- Data structures required --//
#include <stack>

#define PI 3.14159

//////////////////////////////////////////////////////

//-- Global Variable Declarations --//
boost::property_tree::ptree Configuration;
ros::Publisher  rover;
ros::Subscriber odometry;
ros::Subscriber obstacle;
ros::Subscriber opencv;
ros::Subscriber master;
ros::Subscriber laser;

//////////////////////////////////////////////////////

//-- Structure Definitions and Declarations --//
struct AttitudeStruct
{
  float                 roll;
  float                 pitch;
  float                 yaw;
};

struct StateStruct
{
  time_t                timestamp;
  math::Vector3         position;
  struct AttitudeStruct attitude;
  math::Vector3         velocity;
  math::Vector3         ang_velocity;
} state, init_state;

struct DestStruct
{
  time_t                timestamp;
  math::Vector2d        position;
  float                 yaw;
  bool                  overrideable;
} dest;

struct ScanStruct
{
  time_t                timestamp;
  float                 min_yaw;
  float                 max_yaw;
  float                 min_range;
  float                 max_range;
  float                 delta_yaw;
  std::vector<float>    ranges;
} scan;

struct HeadingObstacleData {
  float minObsYaw;
  float maxObsYaw;
  float minRange;
} heading;

//////////////////////////////////////////////////////
void OdometryUpdateCallback(const msi_rover::RoverState::ConstPtr& _state) {
  std::time(&state.timestamp);
  state.position.x     = _state->position.x;
  state.position.y     = _state->position.y;
  state.position.z     = _state->position.z;
  state.attitude.yaw   = _state->attitude.yaw;
  state.attitude.pitch = _state->attitude.pitch;
  state.attitude.roll  = _state->attitude.roll;
  state.velocity.x     = _state->lin_velocity.x;
  state.velocity.y     = _state->lin_velocity.y;
  state.velocity.z     = _state->lin_velocity.z;
  state.ang_velocity.x = _state->ang_velocity.x;
  state.ang_velocity.y = _state->ang_velocity.y;
  state.ang_velocity.z = _state->ang_velocity.z;
}

//////////////////////////////////////////////////////
void ObstacleUpdateCallback(const msi_rover::ObstacleScan::ConstPtr& _scan) {
/*  std::time(&scan.timestamp);
  scan.min_yaw         = _scan->angle_min;
  scan.max_yaw         = _scan->angle_max;
  scan.delta_yaw       = _scan->angle_increment;
  scan.min_range       = _scan->range_min;
  scan.max_range       = _scan->range_max;
  scan.ranges          = _scan->ranges;*/
}

//////////////////////////////////////////////////////
void HokuyoLaserCallback(const sensor_msgs::LaserScan laser) {
  scan.min_yaw       = laser.angle_min;
  scan.max_yaw       = laser.angle_max;
  scan.delta_yaw = laser.angle_increment;
  //scan.min_range       = laser.range_min;
  //scan.max_range       = laser.range_max;
  scan.ranges.clear();
  for(int index = 0; index < laser.ranges.size(); index++) {
    if ( laser.ranges[index] < scan.min_range || laser.ranges[index] > scan.max_range ) scan.ranges.push_back(-1);
    else scan.ranges.push_back(laser.ranges[index]);
  }
}

//////////////////////////////////////////////////////
void OpenCVCoordUpdateCallback(const geometry_msgs::Pose2D::ConstPtr& _dest);
void MasterCommandCallback(const std_msgs::String::ConstPtr& command);
void Loop();
void Load();

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OpenCVCoordUpdateCallback(const geometry_msgs::Pose2D::ConstPtr& _dest) {
  if ( Configuration.get<bool>("control.OpenCVNav") ) {
    std::time(&dest.timestamp);
    dest.overrideable  = 1;
    dest.position.x    = _dest->x;
    dest.position.y    = _dest->y;
    dest.yaw           = _dest->theta;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MasterCommandCallback(const std_msgs::String::ConstPtr& command)
{
  //------------------------------//
  //------------ TODO ------------//
  //------------------------------//
}

//////////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ros::init(argc, argv, "navigation_node");
  ros::NodeHandle _nh("rover");

  rover    = _nh.advertise<msi_rover::NavigationDirectives>("/rover/navigation_proc", 1);
  odometry = _nh.subscribe("/rover/odometry",      1, OdometryUpdateCallback);
  obstacle = _nh.subscribe("/rover/obstacle_scan", 1, ObstacleUpdateCallback);
  opencv   = _nh.subscribe("/rover/opencv_coords", 5, OpenCVCoordUpdateCallback);
  master   = _nh.subscribe("/rover/base_station",  5, MasterCommandCallback);
  laser	   = _nh.subscribe("/hokuyo_laser",        1, HokuyoLaserCallback);

  std::time(&state.timestamp);

  Misc::LoadXMLConfig("config/map_generation.xml", Configuration);

  std::time(&scan.timestamp);
  scan.min_yaw         = Configuration.get<float>("obstacle_map.min_yaw");
  scan.max_yaw         = Configuration.get<float>("obstacle_map.max_yaw");
  scan.delta_yaw       = Configuration.get<float>("obstacle_map.delta_yaw");
  scan.min_range       = Configuration.get<float>("obstacle_map.min_range");
  scan.max_range       = Configuration.get<float>("obstacle_map.max_range");
  scan.ranges.clear();
  int index = (scan.max_yaw - scan.min_yaw)/scan.delta_yaw;
  while ( index > 0 ) { scan.ranges.push_back(-1); index--; }

  Misc::LoadXMLConfig("config/navigation_proc.xml", Configuration);

  state.position.x     = Configuration.get<float>("initial.position.x");
  state.position.y     = Configuration.get<float>("initial.position.y");
  state.position.z     = Configuration.get<float>("initial.position.z");
  state.attitude.yaw   = Configuration.get<float>("initial.attitude.yaw");
  state.attitude.pitch = Configuration.get<float>("initial.attitude.pitch");
  state.attitude.roll  = Configuration.get<float>("initial.attitude.roll");
  state.velocity.x     = Configuration.get<float>("initial.velocity.x");
  state.velocity.y     = Configuration.get<float>("initial.velocity.y");
  state.velocity.z     = Configuration.get<float>("initial.velocity.z");
  state.ang_velocity.x = Configuration.get<float>("initial.ang_velocity.x");
  state.ang_velocity.y = Configuration.get<float>("initial.ang_velocity.y");
  state.ang_velocity.z = Configuration.get<float>("initial.ang_velocity.z");

  ros::Rate loop_rate(Configuration.get<int>("publishers.navigation_directives.publish_rate"));
  Load();

  ROS_INFO("Navigation processing node successfuly loaded :)");

  while( ros::ok() ) {
    Loop();
    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
