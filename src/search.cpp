#include <iostream>

#include "search.hpp"
#include "movegen.hpp"
#include "move.hpp"
#include "makemove.hpp"

void splitSearch(Hashtable *tt, const Position& pos, const int depth)
{
    // Search info
    searchInfo info;
    info.start = clock();
    info.end   = clock() + 1000*CLOCKS_PER_SEC;
    info.nodes = 0ULL;
    info.leafNodes = 0ULL;
    info.depth = 0;
    info.selDepth = 0;
    info.tt = tt;

    Move moves[256];
    int numMoves = movegen(pos, moves);

    // Set search stack
    searchStack ss[MAX_DEPTH];
    for(int n = 0; n < MAX_DEPTH; ++n)
    {
        ss[n].ply = n;
    }

    for(int n = 0; n < numMoves; ++n)
    {
        Position newPos = pos;

        makemove(newPos, moves[n]);

        info.nodes = 0ULL;
        info.leafNodes = 0ULL;
        info.depth = 0;
        info.selDepth = 0;

        PV pv;
        pv.numMoves = 0;

        int score = -alphaBeta(newPos, info, ss, pv, -INF, INF, depth);

        std::cout << n+1 << ":  ";
        if(n+1 < 10 && numMoves >= 10) {std::cout << " ";}
        std::cout << score << " ";

        std::cout << moveString(moves[n]);

        for(int i = 0; i < pv.numMoves; ++i)
        {
            std::cout << " " << moveString(moves[i]);
        }
        std::cout << std::endl;
    }
}

void search(Hashtable *tt, const Position& pos, const int depth)
{
    uint64_t nodeTotal = 0ULL;

    // PV
    PV lastPV;
    lastPV.numMoves = 0;
    lastPV.moves[0] = (Move){.from=0, .to=0};

    // Search stack
    searchStack ss[MAX_DEPTH];
    for(int n = 0; n < MAX_DEPTH; ++n)
    {
        ss[n].ply = n;
    }

    // Search info
    searchInfo info;
    info.start = clock();
    info.end   = clock() + 20*CLOCKS_PER_SEC;
    info.nodes = 0ULL;
    info.leafNodes = 0ULL;
    info.depth = 0;
    info.selDepth = 0;
    info.tt = tt;

    for(int d = 1; d <= depth; ++d)
    {
        PV pv;
        pv.numMoves = 0;

        //int score = random(pos, info, ss, pv);
        //int score = mostCaptures(pos, info, ss, pv);
        //int score = minimax(pos, info, ss, pv, d);
        int score = alphaBeta(pos, info, ss, pv, -INF, INF, d);

        clock_t end = clock();
        double timeSpent = (double)(end - info.start)/CLOCKS_PER_SEC;

        // Throw away the result if we ran out of time
        if(end >= info.end)
        {
            break;
        }

        nodeTotal += info.nodes;

        std::cout << "info"
                  << " depth " << d
                  << " nodes " << info.nodes
                  << " score " << score
                  << " time " << 1000.0*timeSpent;

        if(pv.numMoves == 0)
        {
            std::cout << " pv pass";
        }
        else
        {
            std::cout << " pv";
            for(int n = 0; n < pv.numMoves; ++n)
            {
                std::cout << " " << moveString(pv.moves[n]);
            }
        }

        std::cout << std::endl;

        lastPV = pv;
    }

    std::cout << "bestmove";
    std::cout << " " << moveString(lastPV.moves[0]);
    std::cout << std::endl;
}