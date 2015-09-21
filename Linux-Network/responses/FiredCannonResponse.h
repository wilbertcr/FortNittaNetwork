#ifndef FIRED_CANNON_RESPONSE_H
#define FIRED_CANNON_RESPONSE_H

#include "NetworkResponse.h"
#include "PointerLocationResponse.h"

class CGame;

class CFiredCannonResponse : public CPointerLocationResponse{
    public:
        CFiredCannonResponse(int time_step, int player_id, int x, int y):
            CPointerLocationResponse(time_step, player_id, x, y){
        	DAction = (int16_t) FIRED_CANNON;
        }

    void Process(CGame* game);

};

#endif
