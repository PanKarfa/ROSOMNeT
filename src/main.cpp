#include <iostream>

#include <std_msgs/String.h>

#include "ROSNode.h"
#include "ROSOMNeT.h"

using namespace std;
using namespace ros;

int main() {
	cout << "ROSOMNeT++ starting up" << endl;

	ROSOMNeT &rosomnet = ROSOMNeT::getInstance();

	cout << "Starting ROS node" << endl;
	rosomnet.runROSNode();
	cout << "Running OMNeT++ simulation" << endl;
	rosomnet.runSimulation("omnetpp.ini");
	cout << "All done" << endl;

	return 0;
}
