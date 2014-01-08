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
	virtual void AgentGo() = 0;
	virtual void HumanGo(int x, int y) = 0;
};

#endif
