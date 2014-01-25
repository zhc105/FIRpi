#include "DoublePlayer.h"

DoublePlayer::DoublePlayer() : IFIRAgent("DoublePlayer")
{
	memset(Brd, 0, sizeof(Brd));
	Turn = 0;
	NextColor = 1;
}

void DoublePlayer::AgentGo()
{
	// nothing to do
}

void DoublePlayer::HumanGo(int x, int y)
{
	if (!VALID(x, y) || Brd[x][y])
		return;

	Brd[x][y] = NextColor;
	NextColor = (NextColor % 2) + 1;
	++Turn;
}

void DoublePlayer::GetStatus(int (*Board)[15], bool& HumanTurn, int& TotalTurns, int& NextColor, int& Winner)
{
	memcpy(Board, Brd, sizeof(Brd));
	HumanTurn = true;
	TotalTurns = Turn;
	NextColor = this->NextColor;
	Winner = CheckOver();
}

int DoublePlayer::CheckOver()
{
	int i, j, k;
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			if (Brd[i][j])
			{
				for (k = 0; k < 5 && i + k < 15 && Brd[i + k][j] == Brd[i][j]; k++)
					if (k == 4)
						return Brd[i][j];
				for (k = 0; k < 5 && j + k < 15 && Brd[i][j + k] == Brd[i][j]; k++)
					if (k == 4)
						return Brd[i][j];
				for (k = 0; k < 5 && i + k < 15 && j + k < 15 && Brd[i + k][j + k] == Brd[i][j]; k++)
					if (k == 4)
						return Brd[i][j];
				for (k = 0; k < 5 && i + k < 15 && j - k >= 0 && Brd[i + k][j - k] == Brd[i][j]; k++)
					if (k == 4)
						return Brd[i][j];
			}
	return 0;
}

