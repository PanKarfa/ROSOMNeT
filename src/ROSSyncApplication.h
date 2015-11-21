/*
 * ROSSyncApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: vlada
 */

#ifndef SRC_ROSSYNCAPPLICATION_H
#define SRC_ROSSYNCAPPLICATION_H

#include <string>
#include <condition_variable>
#include <mutex>

#include <omnetpp.h>

#include <rosgraph_msgs/Clock.h>

#include "ROSOMNeT.h"

using namespace std;
using namespace ros;

class ROSSyncApplication: public cSimpleModule {
public:
	const double TIME_STEP = 0.100;
	const long CLOCK_QUEUE_LENGTH = 1000;

	ROSSyncApplication();
	~ROSSyncApplication();

private:
	const char* ROS_SYNC_MESSAGE = "@ROSSyncMessage@";
	const string CLOCK_TOPIC = "/clock";

	ROSOMNeT &rosomnet;
	Subscriber clockSubscriber;
	cMessage *syncMsg;
	mutex syncMutex;
	condition_variable syncCondition;

	void initialize(int stage);
	void initializeStage0();
	void initializeStage1();
	void handleMessage(cMessage *msg);
	void clockCallback(const rosgraph_msgs::Clock &msg);


};

#endif /* SRC_ROSSYNCAPPLICATION_H */
