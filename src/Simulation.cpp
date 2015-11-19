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
#include <fsutils.h>
#include <appreg.h>

using namespace std;

Register_GlobalConfigOption(CFGID_LOAD_LIBS, "load-libs", CFG_FILENAMES, "", "A space-separated list of dynamic libraries to be loaded on startup. The libraries should be given without the `.dll' or `.so' suffix -- that will be automatically appended.");
Register_GlobalConfigOption(CFGID_CONFIGURATION_CLASS, "configuration-class", CFG_STRING, "", "Part of the Envir plugin mechanism: selects the class from which all configuration information will be obtained. This option lets you replace omnetpp.ini with some other implementation, e.g. database input. The simulation program still has to bootstrap from an omnetpp.ini (which contains the configuration-class setting). The class should implement the cConfigurationEx interface.");
Register_GlobalConfigOption(CFGID_USER_INTERFACE, "user-interface", CFG_STRING, "", "Selects the user interface to be started. Possible values are Cmdenv and Tkenv. This option is normally left empty, as it is more convenient to specify the user interface via a command-line option or the IDE's Run and Debug dialogs. New user interfaces can be defined by subclassing cRunnableEnvir.");

// helper macro
#define CREATE_BY_CLASSNAME(var,classname,baseclass,description) \
     baseclass *var ## _tmp = (baseclass *) createOne(classname); \
     var = dynamic_cast<baseclass *>(var ## _tmp); \
     if (!var) \
         throw cRuntimeError("Class \"%s\" is not subclassed from " #baseclass, (const char *)classname);

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

		// Load libs
		cout << "Loading libraries" << endl;
		std::vector < std::string > libs = bootconfig->getAsFilenames(CFGID_LOAD_LIBS);
		for (int k = 0; k < (int) libs.size(); k++) {
			::printf("Loading %s ...\n", libs[k].c_str());
			loadExtensionLibrary(libs[k].c_str());
		}

		// Create custom configuration object, if needed.
		cout << "Creating configuration object" << endl;
		std::string configclass = bootconfig->getAsString(CFGID_CONFIGURATION_CLASS);
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
		}

		// Validate the configuration, but make sure we don't report cmdenv-* keys as errors if Cmdenv is absent; same for Tkenv.
		cout << "Validating configuration" << endl;
		std::string ignorablekeys;
		if (omnetapps.getInstance()->lookup("Cmdenv") == NULL) {
			ignorablekeys += " cmdenv-*";
		}
		if (omnetapps.getInstance()->lookup("Tkenv") == NULL) {
			ignorablekeys += " tkenv-*";
		}
		configobject->validate(ignorablekeys.c_str());

		// Choose and set up user interface (EnvirBase subclass). Everything else
		// will be done by the user interface class.
		cout << "Setting user interface" << endl;
		const char * appname = "Cmdenv";
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

