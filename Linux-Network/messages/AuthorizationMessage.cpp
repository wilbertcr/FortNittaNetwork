#include "AuthorizationMessage.h"

CAuthorizationMessage::CAuthorizationMessage(std::string username,std::string password) : CNetworkMessage() {
    DUsername = username;
    DPassword = password;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) CLIENT_AUTH;
}

void CAuthorizationMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->AuthActionPacket(DMessage,MAX_MESSAGE_SIZE, DUsername, DPassword);
}

char* CAuthorizationMessage::GetMessage(){
    return DMessage;
}

const int CAuthorizationMessage::GetMessageLength(){
    return DMessageLength;
}

CAuthorizationMessage::~CAuthorizationMessage(){
    delete DMessage;
    delete DPacketFactory;
}
