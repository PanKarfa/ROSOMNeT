/*
 * ROSForwarderApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: Vladimir Matena
 */

#ifndef SRC_ROSFORWARDERAPPLICATION_H
#define SRC_ROSFORWARDERAPPLICATION_H

#include <ros/ros.h>

#include <omnetpp.h>
#include <inet/mobility/contract/IMobility.h>

#include <std_msgs/String.h>

using namespace inet;
using namespace ros;

class ROSForwarderApplication: public cSimpleModule {
public:
	ROSForwarderApplication();
	~ROSForwarderApplication();

	void setPosition(double x, double y, double z);

private:
	const char* ROS_MANET_PACKET = "@ROSManetPacket@";
	const char* START_MESSAGE = "START_MESSAGE";

	void initialize(int stage);
	void initializeStage1();
	void initializeStage2();
	int numInitStages() const { return 2; }
	void handleMessage(cMessage *msg);
	IMobility* getMobilityModule();

	cMessage* startTry1Message;
	cMessage* startTry2Message;

    int lower802154LayerIn;
    int lower802154LayerOut;

    Subscriber *chatterSubscriber;

    Subscriber *truthPoseSubscriber;
    Publisher *receivedPacketPublisher;

    void chatterCallback(const std_msgs::String &msg);
};

#endif /* SRC_ROSFORWARDERAPPLICATION_H */
