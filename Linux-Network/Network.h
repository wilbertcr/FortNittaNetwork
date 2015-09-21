#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/poll.h>
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
#include <chrono>
#include <mutex>
#include <vector>
#include <atomic>

#include "../types/Int2.h"
#include "responses/NetworkResponse.h"
#include "messages/NetworkMessage.h"
#include "Thread.h"
#include "ThreadMutex.h"
#include "NetworkReceive.h"
#include "NetworkConstants.h"


class CGame;

class CNetwork: public CThread {
    public:
        CNetwork(size_t stack_size, bool start_suspended);
        //Add fake network to these.
        //Create connect and disconnect.
        bool Connect();

        void Disconnect();

        void LogIn(std::string username, std::string password);

        void GetAvailableRooms();

        void CreateRoom(int capacity, std::string room_name);

        void LeaveRoom(std::string room_name);

        void JoinRoom(std::string room_name);

        void StartGame(std::string map_data);
        
        void EndGame(int playerID, int winnerID);

        SInt2 ConvertFromNetworkPosition(CGame* game, SInt2 network_position);

        SInt2 ConvertToNetworkPosition(CGame* game, SInt2 screen_position);

        void InGameAction(int16_t action, int time_step,  int player_id, int x_coordinate, int y_coordinate);

        void Update(CGame* game);

        void Update();        

        const std::string GetConnectionString();

        void SetConnectionString(const std::string &connection_string);

    protected:
        int ConnectToServer();
        void* Execute();
        bool Terminated();
        bool Status();                     
        std::deque<CNetworkResponse*> DEvents;
        std::deque<CNetworkMessage*> DEventsOut;
        std::string DConnectionString;
        std::string DHost;
        std::string DPort;
        std::string DUsername;
        CThreadMutex *DNetworkOutMutex;
        CThreadMutex *DNetworkInMutex;
        std::atomic<bool> DConnected;
        CNetworkReceive *DReceiver;
        std::ofstream DLogFile;
        int DNumberOfPlayers = -1;
        bool DInGame = false;
    };

#endif
