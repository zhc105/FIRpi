#include "FIRAgent.h"

FIRAgent::FIRAgent(int AgentColor, int SearchDepth = 3, int GameMode = 0) : IFIRAgent("zhc105")
{
	brd = new int[15][15];
	SRange = new int[15][15];
	root_score = new int[15][15];
	MaxNums = new int[72][2][2];
	VecBak = new int[50][2][2];
	memset(brd, 0, sizeof(int) * 15 * 15);
	memset(SRange, 0x7f, sizeof(int) * 15 * 15);
	memset(MaxNums, 0, sizeof(int) * 72 * 2 * 2);
	stk = 0;
	Self = AgentColor;
	Opp = (1 == Self) ? 2 : 1;
	Turn = 0;
	SelfTurn = Self - 1;
	MaxDepth = SearchDepth;
	InitVector();
}

FIRAgent::~FIRAgent()
{
	delete [] brd;
	delete [] SRange;
	delete [] root_score;
	delete [] MaxNums;
	delete [] VecBak;
}

int FIRAgent::ScanLine(int Color, int Line[6], int& Type)
{
	bool dead = false, border = false;
	int num = 0, hole = 0, space = 0;
	for (int i = 0; i < 6; i++)
	{
		if (Line[i] == 0)
		{
			if (num > 0)
				++space;
		}
		else if (Line[i] == Color)
		{
			++num;
			hole += space;
			space = 0;
			if (0 == i|| 5 == i)
				border = true;
		}
		else
		{
			if ((0 == i || 5 == i) && !dead)
			{
				dead = true;
			}
			else
			{
				num = 0;
				break;
			}
		}
	}
	if (num >= 5)
	{
		if (hole)
		{
			Type = 1;
			return 4;
		}
		else
		{
			if (dead)
				Type = 1;
			else
				Type = 0;
			return 5;
		}
	}
	else
	{
		if (hole >= 3) --num;
		if (hole == 2 && num == 4) --num;
		if (num <= 3) hole = 0;
		if (hole || dead || border)
			Type = 1;
		else
			Type = 0;
		return num;
	}
}

void FIRAgent::UpdateVector(int VecId)
{
	VectorStart vec = Vecs[VecId];
	int x, y, i, now = 0, num, sum = 0, line[20], t;

	memset(MaxNums[VecId], 0, sizeof(int) * 2 * 2);

	x = vec.x;
	y = vec.y;
	line[0] = brd[x][y];
	sum += line[0];
	while (VALID(x, y))
	{
		x += vec.dx;
		y += vec.dy;
		if (VALID(x, y))
			line[++now] = brd[x][y];
		else
			line[++now] = 3;		// out of border
		sum += line[now];
		if (now > 5)
			sum -= line[now - 6];
		if (now >= 5 && sum > 0)
		{
			num = ScanLine(1, line + (now - 5), t); // scan black chess
			MaxNums[VecId][t][0] = std::max(MaxNums[VecId][t][0], num);
			num = ScanLine(2, line + (now - 5), t); // scan white chess
			MaxNums[VecId][t][1] = std::max(MaxNums[VecId][t][1], num);
		}
	}
}

void FIRAgent::CountNums(int c, int Nums[3][6])
{
	memset(Nums, 0, sizeof(int) * 3 * 6);
	for (int i = 0; i < 72; i++)
	{
		if (MaxNums[i][1][c] > MaxNums[i][0][c])
			++Nums[1][MaxNums[i][1][c]];
		else
			++Nums[2][MaxNums[i][0][c]];
	}
}

int FIRAgent::ContinuousScoreOpp(int Color)
{
	int score = 0, Nums[3][6];
	CountNums(Color - 1, Nums);
	score += Nums[2][1] *  2 + Nums[1][1] * 1;
	score += Nums[2][2] * 20 + Nums[1][2] * 5;
	score += Nums[1][3] * 100;
	if (Nums[2][3] > 1)
		score += 1000;
	else if (Nums[2][3] > 0)
		score += 200;
	if (Nums[2][3] > 0 && Nums[1][4] > 0)
		score += 1500;
	if (Nums[1][4] > 1)	
		score += 2000;
	else if (Nums[1][4] > 0)
		score += 250;
	if (Nums[2][4] > 0)
		score += 4000;
	if (Nums[1][5] > 0)
		score += 999999;
	if (Nums[2][5] > 0)
		score += 1100000;
	return score;
}

int FIRAgent::ContinuousScoreSelf(int Color)
{
	int score = 0, Nums[3][6];
	CountNums(Color - 1, Nums);
	score += Nums[2][1] *  2 + Nums[1][1] * 1;
	score += Nums[2][2] * 30 + Nums[1][2] * 10;
	score += Nums[1][3] * 120;
	if (Nums[2][3] > 0)
		score += 2000;
	if (Nums[2][4] > 0 || Nums[1][4] > 0)
		score += 20000;
	if (Nums[1][5] > 0)
		score += 1100000;
	if (Nums[2][5] > 0)
		score += 1200000;
	return score;
}

int FIRAgent::Evaluate(int depth)
{
	int score = 0;
	if (depth & 1)
	{
		score += ContinuousScoreOpp(Self);
		score -= ContinuousScoreSelf(Opp);
	}
	else
	{
		score += ContinuousScoreSelf(Self);
		score -= ContinuousScoreOpp(Opp);
	}
	return score;
}

int FIRAgent::CheckOver()
{
	int i, j, k;
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			if (brd[i][j])
			{
				for (k = 0; k < 5 && i + k < 15 && brd[i + k][j] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
				for (k = 0; k < 5 && j + k < 15 && brd[i][j + k] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
				for (k = 0; k < 5 && i + k < 15 && j + k < 15 && brd[i + k][j + k] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
				for (k = 0; k < 5 && i + k < 15 && j - k >= 0 && brd[i + k][j - k] == brd[i][j]; k++)
					if (k == 4)
						return brd[i][j];
			}
	return 0;
}

void FIRAgent::ChessSet(int d, int x, int y, int c, bool bak = false)
{
	if (!VALID(x, y))
		return;
	if (VALID(x - 1, y) && d < SRange[x - 1][y]) SRange[x - 1][y] = d;
	if (VALID(x - 2, y) && d < SRange[x - 2][y]) SRange[x - 2][y] = d;
	if (VALID(x - 1, y + 1) && d < SRange[x - 1][y + 1]) SRange[x - 1][y + 1] = d;
	if (VALID(x - 2, y + 2) && d < SRange[x - 2][y + 2]) SRange[x - 2][y + 2] = d;
	if (VALID(x, y + 1) && d < SRange[x][y + 1]) SRange[x][y + 1] = d;
	if (VALID(x, y + 2) && d < SRange[x][y + 2]) SRange[x][y + 2] = d;
	if (VALID(x + 1, y + 1) && d < SRange[x + 1][y + 1]) SRange[x + 1][y + 1] = d;
	if (VALID(x + 2, y + 2) && d < SRange[x + 2][y + 2]) SRange[x + 2][y + 2] = d;
	if (VALID(x + 1, y) && d < SRange[x + 1][y]) SRange[x + 1][y] = d;
	if (VALID(x + 2, y) && d < SRange[x + 2][y]) SRange[x + 2][y] = d;
	if (VALID(x + 1, y - 1) && d < SRange[x + 1][y - 1]) SRange[x + 1][y - 1] = d;
	if (VALID(x + 2, y - 2) && d < SRange[x + 2][y - 2]) SRange[x + 2][y - 2] = d;
	if (VALID(x, y - 1) && d < SRange[x][y - 1]) SRange[x][y - 1] = d;
	if (VALID(x, y - 2) && d < SRange[x][y - 2]) SRange[x][y - 2] = d;
	if (VALID(x - 1, y - 1) && d < SRange[x - 1][y - 1]) SRange[x - 1][y - 1] = d;
	if (VALID(x - 2, y - 2) && d < SRange[x - 2][y - 2]) SRange[x - 2][y - 2] = d;
	brd[x][y] = c;
	// Update vector
	for (std::vector<int>::iterator p = AssocVec[x][y].begin(); p != AssocVec[x][y].end(); p++)
	{
		if (bak)
			memcpy(VecBak[stk++], MaxNums[*p], sizeof(int) * 2 * 2);
		UpdateVector(*p);
	}
}

void FIRAgent::ChessClear(int x, int y)
{
	brd[x][y] = 0;
	// recovery vector
	for (std::vector<int>::reverse_iterator p = AssocVec[x][y].rbegin(); p != AssocVec[x][y].rend(); p++)
	{
		memcpy(MaxNums[*p], VecBak[--stk], sizeof(int) * 2 * 2);
	}
}

int FIRAgent::AgentSearch(int depth, int alpha, int beta, int score[15][15])
{
	int val = Evaluate(depth);
	if (val > 900000 || val < -900000)
		return val;
	//printf("score: %d\n", val);
	if (depth >= MaxDepth)
		return val;
	int i, j, ret = (depth & 1) ? INFINITE : -INFINITE;
	int subalpha = alpha, subbeta = beta;
	int SRange_bak[15][15];
	
	/*Search subnodes*/
	memcpy(SRange_bak, SRange, sizeof(int) * 15 * 15);
	for (i = 0; i < 15 * 15; i++)
	{
		int x = i / 15, y = i % 15;
		if (Turn + depth > SRange[x][y] && !brd[x][y])
		{
			int sub_score[15][15] = { 0 };
			ChessSet(Turn + depth, x, y, (depth & 1) ? Opp : Self, true);	// set chess
			int val = AgentSearch(depth + 1, subalpha, subbeta, sub_score);	// search subnode
			ChessClear(x, y);						// clear chess

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
	memcpy(SRange, SRange_bak, sizeof(int) * 15 * 15);
	return ret;
}

void FIRAgent::AgentGo()
{
	if ((Turn & 1) != SelfTurn)
		return;
	memset(root_score, 0x80, sizeof(int) * 15 * 15);
	if (0 == Turn)	// First step
	{
		ChessSet(0, 7, 7, Self, false);
	}
	else
	{
		int alpha = -INFINITE, beta = INFINITE;
		int max_x = -1, max_y = -1, max_v = -INFINITE;
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
			ChessSet(Turn, max_x, max_y, Self, false);
        	//printf("AgentGo: %d %d\n", max_x, max_y);
	}

	/*for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
			if (root_score[i][j] < -99999999)
				printf("x ");
			else
				printf("%d ", root_score[i][j]);
		printf("\n");
	}*/
	++Turn;
}

void FIRAgent::HumanGo(int x, int y)
{
	if ((Turn & 1) == SelfTurn)
		return;
	if (!VALID(x, y) || brd[x][y])
		return;
	ChessSet(Turn, x, y, Opp, false);
	++Turn;
}

void FIRAgent::GetStatus(
		int (*Board)[15], 
		bool& HumanTurn, 
		int& TotalTurns, 
		int& NextColor,
		int& Winner)
{
	memcpy(Board, brd, sizeof(int) * 15 * 15);
	if ((Turn & 1) != SelfTurn)
		HumanTurn = true;
	else
		HumanTurn = false;
	TotalTurns = Turn;
	if ((Turn & 1) == SelfTurn)
		NextColor = Self;
	else
		NextColor = Opp;
	Winner = CheckOver();
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

void FIRAgent::InitVector()
{
	int VecNo = 0, i, j, k;
	Vecs.clear();

	// row vector
	for (i = 0; i < 15; i++)
	{
		Vecs.push_back(VectorStart(i, 0, 0, 1));
		for (j = 0; j < 15; j++)
			AssocVec[i][j].push_back(VecNo);
		++VecNo;
	}
	// column vector
	for (j = 0; j < 15; j++)
	{
		Vecs.push_back(VectorStart(0, j, 1, 0));
		for (i = 0; i < 15; i++)
			AssocVec[i][j].push_back(VecNo);
		++VecNo;
	}
	// slash vector
	for (i = 0; i < 11; i++)
	{
		Vecs.push_back(VectorStart(i, 0, 1, 1));
		for (j = i, k = 0; VALID(j, k); j++, k++)
			AssocVec[j][k].push_back(VecNo);
		++VecNo;
	}
	for (j = 1; j < 11; j++)
	{
		Vecs.push_back(VectorStart(0, j, 1, 1));
		for (i = 0, k = j; VALID(i, k); i++, k++)
			AssocVec[i][k].push_back(VecNo);
		++VecNo;
	}
	// backlash vector
	for (i = 0; i < 11; i++)
	{
		Vecs.push_back(VectorStart(i, 14, 1, -1));
		for (j = i, k = 14; VALID(j, k); j++, k--)
			AssocVec[j][k].push_back(VecNo);
		++VecNo;
	}
	for (j = 4; j < 14; j++)
	{
		Vecs.push_back(VectorStart(0, j, 1, -1));
		for (i = 0, k = j; VALID(i, k); i++, k--)
			AssocVec[i][k].push_back(VecNo);
		++VecNo;
	}
}

