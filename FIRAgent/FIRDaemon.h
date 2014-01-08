#ifndef _FIRDAEMON_H_
#define _FIRDAEMON_H_

#include "IFIRAgent.h"
#include "FIRAgent.h"

class FIRDaemon
{
private:
	int Turn;
	bool Busy;
	int Brd[15][15];
	IFIRAgent *Agent;

public:
	FIRDaemon();
	// Start FIR server
	int Start();
	// AgentColor: 1 - Agent first, 2 - Agent last
	void CreateAgent(std::string AgentName, int AgentColor);	
	// return JSON String to represent the board, busy status, turn number and ...
	std::string GetStatusJson();

};

#endif
