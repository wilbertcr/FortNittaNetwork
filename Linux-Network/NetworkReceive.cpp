#include "NetworkReceive.h"

#include <ctime>
#include <chrono>
#include "../game/Room.h"
#include "responses/LoginResponse.h"
#include "responses/RoomsAvailableResponse.h"
#include "responses/RoomJoinedResponse.h"
#include "responses/RoomDeniedResponse.h"
#include "responses/RoomCreatedResponse.h"
#include "responses/PlayerJoinedResponse.h"
#include "responses/PlayerLeftResponse.h"
#include "responses/PointerLocationResponse.h"
#include "responses/SelectedCastleResponse.h"
#include "responses/PlacedCannonResponse.h"
#include "responses/FiredCannonResponse.h"
#include "responses/RotatedWallResponse.h"
#include "responses/PlacedWallResponse.h"
#include "responses/StartGameResponse.h"
#include "responses/OwnerLeftRoomResponse.h"
#include "responses/EndGameResponse.h"
#include "NetworkPacketFactory.h"


std::deque<CNetworkResponse*>* DEvents;

CNetworkReceive::CNetworkReceive(int sock, std::deque<CNetworkResponse*>* DE, CThreadMutex* mutexin, bool &in_game, int &number_of_players)
{
    socket = sock;
    DEvents = DE;
    DInGame = in_game;
    DNumberOfPlayers = number_of_players;
    DNetworkInMutex = mutexin;
}

void CNetworkReceive::ReceivePacket(int sock) {
    //NOTE: this should take care of reading
    std::ofstream Log("Log.txt",std::ofstream::app);
    int32_t time_step = 5;
    int16_t playerid = 2;
    int32_t x = 1000;
    int32_t y= 500;
    int16_t action= 14;
    char buffer[512];
    int32_t packet_length = 0;
    int bytes_read = 0;
    //get length of the packet in network byte order.
    bytes_read = read(sock,&packet_length,sizeof(packet_length));


// ...


    if(bytes_read <= 0){
        #ifdef LOG_ON
            Log << "Error: Bytes received received: " << bytes_read << "\n";
            Log.flush();
            Log.close();
        #endif
        return;
    }
    if(ntohl(packet_length) <= 0){
        #ifdef LOG_ON
        Log << "Received packet advertising length of zero.\n";
        #endif
        return;
    }
    #ifdef LOG_ON
    Log << "1:Packet length received: " << ntohl(packet_length) << "\n";
    Log.flush();
    #endif
    //Allocate buffer of the size of the message
    char message_buffer[ntohl(packet_length)+1];
    //We need to write the length into the message buffer(in network byte order), as it'll be needed later.
    *(int32_t*)(message_buffer) = packet_length;
    packet_length = ntohl(packet_length);
    //Let's read the rest of the packet.
    #ifdef LOG_ON
    Log << "2:Reading the packet.\n";
    Log.flush();
    #endif
    //Let's make sure the entire message is on the
    while(bytes_read < packet_length){
        int read_result = read(sock,message_buffer+bytes_read, packet_length-bytes_read);
        if(read_result < 0){
            #ifdef LOG_ON
            Log << "Error reading packet from socket\n";
            #endif
            return;
        } else {
            bytes_read += read_result;
        }
    }
    //send packet to be decoded
    #ifdef LOG_ON
    Log << "3:Parsing packet of length:" << bytes_read << "\n";
    Log.flush();
    Log.close();
    #endif
    Log << "timestep# " << ntohl(*(int32_t*)(message_buffer+6)) << "\n";
    ParsePacket(message_buffer, packet_length);

}

void CNetworkReceive::ParsePacket(char *message_buffer, int32_t packet_length) {
    int32_t packetSize;
    int16_t actionCode;
    std::ofstream Log("Log.txt",std::ofstream::app);

    //packetSize = getPacketSize(message_buffer);
    packetSize = *(int32_t*) message_buffer;
    packetSize = ntohl(packetSize);
    //NOTE: NEED TO LOCALLY OFFSET THESE POINTERS
    message_buffer += sizeof(packetSize);
    //actionCode = getAction(message_buffer);
    actionCode = *(int16_t*) message_buffer;
    actionCode = ntohs(actionCode);

    message_buffer += sizeof(actionCode);
    #ifdef LOG_ON

    #endif
    //TODO: PacketSize error checking
    switch (actionCode) {
            //AUTH_ACTION
            case SERVER_AUTH_ACTION:
                //LoginInResponse obj pushed to DEvents
                #ifdef LOG_ON
                    Log << "6:Calling ParseAuthActionPacket: " << "\n";
                    Log.close();
                #endif
                ParseAuthActionPacket(message_buffer);
                DInGame = false;
                break;
            //SEND_AVAILABLE_ROOMS
            case SERVER_SEND_AVAILABLE_ROOMS_ACTION:
            #ifdef LOG_ON
                    Log << "6:Calling ParseSendAvailableRoomsPacket: " << "\n";
                    Log.close();
            #endif
                //RoomsAvailableResponse obj pushed to DEvents
                ParseSendAvailableRoomsPacket(message_buffer);
                DInGame = false;
                break;
            //NEW_ROOM_RESPONSE
            case SERVER_CREATE_ROOM_ACTION:
                #ifdef LOG_ON
                    Log << "6:Calling ParseNewRoomResponsePacket: " << "\n";
                    Log.close();
                #endif
                //RoomCreatedResponse obj pushed to DEvents
                ParseNewRoomResponsePacket(message_buffer);
                DInGame = false;
                break;
            //ALLOW_JOIN_ROOM
            case SERVER_ALLOW_JOIN_ROOM_ACTION:
                #ifdef LOG_ON
                    Log << "6:Calling ParseAllowJoinRoomPacket: " << "\n";
                    Log.close();
                #endif
                //RoomJoinedResponse obj pushed to DEvents
                ParseAllowJoinRoomPacket(message_buffer);
                DInGame = false;
                break;
            //DENY_JOIN_ROOM
            case SERVER_DENY_JOIN_ROOM_ACTION:
                #ifdef LOG_ON
                    Log << "6:Calling ParseDenyJoinRoomPacket: " << "\n";
                    Log.close();
                #endif
                //RoomDeniedResponse obj pushed to DEvents
                //NOTE: RoomDeniedReponse's process() needs to be implemented
                ParseDenyJoinRoomPacket(message_buffer);
                DInGame = false;
                break;
            //PLAYER_JOINED_ROOM
            case SERVER_PLAYER_JOINED_ROOM_ACTION:
                //PlayerJoinedResponse
                ParsePlayerJoinedRoomPacket(message_buffer);
                break;
            //LEAVE_ROOM
            case SERVER_LEAVE_ROOM_ACTION:
                #ifdef LOG_ON
                    Log << "6:Calling ParseLeaveRoomPacket: " << "\n";
                    Log.close();
                #endif
                //PlayerLeftResponse obj pushed to DEvents
                ParseLeaveRoomPacket(message_buffer);
                DInGame = false;
                break;
            //SERVER_START_GAME
            case SERVER_OWNER_LEFT_ROOM_ACTION:
                #ifdef LOG_ON
                    Log << "6:Calling ParseOwnerLeftRoomPacket: " << "\n";
                    Log.close();
                #endif
                ParseOwnerLeftRoomPacket(message_buffer);
                DInGame = false;
                break;
            case SERVER_START_GAME_ACTION:
                #ifdef LOG_ON
                Log << "6:Calling ParseStartGamePacket: " << "\n";
                Log.close();
                #endif
                ParseStartGamePacket(message_buffer);
                DInGame = true;
                break;
            case POINTER_LOCATION:
                #ifdef LOG_ON
                    Log << "6:Calling ParseActionPacket with code " << POINTER_LOCATION << "\n";
                    Log.close();
                #endif
                ParseActionPacket(message_buffer,actionCode);
                DInGame = true;
                break;
            case SELECTED_CASTLE:
                #ifdef LOG_ON
                    Log << "6:Calling ParseActionPacket with code " << SELECTED_CASTLE <<"\n";
                    Log.close();
                #endif
                ParseActionPacket(message_buffer,actionCode);
                DInGame = true;
                break;
            case PLACED_CANNON:
                #ifdef LOG_ON
                Log << "6:Calling ParseActionPacket with code " << PLACED_CANNON<<  "\n";
                Log.close();
                #endif
                ParseActionPacket(message_buffer,actionCode);
                DInGame = true;
                break;
            case FIRED_CANNON:
                #ifdef LOG_ON
                Log << "6:Calling ParseActionPacket with code " << FIRED_CANNON << "\n";
                Log.close();
                #endif
                ParseActionPacket(message_buffer,actionCode);
                DInGame = true;
                break;
            case ROTATED_WALL:
                #ifdef LOG_ON
                Log << "6:Calling ParseActionPacket with code " << ROTATED_WALL << "\n";
                Log.close();
                #endif
                ParseActionPacket(message_buffer,actionCode);
                DInGame = true;
                break;
            case PLACED_WALL:
                #ifdef LOG_ON
                Log << "6:Calling ParseActionPacket with code " << PLACED_WALL << "\n";
                Log.close();
                #endif
                ParseActionPacket(message_buffer,actionCode);
                DInGame = true;
                break;
            case END_GAME:
				#ifdef LOG_ON
				Log << "6:Calling ParseEndGamePacket with code " << END_GAME << "\n";
				Log.close();
				#endif
				ParseEndGamePacket(message_buffer);
				DInGame = false;
				break;
            default:
                break;
        }
}

void CNetworkReceive::ParseAuthActionPacket(char* packet) {
    char* cptr;
    int result;

    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    #endif

    result = ntohs(*(int16_t*)packet);

    #ifdef LOG_ON
    Log << "7: Authentication result: " << result << "\n";
    Log.flush();
    #endif

    packet += sizeof(int16_t);

    #ifdef LOG_ON
    Log << "8: Pushing LoginResponse" << "\n";
    Log.flush();
    #endif
    DNetworkInMutex->Lock();
    DEvents->push_back(new CLoginResponse(result));
    DNetworkInMutex->Unlock();

    #ifdef LOG_ON
    Log << "9: Pushed LoginResponse" << "\n";
    Log.flush();
    Log.close();
    #endif
}

void CNetworkReceive::ParseSendAvailableRoomsPacket(char* packet) {
    //Decode packet and generate RoomsAvailableResponse

    int numberOfRooms;
    std::vector<CRoom*> rooms;
    std::string roomname = "";
    numberOfRooms = ntohs(*(int16_t*)packet);

    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    Log << "7: Rooms received: " << numberOfRooms << "\n";
    Log.flush();
    #endif

    packet += sizeof(int16_t);

    char* cptr;
    //NOTE: PACKET SHOULD ALREADY BE POINTING TO FIRST Roomname
    if(numberOfRooms == 1){
        cptr = strtok(packet, ",");
        roomname = cptr;
        rooms.push_back(new CRoom(roomname,2));
    }
    if(numberOfRooms > 1){
        cptr = strtok(packet, ",");
        roomname = cptr;
        rooms.push_back(new CRoom(roomname,2));
        for(int i = 2; i <= numberOfRooms; i++){
            cptr = strtok(NULL, ",");
            roomname = cptr;
            rooms.push_back(new CRoom(roomname,2));
        }
    }
    DNetworkInMutex->Lock();
    DEvents->push_back(new CRoomsAvailableResponse(rooms));
    DNetworkInMutex->Unlock();
    #ifdef LOG_ON
    Log << "8: Response pushed into incoming queue:\n";
    Log.close();
    #endif
}

void CNetworkReceive::ParseNewRoomResponsePacket(char *packet) {
    //Decode packet and generate RoomCreatedResponse
    int result;
    char* cptr;
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    #endif
    //NOTE: Addition of Result field
    result = ntohs(*(int16_t*)packet);
    packet += sizeof(int16_t);
    cptr =  strtok(packet, ",");
    std::string room_name = std::string(cptr);
    //Make sure to deal with denial.
    if (1 == result){
        //NOTE: PACKET SHOULD ALREADY BE POINTING TO FIRST Roomname
        #ifdef LOG_ON
        Log << "7: Server said yes to request for room: " << room_name << "\n";
        Log.flush();
        #endif
        DNetworkInMutex->Lock();
        DEvents->push_back(new CRoomCreatedResponse(result, room_name));
        DNetworkInMutex->Unlock();
    } else {
        DNetworkInMutex->Lock();
        DEvents->push_back(new CRoomCreatedResponse(result, room_name));
        DNetworkInMutex->Unlock();
        #ifdef LOG_ON
        Log << "8: Server said no to request\n";
        Log.flush();
        #endif
    }
    #ifdef LOG_ON
    Log.close();
    #endif
}

void CNetworkReceive::ParseAllowJoinRoomPacket(char *packet) {
    //Decode packet and generate RoomJoinedResponse
    char* cptr;
    int player_count = ntohs(*(int16_t*)packet);
    packet += sizeof(int16_t);

    cptr = strtok(packet,",");
    std::string room_name = std::string(cptr);
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    Log << "7: ALLOW_JOIN_ROOM received for room: " << room_name << "\n";
    Log.flush();
    #endif
    std::vector <std::string> players;
    for (int i = 0; i < player_count; i++) {
            cptr = strtok(NULL,",");
            players.push_back(std::string(cptr));
    }
    DNetworkInMutex->Lock();
    DEvents->push_back(new CRoomJoinedResponse(players,player_count,room_name));
    DNetworkInMutex->Unlock();
    #ifdef LOG_ON
    Log << "8: Pushed back to queue: " << room_name << "\n";
    Log.close();
    #endif
}

void CNetworkReceive::ParseDenyJoinRoomPacket(char *packet) {
    //generate a rejected room join response (not given)
    char* cptr;
    cptr = strtok(packet, ",");
    std::string room_name  = std::string(cptr);
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    Log << "7: DENY_JOIN_ROOM received for room: " << room_name << "\n";
    #endif
    //NOTE: RoomDeniedResponse's process() not implemented
    DNetworkInMutex->Lock();
    DEvents->push_back(new CRoomDeniedResponse(room_name));
    DNetworkInMutex->Unlock();
}

void CNetworkReceive::ParsePlayerJoinedRoomPacket(char* packet){
    char* cptr;

    cptr = strtok(packet, ",");
    std::string room_name = std::string(cptr);
    cptr = strtok(NULL,",");
    std::string username  = std::string(cptr);
    std::ofstream Log("Log.txt",std::ofstream::app);
    #ifdef LOG_ON
    Log << "7: PLAYER_JOINED_ROOM received from player: " << username << "\n";
    #endif
    DNetworkInMutex->Lock();
    DEvents->push_back(new CPlayerJoinedResponse(username, room_name));
    DNetworkInMutex->Unlock();
}

void CNetworkReceive::ParseLeaveRoomPacket(char *packet) {
    //Decode packet and generate PlayerLeftResponse
    char* cptr;
    cptr = strtok(packet, ",");
    std::string username = std::string(cptr);
    cptr = strtok(NULL,",");
    std::string room_name  = std::string(cptr);
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    Log << "7: PLAYER_LEAVE_ROOM received from player: " << username << "\n";
    #endif
    DNetworkInMutex->Lock();
    DEvents->push_back(new CPlayerLeftResponse(username, room_name));
    DNetworkInMutex->Unlock();
}

void CNetworkReceive::ParseOwnerLeftRoomPacket(char *packet){
    char * cptr;
    cptr = strtok(packet, ",");
    std::string username = std::string(cptr);
    cptr = strtok(NULL,",");
    std::string room_name = std::string(cptr);
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    Log << "7: OWNER_LEFT_ROOM received from player: " << username << "\n";
    #endif
    DNetworkInMutex->Lock();
    DEvents->push_back(new COwnerLeftRoomResponse(username, room_name));
    DNetworkInMutex->Unlock();
}

void CNetworkReceive::ParseStartGamePacket(char *packet) {
    int num_players = ntohs(*(int16_t*)packet);
    packet += sizeof(int16_t);
    std::vector<std::string> players;
    //NOTE: new MAP field
    std::string map;
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    #endif
    if(num_players <=0 ){
        #ifdef LOG_ON
        Log << "Error: Start Game response with zero players received." << "\n";
        #endif
        //DEvents->push_back(new CStartGameResponse(players,map));
        return;
    }

    //NOTE: new id fields
    int player_ids[num_players];
    for (int i = 0; i < num_players; i++){
        player_ids[0] = ntohs(*(int16_t*)packet);
        packet += sizeof(int16_t);
    }

    //NOTE: new username fields
    char* cptr;
    cptr = strtok(packet, ",");
    players.push_back(std::string(cptr));
    for (int i = 1; i < num_players; i++){
        cptr = strtok(NULL, ",");
        players.push_back(std::string(cptr));
    }
    cptr = strtok(NULL, ",");
    map = std::string(cptr);
    #ifdef LOG_ON
    Log << "7: START_GAME_PACKET received, received: " << num_players << " players.\n";
    #endif
    DNetworkInMutex->Lock();
    DEvents->push_back(new CStartGameResponse(players,map));
    DNetworkInMutex->Unlock();
    DNumberOfPlayers = num_players;
}

void CNetworkReceive::ParseEndGamePacket(char* packet) {
	std::ofstream Log("Log.txt",std::ofstream::app);
    int16_t winnerID  = ntohs(*(int16_t*)packet);
    packet += sizeof(int16_t);
    DNetworkInMutex->Lock();
    DEvents->push_back(new CEndGameResponse(winnerID));
    DNetworkInMutex->Unlock();
    #ifdef LOG_ON
    Log << "7: END_GAME packet received, winnerID: " << winnerID << "\n";
    #endif
    DNumberOfPlayers = -1;
}


void CNetworkReceive::ParseActionPacket(char* packet, int16_t action_code) {
    int time_step  = ntohl(*(int32_t*)packet);
    packet += sizeof(int32_t);
    int player_ID  = ntohs(*(int16_t*)packet);
    packet += sizeof(int16_t);
    int x_coord  = ntohl(*(int32_t*)packet);
    packet += sizeof(int32_t);
    int y_coord  = ntohl(*(int32_t*)packet);
    #ifdef LOG_ON
    std::ofstream Log("Log.txt",std::ofstream::app);
    if(x_coord != 0 && y_coord != 0){
        Log << "7: ACTION_PACKET code: " << action_code << " PlayerID: " << player_ID << " x-coord: " << x_coord << " y-coord: " << y_coord << "\n";
        Log.flush();
    }
    #endif
    switch(action_code) {
            case POINTER_LOCATION:
                DNetworkInMutex->Lock();
                DEvents->push_back(new CPointerLocationResponse(time_step, player_ID, x_coord,y_coord));
                DNetworkInMutex->Unlock();
                #ifdef LOG_ON
                Log << "8: Pushed packet to receiving queue.\n";
                #endif
                break;
            case SELECTED_CASTLE:
                DNetworkInMutex->Lock();
                DEvents->push_back(new CSelectedCastleResponse(time_step, player_ID, x_coord,y_coord));
                DNetworkInMutex->Unlock();
                #ifdef LOG_ON
                Log << "8: Pushed response to receiving queue.\n";
                #endif
                break;
            case PLACED_CANNON:
                DNetworkInMutex->Lock();
                DEvents->push_back(new CPlacedCannonResponse(time_step, player_ID, x_coord,y_coord));
                DNetworkInMutex->Unlock();
                #ifdef LOG_ON
                Log << "8: Pushed packet to receiving queue.\n";
                #endif
                break;
            case FIRED_CANNON:
                DNetworkInMutex->Lock();
                DEvents->push_back(new CFiredCannonResponse(time_step, player_ID, x_coord,y_coord));
                DNetworkInMutex->Unlock();
                #ifdef LOG_ON
                Log << "8: Pushed packet to receiving queue.\n";
                #endif
                break;
            case ROTATED_WALL:
                DNetworkInMutex->Lock();
                DEvents->push_back(new CRotatedWallResponse(time_step, player_ID, x_coord,y_coord));
                DNetworkInMutex->Unlock();
                #ifdef LOG_ON
                Log << "8: Pushed packet to receiving queue.\n";
                #endif
                break;
            case PLACED_WALL:
                DNetworkInMutex->Lock();
                DEvents->push_back(new CPlacedWallResponse(time_step, player_ID, x_coord,y_coord));
                DNetworkInMutex->Unlock();
                #ifdef LOG_ON
                Log << "8: Pushed packet to receiving queue.\n";
                #endif
                break;
        }
}
