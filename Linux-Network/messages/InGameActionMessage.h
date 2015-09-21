#ifndef CIN_GAME_MESSAGE_H
#define CIN_GAME_MESSAGE_H

#include "NetworkMessage.h"
#include "../NetworkPacketFactory.h"
#include "../NetworkConstants.h"
#include <string>

class CInGameMessage : public CNetworkMessage{
    public:
        CInGameMessage(int16_t action, int time_step, int player_id, int x_coordinate, int y_coordinate);
        void MakeMessage();
        char* GetMessage();
        const int GetMessageLength();
        ~CInGameMessage();
    protected:
        CNetworkPacketFactory* DPacketFactory;
        char* DMessage;
        int DMessageLength;
        uint16_t DAction;
        uint32_t DTimeStep;
        uint16_t DPlayerID;
        uint32_t DXCoordinate;
        uint32_t DYCoordinate;
};
#endif
