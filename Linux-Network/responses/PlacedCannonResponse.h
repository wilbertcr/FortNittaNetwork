#ifndef PLACED_CANNON_RESPONSE_H
#define PLACED_CANNON_RESPONSE_H

#include "NetworkResponse.h"
#include "PointerLocationResponse.h"

class CGame;

class CPlacedCannonResponse : public CPointerLocationResponse{
    public:
        CPlacedCannonResponse(int time_step, int player_id, int x, int y):
            CPointerLocationResponse(time_step, player_id, x, y){
        	DAction = (int16_t) PLACED_CANNON;
        }

    void Process(CGame* game);

};

#endif
