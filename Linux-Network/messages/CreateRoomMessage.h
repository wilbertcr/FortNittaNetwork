#ifndef CCREATE_ROOM_MESSAGE_H
#define CCREATE_ROOM_MESSAGE_H

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"

#include <string>

class CCreateRoomMessage : public CNetworkMessage{
    public:
        CCreateRoomMessage(int capacity,std::string username,std::string room_name);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CCreateRoomMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
        std::string DRoomName;
        int DCapacity;
};
#endif
