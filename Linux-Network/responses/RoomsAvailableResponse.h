#ifndef ROOMS_AVAILABLE_RESPONSE_H
#define ROOMS_AVAILABLE_RESPONSE_H

#include "NetworkResponse.h"
#include "../../game/Room.h"
#include "../NetworkConstants.h"
#include <vector>
#include <ostream>
class CRoomsAvailableResponse: public CNetworkResponse{
    public:
        CRoomsAvailableResponse(std::vector<CRoom*> rooms):
            CNetworkResponse(1), DRooms(rooms){
        	DAction = (int16_t) SERVER_SEND_AVAILABLE_ROOMS_ACTION;
        }

        void Process(CGame* game);
    protected:
       std::vector<CRoom*> DRooms;
};

#endif
