/*
 * ROSSyncApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: vlada
 */

#ifndef SRC_ROSSYNCAPPLICATION_H
#define SRC_ROSSYNCAPPLICATION_H

#include <omnetpp.h>

class ROSSyncApplication: public cSimpleModule {
public:
	const double TIME_STEP = 0.100;
	ROSSyncApplication();
	~ROSSyncApplication();

private:
	const char* ROS_SYNC_MESSAGE = "@ROSSyncMessage@";

	void initialize(int stage);
	void handleMessage(cMessage *msg);

	cMessage *syncMsg;
};

#endif /* SRC_ROSSYNCAPPLICATION_H */
