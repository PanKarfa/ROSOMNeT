/*
 * ROSForwarderApplication.cpp
 *
 *  Created on: 19. 11. 2015
 *      Author: vlada
 */

#include <inet/linklayer/common/MACAddress.h>
#include <inet/linklayer/common/SimpleLinkLayerControlInfo.h>

#include "ROSForwarderApplication.h"
#include "IEEE802154_m.h"

Define_Module(ROSForwarderApplication);

using namespace std;
using namespace inet;

ROSForwarderApplication::ROSForwarderApplication() {
	cout << "ROSForwarderApplication constructor" << endl;
	startMessage = new cMessage(START_MESSAGE);
}

ROSForwarderApplication::~ROSForwarderApplication() {
	cout << "ROSForwarderApplication destructor" << endl;
	delete startMessage;
}

void ROSForwarderApplication::initialize(int stage) {
	cout << "ROSForwarderApplication init " << stage << endl;

	switch(stage) {
	case 0:
		scheduleAt(1, startMessage);
		break;
	case 1:
		lower802154LayerIn = findGate("lower802154LayerIn");
		lower802154LayerOut = findGate("lower802154LayerOut");
		break;
	}
}

void ROSForwarderApplication::handleMessage(cMessage *msg) {
	cout << "ROSForwarderApplication handle message" << endl;

	if(msg == startMessage) {
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
	} else if(opp_strcmp(msg->getName(), ROS_MANET_PACKET) == 0) {
		cout << "Received MANET packet: " << endl;
		cout << ">> id:		" << msg->getId() << endl;
		cout << ">> time:	" << simTime() << endl;

		IEEE802154Packet *ieee802154Packet = check_and_cast<IEEE802154Packet*>(msg);
		char *data = new char[ieee802154Packet->getDataArraySize()];
		for(unsigned int i = 0; i < ieee802154Packet->getDataArraySize(); ++i) {
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
