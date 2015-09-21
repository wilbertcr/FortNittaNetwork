#ifndef CGET_AVAILABLE_ROOMS_MESSAGE_H
#define CGET_AVAILABLE_ROOMS_MESSAGE_H

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"

#include <string>

class CGetAvailableRoomsMessage : public CNetworkMessage{
    public:
        CGetAvailableRoomsMessage(std::string username);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        virtual ~CGetAvailableRoomsMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
};
#endif
