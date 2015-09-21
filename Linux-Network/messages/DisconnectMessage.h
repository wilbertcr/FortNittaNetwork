#ifndef CDISCONNECT_MESSAGE_H
#define CDISCONNECT_MESSAGE_H

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"
#include <string>

class CDisconnectMessage : public CNetworkMessage{
    public:
        CDisconnectMessage();
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CDisconnectMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
        std::string DRoomName;
};
#endif
