#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include "FIRDaemon.h"
#include "MyLog.h"

FIRDaemon::FIRDaemon()
{
	Busy = false;
	Agent = NULL;
}

void FIRDaemon::AgentController(int csock)
{
	char buf[50], cmd[50];

	for (int now = 0; now < 30; now++)
	{
		/* receive command */
		if (recv(csock, buf + now, 1, 0) <= 0)
			break;
		if (buf[now] == '\r' || buf[now] == '\n')
			break;
	}
	sscanf(buf, "%s", cmd);

	if (!strcmp(cmd, "start"))
	{
		char name[50];
		int color;
		MyLog::WriteLog("Client request to start game.", 0);

		sscanf(buf, "%s %s %d", cmd, name, &color);
		CreateAgent(std::string(name), color);
	}
	else if (!strcmp(cmd, "status"))
	{
		MyLog::WriteLog("Client request to get status,", 0);
	}
	else if (!strcmp(cmd, "set"))
	{
		int x, y;

		sscanf(buf, "%s %d %d", cmd, &x, &y);
		HumanGo(x, y);
	}
	else if (!strcmp(cmd, "print"))
	{
		int i, j;
		lock.Lock();
		for (i = 0; i < 15; i++)
		{
			for (j = 0; j < 15; j++)
				if (1 == Brd[i][j])
					send(csock, "* ", 2, 0);
				else if (2 == Brd[i][j])
					send(csock, "o ", 2, 0);
				else
					send(csock, ". ", 2, 0);
			send(csock, "\r\n", 2, 0);
		}
		lock.Unlock();
	}
}

void * FIRDaemon::AgentAction(void *arg)
{
	FIRDaemon *daemon = (FIRDaemon *) arg;

	daemon->Busy = false;
	return (void *) 0;
}

void FIRDaemon::HumanGo(int x, int y)
{
	char msg[50];
	snprintf(msg, sizeof(msg), "Client request to set a chess on %d, %d", x, y);
	MyLog::WriteLog(msg, 0);
	
	if (Busy) 
		return;

	lock.Lock();
	if (HumanTurn && !Winner && Agent != NULL)
	{
		Agent->HumanGo(x, y);
		Winner = Agent->CheckOver();
		Agent->GetStatus(Brd, HumanTurn, Turn);
		while (!Winner && !HumanTurn)
		{
			// Agent Turn (create new thread)
			Busy = true;
			int ret = pthread_create(&AgentThread, NULL, AgentAction, this);
			if (ret != 0)
			{
				std::string errmsg = "cannot create thread: ";
				errmsg += strerror(ret);
				MyLog::WriteLog(errmsg.c_str(), 2);
				exit(-5);
			}
		}
	}
	lock.Unlock();
}

int FIRDaemon::Start()
{
	int ssock, csock;
	socklen_t sin_size;
	struct sockaddr_in saddr, caddr;

	//daemon(1, 0);
	MyLog::WriteLog("Initializing server...", 0);

	ssock = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == ssock)
	{
		MyLog::WriteLog("cannot create socket!", 2);
		return -1;
	}

	bzero(&saddr, sizeof(struct sockaddr_in));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_ANY);
	saddr.sin_port = htons(8899);

	if ( -1 == bind(ssock, (struct sockaddr *) &saddr, sizeof(struct sockaddr)))
	{
		MyLog::WriteLog("bind failed!", 2);
		return -2;
	}

	if ( -1 == listen(ssock, 10))
	{
		MyLog::WriteLog("listen failed!", 2);
		return -3;
	}
	
	MyLog::WriteLog("Server has successfully started!", 0);
	while (true)
	{
		sin_size = sizeof(struct sockaddr_in);
		csock = accept(ssock, (struct sockaddr *) &caddr, &sin_size);
		if ( -1 == csock )
		{
			MyLog::WriteLog("accept failed!", 2);
			return -4;
		}

		AgentController(csock);
		shutdown(csock, SHUT_WR);
		close(csock);
	}

	close(ssock);
	return 0;
}

void FIRDaemon::CreateAgent(std::string AgentName, int AgentColor)
{
	if (Busy)
		return;
	if (AgentName == "zhc105")
	{
		if (Agent != NULL)
			delete Agent;
		Agent = new FIRAgent(AgentColor, 3, 0);
	}
	/* Get initial status */
	lock.Lock();
	Winner = 0;
	Agent->GetStatus(Brd, HumanTurn, Turn);

	while (!Winner && !HumanTurn)
	{
		Agent->AgentGo();
		Agent->GetStatus(Brd, HumanTurn, Turn);
		Winner = Agent->CheckOver();
	}
	lock.Unlock();

	std::string msg = "Game restart: ";
	msg += AgentName + " ";
	if (AgentColor == 1)
		msg += "first";
	else
		msg += "last";
	MyLog::WriteLog(msg.c_str(), 0);
}


