#ifndef _FIRDAEMON_H_
#define _FIRDAEMON_H_

#include <vector>
#include <map>
#include <fstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include "IFIRAgent.h"
#include "FIRAgent.h"
#include "DoublePlayer.h"
#include "MyLock.h"

class ChessInfo
{
public:
	int x, y, c;
	ChessInfo(int x, int y, int c);
	ChessInfo();
};

class AgentStat
{
public:
	int TotalF, TotalL, WinF, WinL;
	AgentStat(int tf, int tl, int wf, int wl);
	AgentStat();
};

class FIRDaemon
{
private:
	int Turn;
	int Brd[15][15];
	int Winner, NextColor, AgentColor;
	bool Busy, HumanTurn, Started;
	IFIRAgent *Agent;
	MyLock lock;
	sem_t event;
	pthread_t AgentThread;
	int csock;
	std::vector<ChessInfo> Chesses;

	std::map<std::string, AgentStat> AgentSts;

private:
	void LoadAgentStat();
	void SaveAgentStat();

	void BuildListByBrd();
	void LocalUpdate();
	void AgentController(int csock);
	// AgentColor: 1 - Agent first, 2 - Agent last
	void CreateAgent(std::string AgentName, int AgentColor);	
	// return JSON String to represent the board, busy status, turn number and ...
	std::string GetStatusJson();
	void HumanGo(int x, int y);
	// a thread routine to wait Agent 
	static void * AgentAction(void *arg);
	static void * ClientThread(void *arg);

public:
	FIRDaemon();
	// Start FIR server
	int Start();

};

#endif
