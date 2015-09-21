#include "DisconnectMessage.h"

CDisconnectMessage::CDisconnectMessage() : CNetworkMessage() {
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) DISCONNECT;
}

void CDisconnectMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->DisconnectActionPacket(DMessage,MAX_MESSAGE_SIZE);
}

char* CDisconnectMessage::GetMessage(){
    return DMessage;
}

const int CDisconnectMessage::GetMessageLength(){
    return DMessageLength;
}

CDisconnectMessage::~CDisconnectMessage(){
    delete DMessage;
    delete DPacketFactory;
}


