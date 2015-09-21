#include "StartGameMessage.h"

CStartGameMessage::CStartGameMessage(std::string username,std::string map_data) : CNetworkMessage() {
    DUsername = username;
    DMapData = map_data;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) START_GAME;
}

void CStartGameMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->StartGameActionPacket(DMessage,MAX_MESSAGE_SIZE,DUsername, DMapData);
}

char* CStartGameMessage::GetMessage(){
    return DMessage;
}

const int CStartGameMessage::GetMessageLength(){
    return DMessageLength;
}

CStartGameMessage::~CStartGameMessage(){
    delete DMessage;
    delete DPacketFactory;
}
