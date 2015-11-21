/*
 * ROSForwarderApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: Vladimir Matena
 */

#ifndef SRC_ROSFORWARDERAPPLICATION_H
#define SRC_ROSFORWARDERAPPLICATION_H

#include <string>

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

#include <omnetpp.h>
#include <inet/mobility/contract/IMobility.h>

#include "ROSOMNeT.h"

using namespace inet;
using namespace ros;
using namespace std;

class ROSForwarderApplication: public cSimpleModule {
public:
	ROSForwarderApplication();
	~ROSForwarderApplication();

	void setPosition(double x, double y, double z);

private:
	const char* ROS_MANET_PACKET = "@ROSManetPacket@";
	const char* START_MESSAGE = "START_MESSAGE";
	const string TRUTH_POSE_TOPIC = "base_pose_ground_truth";
	const long TOPIC_QUEUE_LENGTH = 1000;

	static int instanceCounter;

	const string nameSpace;
	ROSOMNeT &rosomnet;
	void initialize(int stage);
	void initializeStage0();
	void initializeStage1();
	int numInitStages() const { return 2; }
	void handleMessage(cMessage *msg);
	IMobility* getMobilityModule();
	void truthPoseCallback(const nav_msgs::Odometry &msg);

	cMessage* startTry1Message;
	cMessage* startTry2Message;

    int lower802154LayerIn;
    int lower802154LayerOut;

    Subscriber truthPoseSubscriber;
    Publisher receivedPacketPublisher;
};

#endif /* SRC_ROSFORWARDERAPPLICATION_H */
