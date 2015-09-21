#ifndef NETWORK_RESPONSE_H
#define NETWORK_RESPONSE_H

#include <stdint.h>

#include "../NetworkConstants.h"

class CGame;

class CNetworkResponse{
    public:
        CNetworkResponse(int result){
            DResult = result;
        }

        virtual void Process(CGame* game) = 0;

        int DResult;
        int16_t DAction;
};

#endif
