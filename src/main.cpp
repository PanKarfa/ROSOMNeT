#include <iostream>
#include <fstream>

#include <std_msgs/String.h>

#include "ROSOMNeT.h"

using namespace std;
using namespace ros;

const string CONFIG_TEMPLATE = "omnetpp.ini.template";
const string CONFIG_GENERATED = "omnetpp.ini.generated";

int main(int argc, char** argv) {
	if(argc != 3) {
		cout << "Please provide number of nodes as the first parameter and map name second one" << endl;
		return -1;
	}

	string numNodes(argv[1]);
	string mapName(argv[2]);

	cout << "ROSOMNeT++ creating configuration with '" << numNodes << "' nodes." << endl;

	// Create configuration from template
	ifstream templateFile(CONFIG_TEMPLATE);
	ofstream generatedConfig(CONFIG_GENERATED);
	generatedConfig << templateFile.rdbuf();

	// Write number of nodes
	generatedConfig << "**.numNodes = " << numNodes << endl;
	// Write environment model
	generatedConfig << "**.config = xmldoc(\"" << mapName << ".xml\")" << endl;

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
