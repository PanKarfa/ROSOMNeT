/*
 * ROSSyncApplication.cpp
 *
 *  Created on: 19. 11. 2015
 *      Author: Vlaimir Matena
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>

#include "ROSSyncApplication.h"

Define_Module (ROSSyncApplication);

using namespace std;

ROSSyncApplication::ROSSyncApplication() :
		rosomnet(ROSOMNeT::getInstance()) {
	cout << "ROSSyncApplication constructor" << endl;
	syncMsg = new cMessage(ROS_SYNC_MESSAGE);

}

ROSSyncApplication::~ROSSyncApplication() {
	cout << "ROSSyncApplication destructor" << endl;
//	delete syncMsg;
}

void ROSSyncApplication::initialize(int stage) {
	cout << "ROSSyncApplication initialize " << stage << endl;
	switch (stage) {
	case 0:
		initializeStage0();
		break;
	case 1:
		initializeStage1();
		break;
	}
}

void ROSSyncApplication::initializeStage0() {
	cout << "Scheduling message at 0" << endl;
	scheduleAt(0, syncMsg);

	// Start listening to ROS clock topic
	cout << "Subscribing " << CLOCK_TOPIC << " topic" << endl;
	clockSubscriber = rosomnet.getROSNode().subscribe(CLOCK_TOPIC, CLOCK_QUEUE_LENGTH,
			&ROSSyncApplication::clockCallback, this);
}

void ROSSyncApplication::initializeStage1() {

}

void ROSSyncApplication::clockCallback(const rosgraph_msgs::Clock &msg) {
	cout << "Clock callback" << endl;
}

void ROSSyncApplication::handleMessage(cMessage *msg) {
	// Handle time synchronization message
	if (msg == syncMsg) {
		cout << "ROSSyncPoint at " << setw(8) << fixed << simTime() << " s" << endl;

		// Sync with ROS
		// TODO: This is very primitive code, replace with ROS clock sync
		usleep(TIME_STEP * 1000000);

		// Schedule next sync invocation
		scheduleAt(simTime() + TIME_STEP, syncMsg);
	} else {
		cerr << "ROS sync application received unexpected message" << endl;
	}
}
