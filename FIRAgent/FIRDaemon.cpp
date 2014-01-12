#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <cstring>
#include <cstdlib>
#include <sstream>
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

		if (sscanf(buf, "%s %s %d", cmd, name, &color) == 3)
			CreateAgent(std::string(name), color);
	}
	else if (!strcmp(cmd, "status"))
	{
		MyLog::WriteLog("Client request to get status,", 0);

		std::string json = GetStatusJson();
		send(csock, json.c_str(), json.length(), 0);
	}
	else if (!strcmp(cmd, "set"))
	{
		int x, y;

		if (sscanf(buf, "%s %d %d", cmd, &x, &y) == 3)
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
	pthread_detach(pthread_self());	// Automatic release of resource 
	FIRDaemon *daemon = (FIRDaemon *) arg;

	while (!daemon->Winner && !daemon->HumanTurn)
	{
		daemon->Agent->AgentGo();
		// Update status
		daemon->lock.Lock();
		daemon->Agent->GetStatus(daemon->Brd, daemon->HumanTurn, daemon->Turn);
		daemon->Winner = daemon->Agent->CheckOver();
		daemon->lock.Unlock();
	}

	daemon->Busy = false;
	return (void *) 0;
}

void FIRDaemon::HumanGo(int x, int y)
{
	char msg[50];
	snprintf(msg, sizeof(msg), "Client request to set a chess on %d, %d", x, y);
	MyLog::WriteLog(msg, 0);
	
	lock.Lock();
	if (Busy)
	{
		lock.Unlock();
		return;
	}
	if (HumanTurn && !Winner && Agent != NULL)
	{
		Agent->HumanGo(x, y);
		// Update status
		Winner = Agent->CheckOver();
		Agent->GetStatus(Brd, HumanTurn, Turn);
		if (!Winner && !HumanTurn)
		{
			// Agent turn (create new thread)
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

std::string FIRDaemon::GetStatusJson()
{
	std::stringstream json;
	bool not_first = false;

	// Package current status with json format
	json << "{";

	lock.Lock();
	json << "\"Turn\":" << Turn;
	json << ",\"HumanTurn\":" << (HumanTurn ? 1 : 0);
	json << ",\"Winner\":" << Winner;
	json << ",\"Busy\":" << (Busy ? 1 : 0);
	json << ",\"Board\":[";
	for(int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			if (Brd[i][j])
			{
				if (not_first)
					json << ",";
				else
					not_first = true;
				json << "{\"x\":" << i << ",\"y\":" << j << ",\"c\":" << Brd[i][j] << "}";
			}
	json << "]";
	lock.Unlock();

	json << "}";
	return json.str();
}

int FIRDaemon::Start()
{
	int ssock, csock;
	socklen_t sin_size;
	struct sockaddr_in saddr, caddr;

	daemon(1, 0);
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
	else
	{
		MyLog::WriteLog("Agent not found!", 1);
		return;
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


