#include "InGameActionMessage.h"

CInGameMessage::CInGameMessage(int16_t action, int time_step, int player_id, int x_coordinate, int y_coordinate) : CNetworkMessage() {
    DAction = action;
    DTimeStep = (int32_t) time_step;
    DPlayerID = (int16_t) player_id;
    DXCoordinate = (int32_t) x_coordinate;
    DYCoordinate = (int32_t) y_coordinate;
    DMessage = new char[MAX_MESSAGE_SIZE];
    DPacketFactory = new CNetworkPacketFactory();
    DMessageLength = 0;
    MakeMessage();
    DAction = action;
}

void CInGameMessage::MakeMessage(){
    int result = 0;
    DMessageLength = DPacketFactory->ActionPacket(DMessage,MAX_MESSAGE_SIZE,DAction,DTimeStep,DPlayerID,DXCoordinate,DYCoordinate);
}

char* CInGameMessage::GetMessage(){
    return DMessage;
}

const int CInGameMessage::GetMessageLength(){
    return DMessageLength;
}

CInGameMessage::~CInGameMessage(){
    delete DMessage;
    delete DPacketFactory;
}


