#ifndef ROOM_DENIED_RESPONSE_H
#define ROOM_DENIED_RESPONSE_H

#include <string>
#include <vector>

#include "NetworkResponse.h"
#include "../NetworkConstants.h"

class CGame;

#define ROOM_DENIED_TEXT "Could not join game"

class CRoomDeniedResponse : public CNetworkResponse{
    public:
        CRoomDeniedResponse(std::string respRoomName)
            :CNetworkResponse(1), DRoomName(respRoomName){
        	DAction = (int16_t) SERVER_DENY_JOIN_ROOM_ACTION;
        }

        void Process(CGame* game);
        std::string DRoomName;        
};

#endif
