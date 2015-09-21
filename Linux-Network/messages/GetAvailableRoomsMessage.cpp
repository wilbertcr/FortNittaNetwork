#include "GetAvailableRoomsMessage.h"

CGetAvailableRoomsMessage::CGetAvailableRoomsMessage(std::string username) : CNetworkMessage() {
    DUsername = username;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) GET_AVAILABLE_ROOMS;
}

void CGetAvailableRoomsMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->GetAvailableRoomsActionPacket(DMessage,MAX_MESSAGE_SIZE, DUsername);
}

char* CGetAvailableRoomsMessage::GetMessage(){
    return DMessage;
}

const int CGetAvailableRoomsMessage::GetMessageLength(){
    return DMessageLength;
}

CGetAvailableRoomsMessage::~CGetAvailableRoomsMessage(){
    delete DMessage;
    delete DPacketFactory;
}
