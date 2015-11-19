/*
 * Simulation.h
 *
 *  Created on: 18. 11. 2015
 *      Author: Vladimir Matena
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>

class Simulation {
public:
	Simulation(std::string configFileName);

	void runSimulation();

private:
	std::string configFileName;
};

#endif /* SIMULATION_H */
