#ifndef NETWORK_MESSAGE_H
#define NETWORK_MESSAGE_H

#include <string>
#include "../NetworkConstants.h"

class CNetworkMessage{
    public:
        CNetworkMessage();
        virtual void MakeMessage() = 0;
        virtual char* GetMessage() = 0;
        virtual const int GetMessageLength() = 0;
    protected:

        uint16_t DAction = -1;
};

#endif
