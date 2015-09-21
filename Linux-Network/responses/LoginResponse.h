#ifndef LOGIN_RESPONSE_H
#define LOGIN_RESPONSE_H

#include "NetworkResponse.h"
#include "../NetworkConstants.h"

class CLoginResponse: public CNetworkResponse{
    public:
        CLoginResponse(int result):
            CNetworkResponse(result){
        	DAction = (int16_t) SERVER_AUTH_ACTION;
        }

        virtual void Process(CGame* game);
};

#endif
