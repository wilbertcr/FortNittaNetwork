#ifndef CLEAVE_ROOM_MESSAGE_H
#define CLEAVE_ROOM_MESSAGE_H

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"
#include <string>

class CLeaveRoomMessage : public CNetworkMessage{
    public:
        CLeaveRoomMessage(std::string username,std::string room_name);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CLeaveRoomMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
        std::string DRoomName;
};
#endif
