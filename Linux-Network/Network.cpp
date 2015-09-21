#include "Network.h"
#include <cstring>
#include <gtk/gtk.h>


//Comment out for use on Game
//#define TEST_FRAMEWORK
//#define FAKE_NETWORK

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#ifndef POLLRDHUP
#define POLLRDHUP 0x2000
#endif

#include "responses/LoginResponse.h"
#include "../game/Room.h"
#include "responses/RoomsAvailableResponse.h"
#include "responses/RoomJoinedResponse.h"
#include "responses/RoomCreatedResponse.h"
#include "responses/RoomDeniedResponse.h"
#include "responses/PlayerJoinedResponse.h"
#include "responses/PlayerLeftResponse.h"
#include "responses/PointerLocationResponse.h"
#include "responses/SelectedCastleResponse.h"
#include "responses/PlacedCannonResponse.h"
#include "responses/FiredCannonResponse.h"
#include "responses/RotatedWallResponse.h"
#include "responses/PlacedWallResponse.h"
#include "messages/AuthorizationMessage.h"
#include "messages/GetAvailableRoomsMessage.h"
#include "messages/CreateRoomMessage.h"
#include "messages/JoinRoomMessage.h"
#include "messages/DisconnectMessage.h"
#include "messages/LeaveRoomMessage.h"
#include "messages/EndGameMessage.h"
#include "messages/StartGameMessage.h"
#include "messages/InGameActionMessage.h"
#include "responses/StartGameResponse.h"
#include "responses/EndGameResponse.h"
#include "../game/Game.h"

#ifdef FAKE_NETWORK
#include "FakeNetworkEvents.h"
#endif

int DSocket;
int IncomingProcessedCount = 0;
int received = 0;

CNetwork::CNetwork(size_t stack_size, bool start_suspended) : CThread(stack_size, start_suspended),DLogFile()
{
    DConnectionString = "optical.cs.ucdavis.edu:49999";
    DUsername = "";
    DNetworkOutMutex = new CThreadMutex();
    DNetworkInMutex = new CThreadMutex();
    DConnected = false;
    DLogFile.open("LogFile.txt",std::ofstream::app);
    DReceiver = new CNetworkReceive(DSocket, &DEvents, DNetworkInMutex,DInGame,DNumberOfPlayers);
}

int CNetwork::ConnectToServer() {

    std::string delimiter = ":";
    DHost = DConnectionString.substr(0,DConnectionString.find(delimiter));
    std::string DPort = DConnectionString.substr(DConnectionString.find(delimiter)+1,DConnectionString.size());

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int s;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    s = getaddrinfo(DHost.c_str(), DPort.c_str(), &hints, &result);
    if (s != 0) {
        #ifdef LOG_ON
	    DLogFile << "1: getaddrinfo: " << gai_strerror(s) << "\n"; ;
        DLogFile.flush();
        #endif
        //TODO: Throw exception here.
        return 0;
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully bind(2).
       If socket(2) (or bind(2)) fails, we (close the socket
       and) try the next address. */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        DSocket = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
        if (DSocket == -1){
            continue;
        }
        /* Success */
        if (connect(DSocket, rp->ai_addr, rp->ai_addrlen) == 0) {
            break;
        }
       close(DSocket);
    }

    if (rp == NULL) {               /* No address succeeded */
        //TODO Throw server is down exception.
        #ifdef LOG_ON
	    DLogFile << "1: Could not connect\n";
        DLogFile.flush();
        #endif
        //exit(EXIT_FAILURE);
        return 0;
    }

    freeaddrinfo(result);           /* No longer needed */

    //Getting flags from socket.
    int flags = fcntl(DSocket, F_GETFL, 0);
    if (flags < 0){
       #ifdef LOG_ON
	   DLogFile << "Error getting flags from socket.";
       DLogFile.flush();
       #endif
    } else {
        //Disabling Nagle's Algorithm and Making socket Non-blocking.
        flags = flags&(TCP_NODELAY);
        if(fcntl(DSocket, F_SETFL, flags) != 0){
           #ifdef LOG_ON
	       DLogFile << "Error setting socket flags.";
           DLogFile.flush();
           #endif
            //We can chug along without this flags.
            //Not ideal but better than failing.
            //Some sort of soft exception could be implemented for this.
        }
    }
    return 1;
}


void *CNetwork::Execute(){

    received = 0;
    DConnected = ConnectToServer() ? true : false;
    #ifdef LOG_ON
	DLogFile << "Connection to server result:" << DConnected << "\n";
    #endif
    struct pollfd poll_fd[1];
    poll_fd[0].fd = DSocket;
    poll_fd[0].events = (POLLIN | POLLPRI | POLLRDHUP | POLLHUP | POLLERR | POLLNVAL );
    //Game is running at an update rate of 50ms.
    //We may need to process up to 4 incoming messages per cycle, so
    //We're waiting up to 10ms per packet so we can leav some buffer.
    //Changes to the game's update rate will required adjusting here.
    int timeout = 10;
    int nfds = 1;
    int select_result = 0;
    int bytes_sent = 0;
    int i=0;
    int error_count = 0;
    #ifdef LOG_ON
	DLogFile << "Using socket:" << DSocket << "\n";
    DLogFile.flush();
    #endif
    while(DConnected){
        while(DEventsOut.size()){
            CNetworkMessage *Message = DEventsOut.front();
            bytes_sent = write(DSocket,Message->GetMessage(),Message->GetMessageLength());
            if(bytes_sent == Message->GetMessageLength()){
                DNetworkOutMutex->Lock();
                DEventsOut.pop_front();
                DNetworkOutMutex->Unlock();
                delete Message;
				 #ifdef LOG_ON
				 DLogFile << "1: Sent message of size:" << bytes_sent << "\n";
				 DLogFile.flush();
                 #endif
				 if(DInGame){
					 break;
				 }
            } else if(0 < bytes_sent) {
				#ifdef LOG_ON
				 DLogFile << "1: Unable to send packet completely, partial packet sent.\n";
				 DLogFile.flush();
                 #endif
            } else if(0 == bytes_sent){
				#ifdef LOG_ON
				 DLogFile << "1: Something was wrong with that packet, it had length zero.\n";
                 DLogFile.flush();
                 #endif
            } else {
				#ifdef LOG_ON
				 DLogFile << "1: Error writing to socket.\n";
                 DLogFile.flush();
                 #endif
            }
            #ifdef LOG_ON
	        DLogFile << "1: Processed outgoing event #" << i << "\n";
            #endif
        }


        while(DConnected){
            select_result = poll(poll_fd,nfds,timeout);
            if(select_result < 0){
				#ifdef LOG_ON
				DLogFile << "1: Error on poll.";
                DLogFile.flush();
                #endif
                //This is a deal breaker so we kill the connection.
                goto disconnect;
            }
            if(select_result == 0){
                //We're good, there's just nothing there for us.
                break;
            }

            if((poll_fd[0].revents&POLLHUP) || (poll_fd[0].revents&POLLRDHUP)){
				 #ifdef LOG_ON
				 DLogFile << "1: Remote side hung up.\n";
                 DLogFile.flush();
                 #endif
                //This is a deal breaker so we kill the connection.
                goto disconnect;
            }
            if(poll_fd[0].revents&POLLERR){
				 #ifdef LOG_ON
				 DLogFile << "1: An error occurred on the socket.\n";
                 DLogFile.flush();
                //This is a deal breaker so we kill the connection.
                 #endif
                goto disconnect;
            }
            if(poll_fd[0].revents&POLLNVAL){
				#ifdef LOG_ON
				 DLogFile << "1: Something wrong with file descriptor. - maybe it's uninitialized?\n";
                DLogFile.flush();
                 #endif
                //This is a deal breaker so we kill the connection.
                goto disconnect;
            }
            if(poll_fd[0].revents&(POLLIN | POLLPRI)){
				#ifdef LOG_ON
                DLogFile << "1:Receiving packet number #" << received++ << "\n";
                DLogFile.flush();
                #endif
                DReceiver->ReceivePacket(DSocket);
            }
            //Resetting event flags.
            poll_fd[0].revents = 0;
        }

    }
    disconnect:
        #ifdef LOG_ON
	    DLogFile << "Thread is being killed and joined.\n";
        DLogFile.flush();
        #endif
        //let's cleanup queues.
        while(DEventsOut.size()){
            DNetworkOutMutex->Lock();
            DEventsOut.pop_front();
            DNetworkOutMutex->Unlock();
        }
        while(DEvents.size()){
            DNetworkInMutex->Lock();
            DEvents.pop_front();
            DNetworkInMutex->Unlock();
        }
        DConnected = false;
        close(DSocket);
        Join();
}

/**
 * @param packet Pointer to buffer containing packet.
 * @param packet_length Length of the packet
 * @return Positive number on success, negative on error.
 */

bool CNetwork::Status(){
    return DConnected;
}

/**
 * @return Positive number on success, negative on error.
 */

bool CNetwork::Connect(){
#ifdef FAKE_NETWORK
    return true;
#endif
    if(!DConnected){
        #ifdef LOG_ON
	    DLogFile << "Resuming thread.\n";
        DLogFile.flush();
        #endif
        DConnected = Resume();
        DLogFile << "Resume thread returned:" << DConnected<< "\n";
        #ifdef LOG_ON
        DLogFile.flush();
        #endif
    }
    return DConnected;
    //I don't think would require an exception type.
    //It is too internal to the program.
    //This is best to simply put in the log and fail.
    //for programmers to use as an aid in bug fixing.
}

void CNetwork::Disconnect() {
    if(DConnected){
        DNetworkOutMutex->Lock();
        DEventsOut.push_back(new CDisconnectMessage());
        DNetworkOutMutex->Unlock();
        sleep(1);
    }
    DConnected = false;
}

void CNetwork::LogIn(std::string username, std::string password) {
#ifdef FAKE_NETWORK
    DEvents.push_back(new CLoginResponse(1));
    return;
#endif
    if(!DConnected){
        #ifdef LOG_ON
        DLogFile << "Resuming thread.\n";
        DLogFile.flush();
        #endif
        DConnected = Resume();
        DLogFile << "Resume thread returned:" << DConnected<< "\n";
        #ifdef LOG_ON
        DLogFile.flush();
        #endif
    }
    DUsername = username;
    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CAuthorizationMessage(username,password));
    DNetworkOutMutex->Unlock();
}

void CNetwork::GetAvailableRooms() {
#ifdef FAKE_NETWORK
    std::vector<CRoom*> Rooms;
    Rooms.push_back(new CRoom("fred's game", 2));
    Rooms.push_back(new CRoom("bob's game", 2));
    Rooms.push_back(new CRoom("nitta's game", 2));
    Rooms.push_back(new CRoom("broadcasting game", 2, true));
    Rooms.push_back(new CRoom("sally's game", 2));
    Rooms.push_back(new CRoom("tim's game", 2));
    DEvents.push_back(new CRoomsAvailableResponse(Rooms));
    return;
#endif

    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CGetAvailableRoomsMessage(DUsername));
    DNetworkOutMutex->Unlock();
}

void CNetwork::CreateRoom(int capacity, std::string room_name) {
#ifdef FAKE_NETWORK
    DEvents.push_back(new CRoomCreatedResponse(1, room_name));
    DEvents.push_back(new CPlayerJoinedResponse("bob", room_name));
    DEvents.push_back(new CPlayerJoinedResponse("dave", room_name));
    DEvents.push_back(new CPlayerLeftResponse("dave", room_name));
    DEvents.push_back(new CPlayerJoinedResponse("fred", room_name));
    DEvents.push_back(new CPlayerLeftResponse("fred", room_name));
    return;
#endif
    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CCreateRoomMessage(capacity, DUsername, room_name));
    DNetworkOutMutex->Unlock();
}

void CNetwork::JoinRoom(std::string room_name) {
#ifdef FAKE_NETWORK
    std::vector<std::string> Players;
    Players.push_back("bob");
    Players.push_back("test");
    int capacity = 2;
    DEvents.push_back(new CRoomJoinedResponse(Players,capacity,room_name));
    return;
#endif
    #ifdef LOG_ON
    DLogFile << "Asking to go into room: " << room_name <<"\n";
    DLogFile << "As user: " << DUsername <<"\n";
    DLogFile.flush();
    #endif

    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CJoinRoomMessage(DUsername,room_name));
    DNetworkOutMutex->Unlock();
}


void CNetwork::LeaveRoom(std::string room_name) {
#ifdef FAKE_NETWORK

#endif
    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CLeaveRoomMessage(DUsername,room_name));
    DNetworkOutMutex->Unlock();
}

void CNetwork::EndGame(int playerID, int winnerID){
#ifdef FAKE_NETWORK
    DEvents.push_back(new CEndGameResponse(winnerID));
    return;
#endif
	DNetworkOutMutex->Lock();
	DEventsOut.push_back(new CEndGameMessage(playerID, winnerID));
	DNetworkOutMutex->Unlock();
}



void CNetwork::StartGame(std::string map_data) {
#ifdef FAKE_NETWORK
    std::vector<std::string> Players;
    Players.push_back("test");
    Players.push_back("bob");
    DEvents.push_back(new CStartGameResponse(Players, map_data));
#endif
    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CStartGameMessage(DUsername, map_data));
    DNetworkOutMutex->Unlock();
}




SInt2 CNetwork::ConvertToNetworkPosition(CGame* game, SInt2 screen_position){
    int GAME_HEIGHT = game->GameState()->DConfig.GAME_HEIGHT;
    int GAME_WIDTH = game->GameState()->DConfig.GAME_WIDTH;
    return screen_position*SInt2(10000, 10000)/SInt2(GAME_WIDTH, GAME_HEIGHT);
}

SInt2 CNetwork::ConvertFromNetworkPosition(CGame* game, SInt2 network_position){
    int GAME_HEIGHT = game->GameState()->DConfig.GAME_HEIGHT;
    int GAME_WIDTH = game->GameState()->DConfig.GAME_WIDTH;
    return network_position*SInt2(GAME_WIDTH, GAME_HEIGHT)/SInt2(10000, 10000);
}

void CNetwork::InGameAction(int16_t action, int time_step,  int player_id, int x_coordinate, int y_coordinate) {
#ifdef FAKE_NETWORK
    g_print("%d %d %d %d\n", action, player_id, x_coordinate, y_coordinate);
    return;
#endif
    DNetworkOutMutex->Lock();
    DEventsOut.push_back(new CInGameMessage(action,time_step,player_id,x_coordinate,y_coordinate));
    DNetworkOutMutex->Unlock();
}

#ifdef TEST_FRAMEWORK
//Provided by Linux Team
void CNetwork::Update() {
    while(DEvents.size()) {
        i++;
        #ifdef LOG_ON
	    DLogFile << "Processing packet number:" << i << "\n";
        DLogFile.flush();
        #endif
        DNetworkInMutex->Lock();
        CNetworkResponse* Response = DEvents.front();
        Response->Process();
        DEvents.pop_front();
        DNetworkInMutex->Unlock();
        delete Response;
        #ifdef LOG_ON
	    DLogFile << "Done processed packet number:" << i << "\n";
        DLogFile.flush();
        #endif
    }
}
#endif

#ifndef TEST_FRAMEWORK
//Provided by Linux Team
void CNetwork::Update(CGame* game) {
    game->GameState()->DIsNetworkUpdate = true;
#ifdef FAKE_NETWORK
    UpdateFakeEvents(game, DEvents);
#endif
    while(DEvents.size()) {
        IncomingProcessedCount++;
        #ifdef LOG_ON
	    DLogFile << "Processing packet number:" << IncomingProcessedCount << "\n";
        DLogFile.flush();
        #endif
        DNetworkInMutex->Lock();
        CNetworkResponse* Response = DEvents.front();
        Response->Process(game);
        DEvents.pop_front();
        DNetworkInMutex->Unlock();
        delete Response;
        #ifdef LOG_ON
	    DLogFile << "Done processed packet number:" << IncomingProcessedCount << "\n";
        DLogFile.flush();
        #endif
    }
    game->GameState()->DIsNetworkUpdate = false;
}
#endif

//Provided by Linux Team
const std::string CNetwork::GetConnectionString() {
    return DConnectionString;
}

//Provided by Linux Team.
void CNetwork::SetConnectionString(const std::string &connection_string) {
    DConnectionString = connection_string;
}
