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
}

void ROSForwarderApplication::initialize(int stage) {
	cout << "ROSForwarderApplication init " << stage << endl;
}

void ROSForwarderApplication::handleMessage(cMessage *msg) {
	cout << "ROSForwarderApplication handle message" << endl;
}
