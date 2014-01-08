#include "FIRDaemon.h"

FIRAgent agent(1, 3, 0);

int main()
{
        printf("%s\n", agent.GetAgentName().c_str());
        while (!agent.CheckOver())
        {
                int x, y;

                agent.AgentGo();
                agent.PrintChess();
                printf("================================\n");
                scanf("%d%d", &x, &y);
                agent.HumanGo(x, y);
        }
        //agent.brd[7][7] = 1;
        //agent.brd[5][5] = 2;
        //agent.brd[6][6] = 2;
        //agent.PrintChess();
        //printf("%d\n", agent.Evaluate(0));
        return 0;
}

