/*
 * Simulation.cpp
 *
 *  Created on: 18. 11. 2015
 *      Author: Vladimir Matena
 */

#include "Simulation.h"

#include <iostream>

Simulation::Simulation(std::string configFileName):configFileName(configFileName) {
}

void Simulation::runSimulation() {
	std::cout << "Running simulation based on configuration file: " << configFileName << std::endl;

}

