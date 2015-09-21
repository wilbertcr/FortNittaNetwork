#ifndef POINTER_LOCATION_RESPONSE_H
#define POINTER_LOCATION_RESPONSE_H

#include "NetworkResponse.h"

class CGame;

class CPointerLocationResponse : public CNetworkResponse {
    public:
        CPointerLocationResponse(int time_step, int player_id, int x, int y): 
            CNetworkResponse(1),DTimeStep(time_step),DPlayerID(player_id),DXcoordinate(x),DYcoordinate(y){
        }
        void Process(CGame* game);
    protected:
        int DTimeStep;
        int DPlayerID;
        int DXcoordinate;
        int DYcoordinate;
        int16_t DAction;
};

#endif
