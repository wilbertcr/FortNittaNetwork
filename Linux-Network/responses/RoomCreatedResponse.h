#ifndef ROOM_CREATED_RESPONSE_H
#define ROOM_CREATED_RESPONSE_H

#include "NetworkResponse.h"
#include "../NetworkConstants.h"
#include <string>

class CRoomCreatedResponse : public CNetworkResponse{
    public:
        CRoomCreatedResponse(int respresult, std::string resproom_name):
            CNetworkResponse(respresult), DResponseRoomName(resproom_name) {
        	DAction = (int16_t) SERVER_CREATE_ROOM_ACTION;
        }

        virtual void Process(CGame* game);
        std::string DResponseRoomName;
};

#endif
