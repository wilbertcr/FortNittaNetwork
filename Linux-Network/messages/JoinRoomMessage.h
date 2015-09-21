#ifndef CJOIN_ROOM_MESSAGE_H
#define CJOIN_ROOM_MESSAGE_H

#include <string>
#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"

class CJoinRoomMessage : public CNetworkMessage{
    public:
        CJoinRoomMessage(std::string username,std::string room_name);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CJoinRoomMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
        std::string DRoomName;
};
#endif
