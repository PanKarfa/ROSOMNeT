/*
 * ROSForwarderApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: Vladimir Matena
 */

#ifndef SRC_ROSFORWARDERAPPLICATION_H
#define SRC_ROSFORWARDERAPPLICATION_H

#include <string>
#include <queue>

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <beeclickarm_messages/IEEE802154ReceivedPacket.h>
#include <beeclickarm_messages/IEEE802154BroadcastPacket.h>
#include <beeclickarm_messages/IEEE802154BroadcastPacket.h>

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
	const char* TIMER_MESSAGE = "@TIMER_MESSAGE@";
	const string TRUTH_POSE_TOPIC = "base_pose_ground_truth";
	const string PACKET_SENDER_SERVICE_NAME = "MRF24J40/broadcast_packet";
	const string RECEIVED_PACKET_TOPIC = "MRF24J40/received_packets";
	const long TOPIC_QUEUE_LENGTH = 1000;
	const double PACKET_TRANSMIT_INTERVAL = 0.100;

	static int instanceCounter;

	const string nameSpace;
	ROSOMNeT &rosomnet;
	void initialize(int stage);
	void initializeStage0();
	void initializeStage1();
	int numInitStages() const {
		return 2;
	}
	void handleMessage(cMessage *msg);
	IMobility* getMobilityModule();
	cPacket* createFromData(const vector<uint8_t>& data);
	static void printData(const vector<uint8_t>& data);

	void truthPoseCallback(const nav_msgs::Odometry& msg);
	bool sendPacketCallback(beeclickarm_messages::IEEE802154BroadcastPacket::Request& request,
			beeclickarm_messages::IEEE802154BroadcastPacket::Response& response);

	cMessage* timerMessage;

	queue<vector<uint8_t> > messageToProcess;

	int lower802154LayerIn;
	int lower802154LayerOut;

	Subscriber truthPoseSubscriber;
	Publisher receivedPacketPublisher;
	ServiceServer packetSenderService;
};

#endif /* SRC_ROSFORWARDERAPPLICATION_H */
