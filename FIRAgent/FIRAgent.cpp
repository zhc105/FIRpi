#include "FIRAgent.h"

FIRAgent::FIRAgent(int AgentColor, int SearchDepth = 3, int GameMode = 0) : IFIRAgent("zhc105")
{
	memset(brd, 0, sizeof(brd));
	Self = AgentColor;
	Opp = (1 == Self) ? 2 : 1;
	Turn = 0;
	SelfTurn = Self - 1;
	MaxDepth = SearchDepth;
}

void FIRAgent::CheckContinuous(int Color, int (* Nums)[5])
{
	int i, j, k, t;
	memset(Nums, 0, sizeof(int) * 3 * 5);
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			if (brd[i][j] == Color)
			{
				if (0 == i || brd[i - 1][j] != brd[i][j])			// Column
				{
					t = 2;
					if (0 == i || brd[i - 1][j] != 0)
						--t;
					for (k = 0; k < 4 && i + k < 15 && brd[i + k][j] == brd[i][j]; k++);
					if (i + k >= 15 || brd[i + k][j] != 0)
						--t;
					++Nums[t][k];
				}
				if (0 == j || brd[i][j - 1] != brd[i][j])			// Row
				{
					t = 2;
					if (0 == j || brd[i][j - 1] != 0)
						--t;
					for (k = 0; k < 4 && j + k < 15 && brd[i][j + k] == brd[i][j]; k++);
					if (j + k >= 15 || brd[i][j + k] != 0)
						--t;
					++Nums[t][k];
				}
				if (0 == i || 0 == j || brd[i - 1][j - 1] != brd[i][j])		// slash
				{
					t = 2;
					if (0 == i || 0 == j || brd[i - 1][j - 1] != 0)
						--t;
					for (k = 0; k < 4 && i + k < 15 && j + k < 15 && brd[i + k][j + k] == brd[i][j]; k++);
					if (i + k >= 15 || j + k >= 15 || brd[i + k][j + k] != 0)
						--t;
					++Nums[t][k];
				}
				if (0 == i || 14 == j || brd[i - 1][j + 1] != brd[i][j])	// backlash
				{
					t = 2;
					if (0 == i || 14 == j || brd[i - 1][j + 1] != 0)
						--t;
					for (k = 0; k < 4 && i + k < 15 && j - k >= 0 && brd[i + k][j - k] == brd[i][j]; k++);
					if (i + k >= 15 || j - k < 0 || brd[i + k][j - k] != 0)
						--t;
					++Nums[t][k];
				}
			}
}

int FIRAgent::Evaluate(int depth)
{
	int score = 0, score2;
	int nums[3][5];

	CheckContinuous(Self, nums);

	if (depth & 1)
		score += ContinuousScoreOpp(nums);
	else
		score += ContinuousScoreSelf(nums);
	CheckContinuous(Opp, nums);
	if (depth & 1)
		score -= 2 * ContinuousScoreSelf(nums);
	else
		score -= 2 * ContinuousScoreOpp(nums);

	return score;
}

int FIRAgent::ContinuousScoreOpp(int Nums[3][5])
{
	int score = 0;
	score += Nums[2][1] *  2 + Nums[1][1] * 1;
	score += Nums[2][2] * 10 + Nums[1][2] * 5;
	score += Nums[1][3] * 50;
	if (Nums[2][3] > 1)
		score += 1000;
	else if (Nums[2][3] > 0)
		score += 100;
	if (Nums[1][4] > 1)
		score += 2000;
	else if (Nums[1][4] > 0)
		score += 1500;
    if (Nums[2][4] > 0)
        score += 4000;
	return score;
}

int FIRAgent::ContinuousScoreSelf(int Nums[3][5])
{
	int score = 0;
	score += Nums[2][1] *  2 + Nums[1][1] * 1;
	score += Nums[2][2] * 20 + Nums[1][2] * 5;
	score += Nums[1][3] * 50;
	if (Nums[2][3] > 0)
        score += 2000;
	if (Nums[2][4] > 0 || Nums[1][4] > 0)
		score = +999999;
	return score;
}

int FIRAgent::CheckOver()
{
	int i, j, k;
	for (i = 0; i < 11; i++)
		for (j = 0; j < 15; j++)
			if (brd[i][j])
			{
				for (k = 0; k < 5 && brd[i + k][j] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
				for (k = 0; k < 5 && j + k < 15 && brd[i][j + k] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
				for (k = 0; k < 5 && j + k < 15 && brd[i + k][j + k] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
				for (k = 0; k < 5 && j - k >= 0 && brd[i + k][j - k] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
			}
	return 0;
}

int FIRAgent::AgentSearch(int depth, int alpha, int beta, int score[15][15])
{
	int winner = CheckOver();
	//PrintChess();
	if (winner)
		return (winner == Self) ? 9999999 - depth : -9999999;
	if (depth > MaxDepth)
	{
		/*evaluate chess situation*/
		int ret = Evaluate(depth);
		return ret;
	}
	int i, j, ret = (depth & 1) ? INT_MAX : -INT_MAX;
	int subalpha = alpha, subbeta = beta;
	bool can[15][15] = { 0 };
	/*Note the position that agent could choose*/
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			if (brd[i][j] > 0)
			{
				if (i > 0 && !brd[i - 1][j]) can[i - 1][j] = true;
				if (i > 1 && !brd[i - 2][j]) can[i - 2][j] = true;
				if (i > 0 && j < 14 && !brd[i - 1][j + 1]) can[i - 1][j + 1] = true;
				if (i > 1 && j < 13 && !brd[i - 2][j + 2]) can[i - 2][j + 2] = true;
				if (j < 14 && !brd[i][j + 1]) can[i][j + 1] = true;
				if (j < 13 && !brd[i][j + 2]) can[i][j + 2] = true;
				if (i < 14 && j < 14 && !brd[i + 1][j + 1]) can[i + 1][j + 1] = true;
				if (i < 13 && j < 13 && !brd[i + 2][j + 2]) can[i + 2][j + 2] = true;
				if (i < 14 && !brd[i + 1][j]) can[i + 1][j] = true;
				if (i < 13 && !brd[i + 2][j]) can[i + 2][j] = true;
				if (i < 14 && j > 0 && !brd[i + 1][j - 1]) can[i + 1][j - 1] = true;
				if (i < 13 && j > 1 && !brd[i + 2][j - 2]) can[i + 2][j - 2] = true;
				if (j > 0 && !brd[i][j - 1]) can[i][j - 1] = true;
				if (j > 1 && !brd[i][j - 2]) can[i][j - 2] = true;
				if (i > 0 && j > 0 && !brd[i - 1][j - 1]) can[i - 1][j - 1] = true;
				if (i > 1 && j > 1 && !brd[i - 2][j - 2]) can[i - 2][j - 2] = true;
			}

	/*Search subnodes*/
	for (i = 0; i < 15 * 15; i++)
	{
		int x = i / 15, y = i % 15;
		if (can[x][y])
		{
			int sub_score[15][15] = { 0 };
			brd[x][y] = (depth & 1) ? Opp : Self;	// set chess
			int val = AgentSearch(depth + 1, subalpha, subbeta, sub_score);	// search subnode
			brd[x][y] = 0;				// clear chess

			score[x][y] = val;
			if (depth & 1)
			{
				/*min Node*/
				ret = std::min(ret, val);
				subbeta = std::min(subbeta, val);
				if (val <= alpha)
					break;
			}
			else
			{
				/*MAX Node*/
				ret = std::max(ret, val);
				subalpha = std::max(subalpha, val);
				if (val >= beta)
					break;
			}
		}
	}
	return ret;
}

void FIRAgent::AgentGo()
{
	if ((Turn & 1) != SelfTurn)
		return;
	memset(root_score, 0x80, sizeof(root_score));
	if (0 == Turn)	// First step
	{
		brd[7][7] = Self;
	}
	else
	{
		int alpha = -INT_MAX, beta = INT_MAX;
		int max_x = -1, max_y = -1, max_v = -INT_MAX;
		AgentSearch(0, alpha, beta, root_score);

		for (int i = 0; i < 15; i++)
			for (int j = 0; j < 15; j++)
				if (root_score[i][j] > max_v)
				{
					max_v = root_score[i][j];
					max_x = i;
					max_y = j;
				}
		if (max_x >= 0 && max_y >= 0)
			brd[max_x][max_y] = Self;
        printf("AgentGo: %d %d\n", max_x, max_y);
	}

	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
			if (root_score[i][j] < -99999999)
				printf("x ");
			else
				printf("%d ", root_score[i][j]);
		printf("\n");
	}
	++Turn;
}

void FIRAgent::HumanGo(int x, int y)
{
	if ((Turn & 1) == SelfTurn)
		return;
	if (x < 0 || y < 0 || x >= 15 || y >= 15 || brd[x][y])
		return;
	brd[x][y] = Opp;
	++Turn;
}

void FIRAgent::PrintChess()
{
	int i, j;
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
			if (brd[i][j] == 1)
				printf("* ");
			else if (brd[i][j] == 2)
				printf("o ");
			else
				printf(". ");
		printf("\n");
	}
}

