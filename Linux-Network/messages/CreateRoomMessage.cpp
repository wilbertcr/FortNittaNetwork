#include "CreateRoomMessage.h"

CCreateRoomMessage::CCreateRoomMessage(int capacity,std::string username,std::string room_name) : CNetworkMessage() {
    DUsername = username;
    DRoomName = room_name;
    DCapacity = capacity;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) CREATE_ROOM;
}

void CCreateRoomMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->CreateRoomActionPacket(DMessage,MAX_MESSAGE_SIZE,DCapacity, DUsername, DRoomName);
}

char* CCreateRoomMessage::GetMessage(){
    return DMessage;
}

const int CCreateRoomMessage::GetMessageLength(){
    return DMessageLength;
}

CCreateRoomMessage::~CCreateRoomMessage(){
    delete DMessage;
    delete DPacketFactory;
}

