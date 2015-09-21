#ifndef CNETWORK_PACKETFACTORY_H
#define CNETWORK_PACKETFACTORY_H
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <string>
#include <iostream>
#include <fstream>
#include "NetworkConstants.h"

class CNetworkPacketFactory
{
    public:
        CNetworkPacketFactory();
        virtual ~CNetworkPacketFactory();
        int32_t AuthActionPacket(char *buffer,int buffer_length, std::string username, std::string password);
        int32_t GetAvailableRoomsActionPacket(char *buffer, int buffer_length, std::string username);
        int32_t CreateRoomActionPacket(char *buffer, int buffer_length, int16_t capacity, std::string username, std::string room_name);
        int32_t JoinRoomActionPacket(char *buffer, int buffer_length, std::string username, std::string room_name);
        int32_t DisconnectActionPacket(char *buffer, int buffer_length);
        int32_t LeaveRoomActionPacket(char *buffer, int buffer_length, std::string username, std::string room_name);
        int32_t EndGameActionPacket(char *buffer, int buffer_length,int playerID, int winnerID);
        int32_t StartGameActionPacket(char *buffer, int buffer_length,std::string username, std::string map_data);
        int32_t ActionPacket(char * buffer, int buffer_length, int16_t action, int32_t time_step, int16_t player_id, int32_t x_coordinate, int32_t y_coordinate);
        int SetUpHeader(char* buffer, int32_t packet_size, int16_t action);

protected:
    private:
};

#endif // NETWORK_PACKETFACTORY_H
