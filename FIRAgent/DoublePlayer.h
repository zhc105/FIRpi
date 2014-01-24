#ifndef _DOUBLEPLAYER_H_
#define _DOUBLEPLAYER_H_

#include <cstring>
#include "IFIRAgent.h"

#define VALID(_X, _Y) ( (_X) >= 0 && (_Y) >= 0 && (_X) < 15 && (_Y) < 15)

class DoublePlayer : public IFIRAgent
{
private:
	int Brd[15][15];
	int Turn, NextColor;

public:
	DoublePlayer();
	void AgentGo();
	void HumanGo(int x, int y);
	void GetStatus(int (*Board)[15], bool& HumanTurn, int& TotalTurns, int& NextColor);
	int CheckOver();
};

#endif
