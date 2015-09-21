#include "LeaveRoomMessage.h"

CLeaveRoomMessage::CLeaveRoomMessage(std::string username,std::string room_name) : CNetworkMessage() {
    DUsername = username;
    DRoomName = room_name;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = (int16_t) LEAVE_ROOM;
}

void CLeaveRoomMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->LeaveRoomActionPacket(DMessage,MAX_MESSAGE_SIZE,DUsername, DRoomName);
}

char* CLeaveRoomMessage::GetMessage(){
    return DMessage;
}

const int CLeaveRoomMessage::GetMessageLength(){
    return DMessageLength;
}

CLeaveRoomMessage::~CLeaveRoomMessage(){
    delete DMessage;
    delete DPacketFactory;
}


