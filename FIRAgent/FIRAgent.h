#ifndef _FIRAGENT_H_
#define _FIRAGENT_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include "IFIRAgent.h"

#define INT_MAX 0x7FFFFFFF

class FIRAgent : public IFIRAgent
{
private:
        int brd[15][15];
        int root_score[15][15];
        int Self, Opp;
        int Turn, SelfTurn;
        int MaxDepth;

private:
        int AgentSearch(int depth, int alpha, int beta, int score[15][15]);
        int Evaluate(int depth);
        void CheckContinuous(int Color, int Nums[3][5]);
        int ContinuousScoreOpp(int Nums[3][5]);         // Evaluate score (Opp turn)
        int ContinuousScoreSelf(int Nums[3][5]);        // Evaluate score (Self turn)

public:
        FIRAgent(int AgentColor, int SearchDepth, int GameMode);
        void AgentGo();
        void HumanGo(int x, int y);
        int CheckOver();

        void PrintChess();

};

#endif
