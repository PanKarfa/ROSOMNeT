/*
 * Simulation.cpp
 *
 *  Created on: 18. 11. 2015
 *      Author: Vladimir Matena
 */

#include "ROSOMNeT.h"

#include <iostream>
#include <thread>

#include <ros/ros.h>

#include <omnetpp.h>
#include <sectionbasedconfig.h>
#include <inifilereader.h>
#include <fsutils.h>
#include <appreg.h>

using namespace std;
using namespace ros;

Register_GlobalConfigOption(CFGID_CONFIGURATION_CLASS, "configuration-class", CFG_STRING, "", "Part of the Envir plugin mechanism: selects the class from which all configuration information will be obtained. This option lets you replace omnetpp.ini with some other implementation, e.g. database input. The simulation program still has to bootstrap from an omnetpp.ini (which contains the configuration-class setting). The class should implement the cConfigurationEx interface.");

ROSOMNeT ROSOMNeT::instance;

// Helper macro
#define CREATE_BY_CLASSNAME(var,classname,baseclass,description) \
     baseclass *var ## _tmp = (baseclass *) createOne(classname); \
     var = dynamic_cast<baseclass *>(var ## _tmp); \
     if (!var) \
         throw cRuntimeError("Class \"%s\" is not subclassed from " #baseclass, (const char *)classname);

ROSOMNeT& ROSOMNeT::getInstance() {
	return instance;
}

ROSOMNeT::ROSOMNeT() {}

ROSOMNeT::~ROSOMNeT() {}

NodeHandle & ROSOMNeT::getROSNode() {
	return *rosNode;
}

void ROSOMNeT::stopROS() {
	cout << "Shutting down ROS" << endl;
	shutdown();
	cout << "Joining ROS thread" << endl;
	rosThread->join();

	delete rosNode;
	delete rosThread;
}

void ROSOMNeT::runROSNode() {
	cout << "Running ROS node" << endl;
	// Initialize ROS
	init(M_string(), "ROSOMNeT");

	rosNode = new NodeHandle();

	rosThread = new thread(&ROSOMNeT::rosMain, this);
}

void ROSOMNeT::rosMain() {
	cout << "ROS Main spinning on ROS" << endl;
	spin();
	cout << "No more spinning on ROS" << endl;
}

void ROSOMNeT::runSimulation(string configFileName) {
	std::cout << "Running simulation based on configuration file: " << configFileName << std::endl;

	cStaticFlag dummy;
	// SETUPplatdefs.h
	cSimulation *simulationobject = NULL;
	cRunnableEnvir *app = NULL;
	SectionBasedConfiguration *bootconfig = NULL;
	cConfigurationEx *configobject = NULL;
	try {
		// Construct global lists
		cout << "Running startup code fragments" << endl;
		CodeFragments::executeAll(CodeFragments::STARTUP);

		// First, load the ini file. It might contain the name of the user interface to instantiate.
		cout << "Loading configuration ini file" << endl;
		InifileReader *inifile = new InifileReader();
		inifile->readFile(configFileName.c_str());

		// Activate [General] section so that we can read global settings from it
		cout << "Activating general section in configuration" << endl;
		bootconfig = new SectionBasedConfiguration();
		bootconfig->setConfigurationReader(inifile);
		bootconfig->activateConfig("General", 0);

		// Create custom configuration object, if needed.
		cout << "Creating configuration object" << endl;
		string configclass = bootconfig->getAsString(CFGID_CONFIGURATION_CLASS);
		if (configclass.empty()) {
			configobject = bootconfig;
		} else {
			// Create custom configuration object
			CREATE_BY_CLASSNAME(configobject, configclass.c_str(), cConfigurationEx, "configuration");
			configobject->initializeFrom(bootconfig);
			configobject->activateConfig("General", 0);
			delete bootconfig;
			bootconfig = NULL;

			// Load libs from this config as well
			std::vector < std::string > libs = configobject->getAsFilenames(CFGID_LOAD_LIBS);
			for (int k = 0; k < (int) libs.size(); k++)
				loadExtensionLibrary(libs[k].c_str());
		}

		// Validate the configuration
		cout << "Validating configuration" << endl;
		configobject->validate();

		// Lookup specified user interface
		cout << "Setting user interface" << endl;
		cOmnetAppRegistration *appreg = static_cast<cOmnetAppRegistration *>(omnetapps.getInstance()->lookup("Cmdenv"));
		if (!appreg) {
			throw cRuntimeError("Could not start user interface Cmdenv");
		}

		// Create interface object.
		printf("Setting up %s...\n", appreg->getName());
		app = appreg->createOne();
	} catch (exception& e) {
		fprintf(stderr, "\n<!> Error during startup: %s.\n", e.what());
		if (app) {
			delete app;
			app = NULL;
		} else {
			delete bootconfig;
		}
	}

	// RUN
	try {
		if (app) {
			simulationobject = new cSimulation("simulation", app);
			cSimulation::setActiveSimulation(simulationobject);
			app->run(0, NULL, configobject);
		}
	} catch (std::exception& e) {
		fprintf(stderr, "\n<!> %s.\n", e.what());
	}

	// SHUTDOWN
	cSimulation::setActiveSimulation (NULL);
	delete simulationobject;  // will delete app as well
}

