/*
 * Simulation.h
 *
 *  Created on: 18. 11. 2015
 *      Author: Vladimir Matena
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include <thread>

#include <ros/ros.h>

using namespace std;
using namespace ros;

class ROSOMNeT {
public:
	ROSOMNeT();
	~ROSOMNeT();

	void runROSNode();
	void runSimulation(string configFileName);

	void stopROS();

	static ROSOMNeT &getInstance();

private:
	static ROSOMNeT instance;

	std::string configFileName;

	// This has to be unfortunately static as there is no other way how to pass data to OMNeT++ applications
	NodeHandle *rosNode;

	thread* rosThread;
	void rosMain();
};

#endif /* SIMULATION_H */
