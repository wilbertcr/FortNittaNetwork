#ifndef ROTATED_WALL_RESPONSE_H
#define ROTATED_WALL_RESPONSE_H

#include "NetworkResponse.h"
#include "PointerLocationResponse.h"

class CGame;

class CRotatedWallResponse : public CPointerLocationResponse{
    public:
        CRotatedWallResponse(int time_step, int player_id, int x, int y):
            CPointerLocationResponse(time_step,player_id,x, y){
        	DAction = (int16_t) ROTATED_WALL;
        }

    void Process(CGame* game);
};

#endif
