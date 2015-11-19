/*
 * ROSForwarderApplication.cpp
 *
 *  Created on: 19. 11. 2015
 *      Author: vlada
 */

#include "ROSForwarderApplication.h"

Define_Module(ROSForwarderApplication);

using namespace std;

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

		cout << "Sending packet" << endl;
		send(new cPacket(), lower802154LayerOut);
	}
}
