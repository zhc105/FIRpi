#ifndef _FIRDAEMON_H_
#define _FIRDAEMON_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "IFIRAgent.h"
#include "FIRAgent.h"
#include "MyLock.h"

class FIRDaemon
{
private:
	int Turn;
	int Brd[15][15];
	int Winner;
	bool Busy, HumanTurn;
	IFIRAgent *Agent;
	MyLock lock;
	pthread_t AgentThread;

	void AgentController(int csock);
	// AgentColor: 1 - Agent first, 2 - Agent last
	void CreateAgent(std::string AgentName, int AgentColor);	
	// return JSON String to represent the board, busy status, turn number and ...
	std::string GetStatusJson();
	// a thread routine to wait Agent 
	static void * AgentAction(void *arg);
	void HumanGo(int x, int y);

public:
	FIRDaemon();
	// Start FIR server
	int Start();

};

#endif
