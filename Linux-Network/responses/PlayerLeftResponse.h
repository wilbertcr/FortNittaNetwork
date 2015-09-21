#ifndef PLAYER_LEFT_RESPONSE_H
#define PLAYER_LEFT_RESPONSE_H

#include "NetworkResponse.h"

#include <string>

class CPlayerLeftResponse : public CNetworkResponse{
    public:
        CPlayerLeftResponse(std::string username, std::string roomname):
            CNetworkResponse(1), DUsername(username), DRoomname(roomname) {
        }

        virtual void Process(CGame* game);

    protected:
        std::string DUsername;
        std::string DRoomname;

};

#endif
