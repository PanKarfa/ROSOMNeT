/*
 * Simulation.cpp
 *
 *  Created on: 18. 11. 2015
 *      Author: Vladimir Matena
 */

#include "Simulation.h"

#include <iostream>

#include <omnetpp.h>
#include <sectionbasedconfig.h>
#include <inifilereader.h>
#include <appreg.h>

USING_NAMESPACE;

Simulation::Simulation(std::string configFileName) :
		configFileName(configFileName) {
}

void Simulation::runSimulation() {
	std::cout << "Running simulation based on configuration file: " << configFileName << std::endl;

	cStaticFlag dummy;
	//
	// SETUPplatdefs.h
	//
	cSimulation *simulationobject = NULL;
	cRunnableEnvir *app = NULL;
	SectionBasedConfiguration *bootconfig = NULL;
	cConfigurationEx *configobject = NULL;
	try {
		// construct global lists
		CodeFragments::executeAll(CodeFragments::STARTUP);

		//
		// First, load the ini file. It might contain the name of the user interface
		// to instantiate.
		//

		InifileReader *inifile = new InifileReader();
		inifile->readFile(configFileName.c_str());

		// activate [General] section so that we can read global settings from it
		bootconfig = new SectionBasedConfiguration();
		bootconfig->setConfigurationReader(inifile);
		bootconfig->activateConfig("General", 0);

		//load libs
/*		std::vector < std::string > libs = bootconfig->getAsFilenames(CFGID_LOAD_LIBS);
		for (int k = 0; k < (int) libs.size(); k++) {
			::printf("Loading %s ...\n", libs[k].c_str());
			loadExtensionLibrary(libs[k].c_str());
		}*/

		//
		// Create custom configuration object, if needed.
		//
	/*	std::string configclass = bootconfig->getAsString(CFGID_CONFIGURATION_CLASS);
		if (configclass.empty()) {
			configobject = bootconfig;
		} else {
			// create custom configuration object
			CREATE_BY_CLASSNAME(configobject, configclass.c_str(), cConfigurationEx, "configuration");
			configobject->initializeFrom(bootconfig);
			configobject->activateConfig("General", 0);
			delete bootconfig;
			bootconfig = NULL;

			// load libs from this config as well
			std::vector < std::string > libs = configobject->getAsFilenames(CFGID_LOAD_LIBS);
			for (int k = 0; k < (int) libs.size(); k++)
				loadExtensionLibrary(libs[k].c_str());
		}*/

		// validate the configuration, but make sure we don't report cmdenv-* keys
		// as errors if Cmdenv is absent; same for Tkenv.
		std::string ignorablekeys;
		if (omnetapps.getInstance()->lookup("Cmdenv") == NULL)
			ignorablekeys += " cmdenv-*";
		if (omnetapps.getInstance()->lookup("Tkenv") == NULL)
			ignorablekeys += " tkenv-*";
		configobject->validate(ignorablekeys.c_str());

		//
		// Choose and set up user interface (EnvirBase subclass). Everything else
		// will be done by the user interface class.
		//
		const char * appname = "cmdenv";
/*		if (appname == NULL || opp_strcmp(appname, "") == 0)
			appname = configobject->getAsString(CFGID_USER_INTERFACE).c_str();*/
		cOmnetAppRegistration *appreg = NULL;
		if (!(appname == NULL || opp_strcmp(appname, "") == 0)) {
			// look up specified user interface
			appreg = static_cast<cOmnetAppRegistration *>(omnetapps.getInstance()->lookup(appname));
			if (!appreg) {
				::printf("\n"
						"User interface '%s' not found (not linked in or loaded dynamically).\n"
						"Available ones are:\n", appname);
				cRegistrationList *a = omnetapps.getInstance();
				for (int i = 0; i < a->size(); i++)
					::printf("  %s : %s\n", a->get(i)->getName(), a->get(i)->info().c_str());

				throw cRuntimeError("Could not start user interface '%s'", appname);
			}
		} else {
			// user interface not explicitly selected: pick one from what we have
			appreg = cOmnetAppRegistration::chooseBest();
			if (!appreg)
				throw cRuntimeError("No user interface (Cmdenv, Tkenv, etc.) found");
		}

		//
		// Create interface object.
		//
		::printf("Setting up %s...\n", appreg->getName());
		app = appreg->createOne();
	} catch (std::exception& e) {
		::fprintf(stderr, "\n<!> Error during startup: %s.\n", e.what());
		if (app) {
			delete app;
			app = NULL;
		} else {
			delete bootconfig;
		}
	}

	//
	// RUN
	//
	try {
		if (app) {
			simulationobject = new cSimulation("simulation", app);
			cSimulation::setActiveSimulation(simulationobject);
			app->run(0, NULL, configobject);
		}
	} catch (std::exception& e) {
		::fprintf(stderr, "\n<!> %s.\n", e.what());
	}

	//
	// SHUTDOWN
	//
	cSimulation::setActiveSimulation (NULL);
	delete simulationobject;  // will delete app as well
}

