/*
 * ROSForwarderApplication.cpp
 *
 *  Created on: 19. 11. 2015
 *      Author: Vladimir Matena
 */

#include <inet/linklayer/common/MACAddress.h>
#include <inet/linklayer/common/SimpleLinkLayerControlInfo.h>

#include <string>
#include <vector>

#include "ROSForwarderApplication.h"
#include "IEEE802154_m.h"
#include "CustomMobility.h"

Define_Module (ROSForwarderApplication);

using namespace std;
using namespace inet;

int ROSForwarderApplication::instanceCounter = 0;

ROSForwarderApplication::ROSForwarderApplication() :
		nameSpace("/robot_" + to_string(instanceCounter++) + "/"), rosomnet(ROSOMNeT::getInstance()) {
	cout << "ROSForwarderApplication constructor: " << nameSpace << endl;
	timerMessage = new cMessage(TIMER_MESSAGE);
}

ROSForwarderApplication::~ROSForwarderApplication() {
	cout << "ROSForwarderApplication destructor" << endl;
// TODO: Properly delete message
//	drop(timerMessage);
//	delete timerMessage;
}

void ROSForwarderApplication::initialize(int stage) {
	cout << "ROSForwarderApplication initialization: " << stage << endl;

	switch (stage) {
	case 0:
		initializeStage0();
		break;
	case 1:
		initializeStage1();
		break;
	}
}

void ROSForwarderApplication::initializeStage0() {
	cout << "Scheduling message at 0" << endl;
	scheduleAt(0, timerMessage);
}

void ROSForwarderApplication::initializeStage1() {
	lower802154LayerIn = findGate("lower802154LayerIn");
	lower802154LayerOut = findGate("lower802154LayerOut");

	// Subscribe to ROS truth pose
	truthPoseSubscriber = rosomnet.getROSNode().subscribe(nameSpace + TRUTH_POSE_TOPIC, TOPIC_QUEUE_LENGTH,
			&ROSForwarderApplication::truthPoseCallback, this);

	// Provide packet sender service
	packetSenderService = rosomnet.getROSNode().advertiseService(nameSpace + PACKET_SENDER_SERVICE_NAME,
			&ROSForwarderApplication::sendPacketCallback, this);

	// Create publisher for received packets
	receivedPacketPublisher = rosomnet.getROSNode().advertise < beeclickarm_messages::IEEE802154ReceivedPacket
			> (nameSpace + RECEIVED_PACKET_TOPIC, TOPIC_QUEUE_LENGTH);
}

void ROSForwarderApplication::truthPoseCallback(const nav_msgs::Odometry &msg) {
	double x = msg.pose.pose.position.x;
	double y = msg.pose.pose.position.y;
	double z = msg.pose.pose.position.z;

	setPosition(x, y, z);

	cout << nameSpace << " now at: (" << x << "," << y << "," << z << ")" << endl;
}

cPacket* ROSForwarderApplication::createFromData(const vector<uint8_t>& data) {
	// Create packet
	IEEE802154Packet *packet = new IEEE802154Packet(ROS_MANET_PACKET);
	packet->setByteLength(data.size());
	for (unsigned int i = 0; i < data.size(); i++) {
		packet->setData(i, data[i]);
	}

	// Attach destination address
	SimpleLinkLayerControlInfo* ctrl = new SimpleLinkLayerControlInfo();
	ctrl->setDest(MACAddress::BROADCAST_ADDRESS);
	packet->setControlInfo(ctrl);

	return packet;
}

bool ROSForwarderApplication::sendPacketCallback(beeclickarm_messages::IEEE802154BroadcastPacket::Request& request,
		beeclickarm_messages::IEEE802154BroadcastPacket::Response& response) {
	vector < uint8_t > data = request.data;

	// Schedule packet to broadcast
	cout << "Queuing packet to be send by next OMNeT++ invocation" << endl;
	cout << ">> data:	";
	printData(data);

	messageToProcess.push(data);

	return true;
}

void ROSForwarderApplication::printData(const vector<uint8_t>& data) {
	for(uint8_t b: data) {
		cout << hex << ((uint32_t)b);
	}
	cout << endl;
}

void ROSForwarderApplication::handleMessage(cMessage *msg) {
	cout << "ROSForwarderApplication handle message" << endl;

	if (msg == timerMessage) {
		while (!messageToProcess.empty()) {
			cout << "Sending queued packet" << endl;
			cPacket* packet = createFromData(messageToProcess.front());
			messageToProcess.pop();
			send(packet, lower802154LayerOut);
		}
		scheduleAt(simTime() + PACKET_TRANSMIT_INTERVAL, timerMessage);
	} else if (opp_strcmp(msg->getName(), ROS_MANET_PACKET) == 0) {
		cout << "Received MANET packet: " << endl;
		cout << ">> id:		" << msg->getId() << endl;
		cout << ">> time:	" << simTime() << endl;

		IEEE802154Packet *ieee802154Packet = check_and_cast<IEEE802154Packet*>(msg);

		cout << "Publishing message to topic" << endl;
		beeclickarm_messages::IEEE802154ReceivedPacket receivedPacketMsg;
		receivedPacketMsg.rssi = 1;
		receivedPacketMsg.fcs = 1;
		receivedPacketMsg.lqi = 1;
		receivedPacketMsg.srcPanId = 1;
		receivedPacketMsg.srcSAddr = 1;
		const unsigned long size = min((int64_t)ieee802154Packet->getDataArraySize(), ieee802154Packet->getByteLength());
		for (unsigned int i = 0; i < size; ++i) {
			receivedPacketMsg.data.push_back(ieee802154Packet->getData(i));
		}

		cout << ">> dataarraysize: " << ieee802154Packet->getDataArraySize() << endl;
		cout << ">> bytelength: " << ieee802154Packet->getByteLength() << endl;
		cout << ">> data:	";
		printData(receivedPacketMsg.data);

		receivedPacketPublisher.publish(receivedPacketMsg);
		cout << "Message published" << endl;
	} else {
		cout << "Received unknown message name: \"" << msg->getName() << "\" at: " << simTime() << endl;
		cout << "info: " << msg->info() << endl;
		cout << "detailed info:" << msg->detailedInfo() << endl;
	}
}

void ROSForwarderApplication::setPosition(const double x, const double y, const double z) {
	CustomMobility *mobility = dynamic_cast<CustomMobility*>(getMobilityModule());
	if (mobility != NULL) {
		Coord currentPosition = mobility->getCurrentPosition();
		Coord newPosition = Coord::ZERO;
		newPosition.x = x;
		newPosition.y = y;
		newPosition.z = z;
		mobility->setCurrentPosition(newPosition);
	} else {
		std::cerr << "Attempt to set position with custom mobility == NULL" << endl;
	}
}

IMobility* ROSForwarderApplication::getMobilityModule() {
	IMobility *mobility = check_and_cast<IMobility *>(getParentModule()->getSubmodule("mobility"));
	return mobility;
}
