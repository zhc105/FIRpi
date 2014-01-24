#ifndef _IFIRAGENT_H_
#define _IFIRAGENT_H_

#include <string>

class IFIRAgent
{
private:
	std::string AgentName;

public:
	IFIRAgent();
	IFIRAgent(std::string AgentName);
	std::string GetAgentName();

	virtual ~IFIRAgent();
	// Agent set a chess by itself
	virtual void AgentGo() = 0;
	// Human set a chess to (x, y)
	virtual void HumanGo(int x, int y) = 0;
	// Return the chess status and whether it is human's turn
	virtual void GetStatus(int (*Board)[15], bool &HumanTurn, int &TotalTurns, int& NextColor) = 0;
	// Return 1 if Black wins, return 2 if White wins, otherwise return 0.
	virtual int CheckOver() = 0;

};

#endif
