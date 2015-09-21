#ifndef CSTART_GAME_MESSAGE_H
#define CSTART_GAME_MESSAGE_H

#include <string>
#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"

class CStartGameMessage : public CNetworkMessage{
    public:
        CStartGameMessage(std::string username,std::string map_data);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CStartGameMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
        std::string DMapData;
};
#endif
