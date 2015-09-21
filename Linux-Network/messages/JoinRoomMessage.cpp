#include "JoinRoomMessage.h"

CJoinRoomMessage::CJoinRoomMessage(std::string username,std::string room_name) : CNetworkMessage() {
    DUsername = username;
    DRoomName = room_name;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) JOIN_ROOM;
}

void CJoinRoomMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->JoinRoomActionPacket(DMessage,MAX_MESSAGE_SIZE, DUsername, DRoomName);
}

char* CJoinRoomMessage::GetMessage(){
    return DMessage;
}

const int CJoinRoomMessage::GetMessageLength(){
    return DMessageLength;
}

CJoinRoomMessage::~CJoinRoomMessage(){
    delete DMessage;
    delete DPacketFactory;
}
