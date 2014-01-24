#ifndef _FIRAGENT_H_
#define _FIRAGENT_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <vector>
#include "IFIRAgent.h"

#define INT_MAX 0x7FFFFFFF
#define VALID(_X, _Y) ( (_X) >= 0 && (_Y) >= 0 && (_X) < 15 && (_Y) < 15)

class VectorStart
{
public:
	int x, y, dx, dy;
	VectorStart(int x, int y, int dx, int dy)
	{
		this->x = x;
		this->y = y;
		this->dx = dx;
		this->dy = dy;
	}
	VectorStart() {}
};

class FIRAgent : public IFIRAgent
{
private:
	int (* MaxNums)[2][2];
	int (* VecBak) [2][2], stk;
	int (* SRange)[15];
        int (* brd)[15];
        int (* root_score)[15];
        int Self, Opp;
        int Turn, SelfTurn;
        int MaxDepth;
	std::vector<VectorStart> Vecs;
	std::vector<int> AssocVec[15][15];
	
private:
        int AgentSearch(int depth, int alpha, int beta, int score[15][15]);
	void CountNums(int c, int Nums[3][6]);
	int ContinuousScoreOpp(int Color);
	int ContinuousScoreSelf(int Color);
	int Evaluate(int depth);
	void UpdateVector(int VecId);
	int ScanLine(int Color, int Line[6], int& Type);
	void ChessSet(int d, int x, int y, int c, bool bak);
	void ChessClear(int x, int y);
	void InitVector();

public:
        FIRAgent(int AgentColor, int SearchDepth, int GameMode);
	~FIRAgent();
        void AgentGo();
        void HumanGo(int x, int y);
	void GetStatus(int (*Board)[15], bool& HumanTurn, int& TotalTurns);
	int CheckOver();

        void PrintChess();

};

#endif
