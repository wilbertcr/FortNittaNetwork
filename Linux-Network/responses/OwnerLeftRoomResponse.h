#ifndef OWNER_LEFT_ROOM_RESPONSE_H
#define OWNER_LEFT_ROOM_RESPONSE_H

#include "NetworkResponse.h"

#include <string>

class CGame;

class COwnerLeftRoomResponse: public CNetworkResponse{
    public:
        COwnerLeftRoomResponse(std::string username, std::string roomname):
            CNetworkResponse(1), DUsername(username), DRoomName(roomname){
        	DAction = (int16_t) SERVER_OWNER_LEFT_ROOM_ACTION;
        }
   
        void Process(CGame *game);
    protected:
       std::string DUsername;
       std::string DRoomName;
       
       
};
#endif
