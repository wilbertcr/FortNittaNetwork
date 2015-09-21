#ifndef PLAYER_JOINED_RESPONSE_H
#define PLAYER_JOINED_RESPONSE_H

#include "NetworkResponse.h"

#include <string>

class CSpectatorJoinedResponse : public CNetworkResponse{
    public:
        CSpectatorJoinedResponse(std::string username,std::string roomname):
            CNetworkResponse(1), DUsername(username), DRoomName(roomname) {
        }

        virtual void Process(CGame* game);

    protected:
        std::string DUsername;
        std::string DRoomName;
};

#endif
