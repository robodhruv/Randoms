#include <boost/bind.hpp>
#include <gazebo/gazebo.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/common/common.hh>
#include <gazebo/common/Plugin.hh>
#include <ros/ros.h>
#include <msi_rover/RoverState.h>
#include <msi_rover/ObstacleScan.h>
#include <msi_rover/NavigationDirectives.h>
#include <msi_rover/attitude.h>

namespace gazebo {

	class RoverSimulator : public ModelPlugin
	{
	
		public:
		void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf) {
		
			// Make sure the ROS node for Gazebo has already been initialized
			if (!ros::isInitialized()) {
				ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin. "
				<< "Load the Gazebo system plugin 'libgazebo_ros_api_plugin.so' in the gazebo_ros package)");
				return;
			}
			
			ros::NodeHandle _nh("rover");
			nav_subs = _nh.subscribe("/rover/navigation_proc", 1, &RoverSimulator::RoverStateCallback, this);
			
			// Save Global Pointers
			this->rover = _model;
			this->sdf = _sdf;
			this->world = rover->GetWorld();

			this->fl_wheel_joint = rover->GetJoint("Left_Front_Tyre_Joint");
			this->ml_wheel_joint = rover->GetJoint("Left_Middle_Tyre_Joint");
			this->rl_wheel_joint = rover->GetJoint("Left_Rear_Tyre_Joint");
			this->fr_wheel_joint = rover->GetJoint("Right_Front_Tyre_Joint");
			this->mr_wheel_joint = rover->GetJoint("Right_Middle_Tyre_Joint");
			this->rr_wheel_joint = rover->GetJoint("Right_Rear_Tyre_Joint");

			this->fl_steer_joint  = rover->GetJoint("Left_Front_Steering_Joint");
			this->rl_steer_joint  = rover->GetJoint("Left_Rear_Steering_Joint");
			this->fr_steer_joint  = rover->GetJoint("Right_Front_Steering_Joint");
			this->rr_steer_joint  = rover->GetJoint("Right_Rear_Steering_Joint");

			this->fl_wheel_rpm  = 0;
			this->ml_wheel_rpm  = 0;
			this->rl_wheel_rpm  = 0;
			this->fr_wheel_rpm  = 0;
			this->mr_wheel_rpm  = 0;
			this->rr_wheel_rpm  = 0;
			
			this->fl_steer_ang  = 0;
			this->rl_steer_ang  = 0;
			this->fr_steer_ang  = 0;
			this->rr_steer_ang  = 0;
			
			this->fl_steer_force=0;
			this->rl_steer_force=0;
			this->rr_steer_force=0;
			this->fr_steer_force=0;
			
			this->fl_wheel_joint->SetMaxForce(0,1000);
			this->ml_wheel_joint->SetMaxForce(0,1000);
			this->rl_wheel_joint->SetMaxForce(0,1000);
			this->fr_wheel_joint->SetMaxForce(0,1000);
			this->mr_wheel_joint->SetMaxForce(0,1000);
			this->rr_wheel_joint->SetMaxForce(0,1000);

			this->_spinnerThread = boost::thread(boost::bind(&RoverSimulator::LoadThread, this));
		}

		void LoadThread() {
		
			if(!this->rover) {
				ROS_FATAL_STREAM("Rover plugin requires a rover as its parent.");
				return;
			}
			
			this->_updateConnection = event::Events::ConnectWorldUpdateBegin(boost::bind(&RoverSimulator::OnUpdate, this, _1));
			ROS_INFO("Rover plugin for ROS successfully loaded :)");

			while(ros::ok()) {
				ros::spinOnce();
			}
		}

		void OnUpdate(const common::UpdateInfo &) {
			if (nav_subs.getNumPublishers() != 0) {
				this->fl_wheel_joint->SetVelocity(0,fl_wheel_rpm);
				this->ml_wheel_joint->SetVelocity(0,ml_wheel_rpm);
				this->rl_wheel_joint->SetVelocity(0,rl_wheel_rpm);
				this->rr_wheel_joint->SetVelocity(0,fr_wheel_rpm);
				this->mr_wheel_joint->SetVelocity(0,mr_wheel_rpm);
				this->fr_wheel_joint->SetVelocity(0,rr_wheel_rpm);
				
				this->fl_steer_joint->SetForce(0,fl_steer_force);
				this->rl_steer_joint->SetForce(0,rl_steer_force);
				this->rr_steer_joint->SetForce(0,rr_steer_force);
				this->fr_steer_joint->SetForce(0,fr_steer_force);
			}
			else {
				this->fl_wheel_joint->SetForce(0,0);
				this->ml_wheel_joint->SetForce(0,0);
				this->rl_wheel_joint->SetForce(0,0);
				this->rr_wheel_joint->SetForce(0,0);
				this->mr_wheel_joint->SetForce(0,0);
				this->fr_wheel_joint->SetForce(0,0);
				
				this->fl_steer_joint->SetForce(0,0);
				this->rl_steer_joint->SetForce(0,0);
				this->rr_steer_joint->SetForce(0,0);
				this->fr_steer_joint->SetForce(0,0);
			}
		}
		
		void RoverStateCallback(msi_rover::NavigationDirectives& rx_msg) {
			this->fl_wheel_rpm = rx_msg.fl_wheel_rpm;
			this->ml_wheel_rpm = rx_msg.ml_wheel_rpm;
			this->rl_wheel_rpm = rx_msg.rl_wheel_rpm;
			this->fr_wheel_rpm = rx_msg.fr_wheel_rpm;
			this->mr_wheel_rpm = rx_msg.mr_wheel_rpm;
			this->rr_wheel_rpm = rx_msg.rr_wheel_rpm;
			
			this->fl_steer_ang = rx_msg.fl_steer_ang;
			this->rl_steer_ang = rx_msg.rl_steer_ang;
			this->fr_steer_ang = rx_msg.fr_steer_ang;
			this->rr_steer_ang = rx_msg.rr_steer_ang;
			
			ROS_INFO("Message recieved");
		}

		// Global pointer declarations
		private: physics::ModelPtr rover;
		private: physics::WorldPtr world;
		private: sdf::ElementPtr sdf;
		private: event::ConnectionPtr _updateConnection;
		private: boost::thread _spinnerThread;

		private: physics::JointPtr fl_wheel_joint;
		private: physics::JointPtr ml_wheel_joint;
		private: physics::JointPtr rl_wheel_joint;
		private: physics::JointPtr fr_wheel_joint;
		private: physics::JointPtr mr_wheel_joint;
		private: physics::JointPtr rr_wheel_joint;

		private: physics::JointPtr fl_steer_joint;
		private: physics::JointPtr rl_steer_joint;
		private: physics::JointPtr fr_steer_joint;
		private: physics::JointPtr rr_steer_joint;

		private: float fl_wheel_rpm;
		private: float ml_wheel_rpm;
		private: float rl_wheel_rpm;
		private: float rr_wheel_rpm;
		private: float mr_wheel_rpm;
		private: float fr_wheel_rpm;
		
		private: float fl_steer_ang;
		private: float rl_steer_ang;
		private: float rr_steer_ang;
		private: float fr_steer_ang;
		
		private: float fl_steer_force;
		private: float rl_steer_force;
		private: float rr_steer_force;
		private: float fr_steer_force;

		private: ros::Subscriber nav_subs;
	};

	GZ_REGISTER_MODEL_PLUGIN(RoverSimulator);
}

