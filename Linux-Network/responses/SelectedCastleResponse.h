#ifndef SELECTED_CASTLE_RESPONSE_H
#define SELECTED_CASTLE_RESPONSE_H

#include <iostream>
#include "NetworkResponse.h"
#include "PointerLocationResponse.h"

class CSelectedCastleResponse : public CPointerLocationResponse {
    public:
        CSelectedCastleResponse(int time_step, int player_id, int x, int y): 
            CPointerLocationResponse(time_step,player_id, x,  y){
        	DAction = (int16_t) SELECTED_CASTLE;
        }

    void Process(CGame* game);

};

#endif
