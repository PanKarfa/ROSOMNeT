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
	~ROSForwarderApplication();

private:
	const char* ROS_MANET_PACKET = "@ROSManetPacket@";
	const char* START_MESSAGE = "START_MESSAGE";

	void initialize(int stage);
	int numInitStages() const { return 2; }
	void handleMessage(cMessage *msg);

	cMessage* startMessage;

    int lower802154LayerIn;
    int lower802154LayerOut;
};

#endif /* SRC_ROSFORWARDERAPPLICATION_H */
