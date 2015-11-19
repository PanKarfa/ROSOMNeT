/*
 * ROSSyncApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: vlada
 */

#ifndef SRC_ROSSYNCAPPLICATION_H
#define SRC_ROSSYNCAPPLICATION_H

#include <omnetpp.h>

#define ROS_SYNC_MESSAGE "@ROSSyncMessage@"

class ROSSyncApplication: public cSimpleModule {
public:
	const double TIME_STEP = 0.100;
	ROSSyncApplication();

private:
	void initialize(int stage);
	void handleMessage(cMessage *msg);

	cMessage *syncMsg = new cMessage(ROS_SYNC_MESSAGE);
};

#endif /* SRC_ROSSYNCAPPLICATION_H */
