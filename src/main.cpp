#include <iostream>
#include <fstream>

#include <std_msgs/String.h>

#include "ROSOMNeT.h"

using namespace std;
using namespace ros;

const string CONFIG_TEMPLATE = "omnetpp.ini.template";
const string CONFIG_GENERATED = "omnetpp.ini.generated";

int main(int argc, char** argv) {
	if(argc != 2) {
		cout << "Please provide number of nodes as the only parameter" << endl;
		return -1;
	}

	string numNodes(argv[1]);

	cout << "ROSOMNeT++ creating configuration with '" << numNodes << "' nodes." << endl;

	ifstream templateFile(CONFIG_TEMPLATE);
	ofstream generatedConfig(CONFIG_GENERATED);
	generatedConfig << templateFile.rdbuf();
	generatedConfig << numNodes << endl;
	templateFile.close();
	generatedConfig.close();

	cout << "ROSOMNeT++ starting up" << endl;
	ROSOMNeT &rosomnet = ROSOMNeT::getInstance();

	cout << "Starting ROS node" << endl;
	rosomnet.runROSNode();
	cout << "Running OMNeT++ simulation" << endl;
	rosomnet.runSimulation(CONFIG_GENERATED);
	cout << "All done" << endl;

	return 0;
}
