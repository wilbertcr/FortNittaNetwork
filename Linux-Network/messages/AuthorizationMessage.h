#ifndef AUTHORIZATION_MESSAGE_H
#define AUTHORIZATION_MESSAGE_H

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"

#include <string>

class CAuthorizationMessage : public CNetworkMessage{
    public:
        CAuthorizationMessage(std::string username,std::string password);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CAuthorizationMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        std::string DUsername;
        std::string DPassword;
};
#endif
