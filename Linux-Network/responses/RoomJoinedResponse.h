#ifndef ROOM_JOINED_RESPONSE_H
#define ROOM_JOINED_RESPONSE_H

#include <string>
#include <vector>

#include "NetworkResponse.h"
#include "../NetworkConstants.h"

class CRoomJoinedResponse : public CNetworkResponse{
    public:      
        CRoomJoinedResponse(std::vector<std::string> players, int player_count, std::string roomname):
            CNetworkResponse(1), DPlayers(players), DCapacity(player_count), DRoomName(roomname) {
        	DAction = (int16_t) SERVER_ALLOW_JOIN_ROOM_ACTION;
        }

        void Process(CGame* game);
    protected:
        std::vector<std::string> DPlayers;
        int DCapacity; 
        std::string DRoomName;
};

#endif
