#ifndef NETWORKRECEIVE_H
#define NETWORKRECEIVE_H

#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include "unistd.h"
#include "stdio.h"
#include "errno.h"

#include <string>
#include <deque>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include "responses/NetworkResponse.h"
#include "NetworkConstants.h"
#include "ThreadMutex.h"

class CNetworkReceive {
    public:
		CNetworkReceive(int sock, std::deque<CNetworkResponse*>* DE, CThreadMutex* mutexin, bool &in_game, int &number_of_players);
        void ReceivePacket(int sock);
    private:
        int socket;
        void ParsePacket(char *message_buffer, int32_t packet_length);
        void ParseAuthActionPacket(char* packet);
        void ParseSendAvailableRoomsPacket(char *packet);
        void ParseNewRoomResponsePacket(char *packet);
        void ParseAllowJoinRoomPacket(char *packet);
        void ParseDenyJoinRoomPacket(char *packet);
        void ParsePlayerJoinedRoomPacket(char *packet);
        void ParseLeaveRoomPacket(char *packet);
        void ParseOwnerLeftRoomPacket(char *packet);        
        void ParseStartGamePacket(char* packet);
        void ParseActionPacket(char* packet, int16_t action_code);
        void ParseEndGamePacket(char* packet);
        CThreadMutex *DNetworkInMutex;
        bool DInGame;
        int DNumberOfPlayers;
        
    };

#endif
