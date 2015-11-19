/*
 * ROSForwarderApplication.h
 *
 *  Created on: 19. 11. 2015
 *      Author: Vladimir Matena
 */

#ifndef SRC_ROSFORWARDERAPPLICATION_H
#define SRC_ROSFORWARDERAPPLICATION_H

#include <omnetpp.h>

class ROSForwarderApplication: public cSimpleModule {
public:
	ROSForwarderApplication();

private:
	void initialize(int stage);
	int numInitStages() const { return 2; }
	void handleMessage(cMessage *msg);
};

#endif /* SRC_ROSFORWARDERAPPLICATION_H */
