#include <unistd.h>
#include <pthread.h>
#include "FIRDaemon.h"

FIRDaemon::FIRDaemon()
{
	Busy = false;
	Agent = NULL;
}

int FIRDaemon::Start()
{
	daemon(1, 0);
	sleep(20);
	return 0;
}

void FIRAgent::CreateAgent(std::string AgentName, int AgentColor)
{
	if (Busy)
		return;
	if (AgentName == "zhc105")
	{
		if (Agent != NULL)
			delete Agent;
		Agent = new FIRAgent(AgentColor, 3, 0);
		Turn = 1;
	}
	/* Get initial status */
	lock.Lock();
	Agent->GetStatus(Brd, HumanTurn);
	lock.UnLock();
}


