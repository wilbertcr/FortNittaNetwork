#ifndef START_GAME_RESPONSE_H
#define START_GAME_RESPONSE_H

#include "NetworkResponse.h"
#include <vector>
#include <string>

class CGame;

class CStartGameResponse: public CNetworkResponse{
    public:
        CStartGameResponse(std::vector<std::string> players, std::string map): CNetworkResponse(1) {
            DPlayers = players;
            DMap = map;
            DAction = (int16_t) SERVER_START_GAME_ACTION;
        }

        void Process(CGame* game);
    protected:
        std::string DMap;
        std::vector<std::string> DPlayers;
};

#endif
