/*
 * ROSSyncApplication.cpp
 *
 *  Created on: 19. 11. 2015
 *      Author: Vlaimir Matena
 */

#include "ROSSyncApplication.h"

Define_Module(ROSSyncApplication);

using namespace std;

ROSSyncApplication::ROSSyncApplication() {
	cout << "ROSSyncApplication constructor" << endl;
}

void ROSSyncApplication::initialize(int stage) {
	cout << "ROSSyncApplication init " << stage << endl;
}

void ROSSyncApplication::handleMessage(cMessage *msg) {
	cout << "ROSSyncApplication handle message" << endl;
}

