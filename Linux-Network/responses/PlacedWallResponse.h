#ifndef PLACED_WALL_RESPONSE_H
#define PLACED_WALL_RESPONSE_H

#include "NetworkResponse.h"
#include "PointerLocationResponse.h"

class CGame;

class CPlacedWallResponse : public CPointerLocationResponse{
    public:
        CPlacedWallResponse(int time_step, int player_id, int x, int y):
            CPointerLocationResponse(time_step, player_id, x,  y){
        	DAction = (int16_t) PLACED_WALL;
        }

    void Process(CGame* game);
};

#endif
