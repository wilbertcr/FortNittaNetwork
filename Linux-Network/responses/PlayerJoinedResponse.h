#ifndef PLAYER_JOINED_RESPONSE_H
#define PLAYER_JOINED_RESPONSE_H

#include "NetworkResponse.h"
#include "../NetworkConstants.h"

#include <string>

class CPlayerJoinedResponse : public CNetworkResponse{
    public:
        CPlayerJoinedResponse(std::string username,std::string roomname):
            CNetworkResponse(1), DUsername(username), DRoomName(roomname) {
        	DAction = (int16_t) SERVER_PLAYER_JOINED_ROOM_ACTION;
        }

        virtual void Process(CGame* game);

    protected:
        std::string DUsername;
        std::string DRoomName;
};

#endif
