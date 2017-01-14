/*------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------  Reference Table  --------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------*/
/*                                                                                                            *
 *  Value        | Variable             | Type               | Reference                                      *
 * ---------------------------------------------------------------------------------------------------------- *
STATE
 *  Last updated | state.timestamp      | time_t             | http://www.cplusplus.com/reference/ctime/
 *  Position     | state.position       | math::Vector3      | http://osrf-distributions.s3.amazonaws.com/gazebo/api/4.0.0/classgazebo_1_1math_1_1Vector3.html
 *  Yaw          | state.attitude.yaw   | float              |
 *  Pitch        | state.attitude.pitch | float              |
 *  Roll         | state.attitude.roll  | float              |
 *  Velocity     | state.velocity       | math::Vector3      | http://osrf-distributions.s3.amazonaws.com/gazebo/api/4.0.0/classgazebo_1_1math_1_1Vector3.html
 *  Ang velocity | state.ang_velocity   | math::Vector3      | http://osrf-distributions.s3.amazonaws.com/gazebo/api/4.0.0/classgazebo_1_1math_1_1Vector3.html
SCAN
 *  Last updated | scan.timestamp       | time_t             | http://www.cplusplus.com/reference/ctime/
 *  Min yaw      | scan.min_yaw         | float              |
 *  Max yaw      | scan.max_yaw         | float              |
 *  Delta yaw    | scan.delta_yaw       | float              |
 *  Min range    | scan.min_range       | float              |
 *  Max range    | scan.max_range       | float              |
 *  Ranges       | scan.ranges          | std::vector<float> | http://www.cplusplus.com/reference/vector/vector/
DESTINATION
 *  Last updated | dest.timestamp       | time_t             | http://www.cplusplus.com/reference/ctime/
 *  Position     | dest.position        | math::Vector2d     | http://osrf-distributions.s3.amazonaws.com/gazebo/api/4.0.0/classgazebo_1_1math_1_1Vector2d.html
 *  Yaw          | dest.yaw             | float              |
 *  Overridable  | dest.overrideable    | bool	             |
 *
/*------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------*/
/*
 *  FUNCTIONS
 *
 *  float getRange( float yaw );                //Linear interpolated range in meters for a given yaw in radians
 *                                              // -1 if error / empty
 *  Configuration.get<bool>("rover.OpenCVNav"); //Read configuration from msi_rover/config/navigation_proc.xml
 */
/*------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------*/

#include <navigation_proc.hpp>

#define DIST_MIN 1.0

msi_rover::NavigationDirectives tx_msg;

float destYaw, angVelocity, linVelocity, dist, initDist, thisDestYaw;
std::stack<struct DestStruct> destStack;

float absolute(float x) {
	return (x>=0)?x:-x;
}

void navigateTo(std::stack<struct DestStruct> destStack) {
	if(!destStack.empty()) {
		struct DestStruct thisDest;
		thisDest = destStack.top();
		
		dist=sqrt(pow((thisDest.position.y-state.position.y),2)+pow((thisDest.position.x-state.position.x),2));
		thisDestYaw = atan2(thisDest.position.y-state.position.y, thisDest.position.x-state.position.x);
	
		if(absolute(thisDestYaw-state.attitude.yaw)>PI/9 && dist>0.5) {
			angVelocity = -(10/(2*PI))*(thisDestYaw-state.attitude.yaw);
			linVelocity = 0;
		}
		else {
			if(dist>DIST_MIN) {
				angVelocity = -(10/(2*PI))*(thisDestYaw-state.attitude.yaw);
				linVelocity = 5*(1-exp(-2*dist))/(1+thisDestYaw-state.attitude.yaw);	
			}
			else if(absolute(thisDest.yaw-state.attitude.yaw) > PI/9 && destStack.size == 1) {
				angVelocity = -(30/(2*PI))*(thisDest.yaw-state.attitude.yaw);
				linVelocity = 0;
			}
			else {
				destStack.pop();
				angVelocity = 0;
				linVelocity = 0;
			}
		}
		else {
			angVelocity = 0;
			linVelocity = 0;
		}
	
		tx_msg.fl_wheel_rpm=linVelocity+angVelocity;
		tx_msg.fr_wheel_rpm=linVelocity-angVelocity;
		tx_msg.rl_wheel_rpm=linVelocity+angVelocity;
		tx_msg.rr_wheel_rpm=linVelocity-angVelocity;
	
		rover.publish(tx_msg);
	}
	else {
		ROS_INFO("Done!");
		ros::shutdown();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Load() {
	dest.position.x = -6;
	dest.position.y = 6;
	dest.yaw = 0;
	init_state.position.x = 0;
	init_state.position.y = 0;
	init_state.position.z = 0;
	
	tx_msg.fl_steer_ang=0;
	tx_msg.fr_steer_ang=0;
	tx_msg.rl_steer_ang=0;
	tx_msg.rr_steer_ang=0;
	
	tx_msg.ml_wheel_rpm=0;
	tx_msg.mr_wheel_rpm=0;
	
	tx_msg.short_fl_wheel=0;
	tx_msg.short_fr_wheel=0;
	tx_msg.short_ml_wheel=0;
	tx_msg.short_mr_wheel=0;
	tx_msg.short_rl_wheel=0;
	tx_msg.short_rr_wheel=0;
	
	destStack.push(dest);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Loop() {
	navigateTo(destStack);
}
