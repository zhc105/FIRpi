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
	// Winner = 1 if Black wins, Winner = 2 if White wins, otherwise Winner = 0.
	virtual void GetStatus(
			int (*Board)[15], 
			bool& HumanTurn, 
			int& TotalTurns, 
			int& NextColor,
			int& Winner
		) = 0;

};

#endif
