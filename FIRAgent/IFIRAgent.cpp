#include "IFIRAgent.h"

IFIRAgent::IFIRAgent()
{
	AgentName = "none";
}

IFIRAgent::IFIRAgent(std::string AgentName)
{
	this->AgentName = AgentName;
}

IFIRAgent::~IFIRAgent()
{
}

std::string IFIRAgent::GetAgentName()
{
	return AgentName;
}
