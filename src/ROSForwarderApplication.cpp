/*
 * ROSForwarderApplication.cpp
 *
 *  Created on: 19. 11. 2015
 *      Author: vlada
 */

#include <inet/linklayer/common/MACAddress.h>
#include <inet/linklayer/common/SimpleLinkLayerControlInfo.h>

#include <string>

#include "ROSForwarderApplication.h"
#include "IEEE802154_m.h"
#include "CustomMobility.h"

Define_Module (ROSForwarderApplication);

using namespace std;
using namespace inet;

int ROSForwarderApplication::instanceCounter = 0;

ROSForwarderApplication::ROSForwarderApplication():nameSpace("/robot_" + to_string(instanceCounter++) + "/") {
	cout << "ROSForwarderApplication constructor: " << nameSpace << endl;
	startTry1Message = new cMessage(START_MESSAGE);
	startTry2Message = new cMessage(START_MESSAGE);
}

ROSForwarderApplication::~ROSForwarderApplication() {
	cout << "ROSForwarderApplication destructor" << endl;
//	delete startTry1Message;
//	delete startTry2Message;
}

void ROSForwarderApplication::initialize(int stage) {
	cout << "ROSForwarderApplication initialization: " << stage << endl;

	switch (stage) {
	case 0:
		initializeStage1();
		break;
	case 1:
		initializeStage2();
		break;
	}
}

void ROSForwarderApplication::initializeStage1() {
	scheduleAt(1, startTry1Message);
	scheduleAt(2, startTry2Message);
}

void ROSForwarderApplication::initializeStage2() {
	lower802154LayerIn = findGate("lower802154LayerIn");
	lower802154LayerOut = findGate("lower802154LayerOut");
}

void ROSForwarderApplication::handleMessage(cMessage *msg) {
	cout << "ROSForwarderApplication handle message" << endl;

	if(msg == startTry2Message) {
		setPosition(0, 0, 0);
	}

	if (msg == startTry1Message || msg == startTry2Message) {
		cout << "Start message received" << endl;

		cout << "Sending packet at " << simTime() << endl;
		IEEE802154Packet *packet = new IEEE802154Packet(ROS_MANET_PACKET);

		packet->setData(0, 'h');
		packet->setData(1, 'e');
		packet->setData(2, 'l');
		packet->setData(3, 'l');
		packet->setData(4, 'o');

		SimpleLinkLayerControlInfo* ctrl = new SimpleLinkLayerControlInfo();
		ctrl->setDest(MACAddress::BROADCAST_ADDRESS);
		packet->setControlInfo(ctrl);

		send(packet, lower802154LayerOut);
	} else if (opp_strcmp(msg->getName(), ROS_MANET_PACKET) == 0) {
		cout << "Received MANET packet: " << endl;
		cout << ">> id:		" << msg->getId() << endl;
		cout << ">> time:	" << simTime() << endl;

		IEEE802154Packet *ieee802154Packet = check_and_cast<IEEE802154Packet*>(msg);
		char *data = new char[ieee802154Packet->getDataArraySize()];
		for (unsigned int i = 0; i < ieee802154Packet->getDataArraySize(); ++i) {
			data[i] = ieee802154Packet->getData(i);
		}
		cout << ">> data:	" << hex << data << endl;
	} else {
		cout << "Received unknown message name: \"" << msg->getName() << "\" at: " << simTime() << endl;
		cout << "info: " << msg->info() << endl;
		cout << "detailed info:" << msg->detailedInfo() << endl;
	}

	delete msg;
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
	IMobility *mobility = check_and_cast<IMobility *>(
			getParentModule()->getSubmodule("mobility"));
	return mobility;
}
