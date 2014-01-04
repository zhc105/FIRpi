#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#define INT_MAX 0x7FFFFFFF

class FIRAgent
{
private:
	int brd[15][15];
	int root_score[15][15];
	int Self, Opp;
	int Turn, SelfTurn;
	int MaxDepth;

private:
	int AgentSearch(int depth, int& alpha, int& beta, int score[15][15]);

public:
	FIRAgent(int AgentColor, int SearchDepth, int GameMode);
	void AgentGo();

};

FIRAgent::FIRAgent(int AgentColor, int SearchDepth = 5, int GameMode = 0)
{
	memset(brd, 0, sizeof(brd));
	Self = AgentColor;
	Opp = (1 == Self) ? 2 : 1;
	Turn = 0;
	SelfTurn = Self - 1;
	MaxDepth = SearchDepth;
}

void FIRAgent::AgentGo()
{
	if ((Turn & 1) != SelfTurn)
		return;
	memset(root_score, 0, sizeof(root_score));
	if (0 == Turn)	// First step
	{
		brd[7][7] = Self;
		++Turn;
	}
	else
	{
		int alpha, beta;
		AgentSearch(0, alpha, beta, root_score);
	}

	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			if (14 == j)
				printf("%d\n", root_score[i][j]);
			else
				printf("%d ", root_score[i][j]);
}

int FIRAgent::AgentSearch(int depth, int& alpha, int& beta, int score[15][15])
{
	if (depth > MaxDepth)
	{
		/*evaluate chess situation*/
		int val = 0;
		return val;
	}
	int i, j, ret = (depth & 1) ? INT_MAX : -INT_MAX;
	bool can[15][15] = { 0 };
	/*Note the position that agent could choose*/
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			if (brd[i][j] > 0)
			{
				if (!brd[i - 1][j]) can[i - 1][j] = true;
				if (!brd[i - 2][j]) can[i - 2][j] = true;
				if (!brd[i - 1][j + 1]) can[i - 1][j + 1] = true;
				if (!brd[i - 2][j + 2]) can[i - 2][j + 2] = true;
				if (!brd[i][j + 1]) can[i][j + 1] = true;
				if (!brd[i][j + 2]) can[i][j + 2] = true;
				if (!brd[i + 1][j + 1]) can[i + 1][j + 1] = true;
				if (!brd[i + 2][j + 2]) can[i + 2][j + 2] = true;
				if (!brd[i + 1][j]) can[i + 1][j] = true;
				if (!brd[i + 2][j]) can[i + 2][j] = true;
				if (!brd[i + 1][j - 1]) can[i + 1][j - 1] = true;
				if (!brd[i + 2][j - 2]) can[i + 2][j - 2] = true;
				if (!brd[i][j - 1]) can[i][j - 1] = true;
				if (!brd[i][j - 2]) can[i][j - 2] = true;
				if (!brd[i - 1][j - 1]) can[i - 1][j - 1] = true;
				if (!brd[i - 2][j - 2]) can[i - 2][j - 2] = true;
			}
	/*Search subnodes*/
	for (i = 0; i < 15 * 15; i++)
	{
		int x = i / 15, y = i % 15;
		if (can[x][y])
		{
			int sub_score[15][15] = { 0 };
			brd[x][y] = (depth & 1) ? Opp : Self;	// set chess
			int val = AgentSearch(depth + 1, alpha, beta, sub_score);	// search subnode
			brd[x][y] = 0;				// clear chess

			score[x][y] = val;
			if (depth & 1)
			{
				/*min Node*/
				ret = std::min(ret, val);
				if (val <= alpha)
					break;
				else
					alpha = val;
			}
			else
			{
				/*MAX Node*/
				ret = std::max(ret, val);
				if (val >= beta)
					break;
				else
					beta = val;
			}		
		}
	}
	return ret;
}

int main()
{
	FIRAgent agent(1);
	agent.AgentGo();
	return 0;
}
