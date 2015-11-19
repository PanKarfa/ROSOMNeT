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
	ROSSyncApplication();

private:
	void initialize(int stage);
	int numInitStages() const { return 2; }
	void handleMessage(cMessage *msg);
};

#endif /* SRC_ROSSYNCAPPLICATION_H */
