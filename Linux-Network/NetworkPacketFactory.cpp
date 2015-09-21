#include "NetworkPacketFactory.h"

using namespace std;


CNetworkPacketFactory::CNetworkPacketFactory()
{
    //ctor
}

CNetworkPacketFactory::~CNetworkPacketFactory()
{
    //dtor
}

//Function takes in a clear buffer and its size, the user name and password.
//. It writes the packet to the buffer and it returns an integer. The integer returned
// can be the size of the packet if greater than zero or -1 if the finished packet is
//forcasted to be bigger than the buffer provided.
/**
 * Given the correct inputs, it writes AuthActionPacket to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer.
 * @param username Provided username.
 * @param password Provided Password
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::AuthActionPacket(char *buffer, int buffer_length, std::string username, std::string password){
    int32_t packet_size;
    int16_t action = (int16_t)CLIENT_AUTH;
    static const char * snprintf_template = "%s,%s,";
    char* pos;
    int length = 0;
    pos = buffer;


    packet_size = sizeof(packet_size)+sizeof(action)+username.size()+password.size()+2;//two commas

    if(packet_size > MAX_MESSAGE_SIZE){
        //TODO handle error with exception instead.
        std::cout << "Buffer provided is too small\n";
        return -1;
    }
    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    length += snprintf(pos,MAX_MESSAGE_SIZE-length,snprintf_template,username.c_str(),password.c_str());

    //Finished template doesn't match size or an error occurred creating it.
    if(length != packet_size) return -1;

    return packet_size;
}

//Function takes in a clear buffer and its size and the username.
//It writes the packet to the buffer and it returns an integer. The integer returned
// can be the size of the packet if greater than zero or -1 if the finished packet is
//forcasted to be bigger than the buffer provided.
/**
 * Given the correct inputs, it writes GetAvailableRoomsActionPacket to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer.
 * @param username  Provided username
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::GetAvailableRoomsActionPacket(char *buffer, int buffer_length, std::string username){

    static const char * snprintf_template = "%s,";
    int32_t packet_size;
    int16_t action = (int16_t) GET_AVAILABLE_ROOMS;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+sizeof(action)+username.size()+1;//One comma

    if(packet_size > MAX_MESSAGE_SIZE){
        //TODO handle error with exception instead.
        std::cout << "Buffer provided is too small\n";
        return -1;
    }

    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    length += snprintf(pos,MAX_MESSAGE_SIZE-length,snprintf_template,username.c_str());
    //Finished packet doesn't match expected size.
    if(length != packet_size) return -1;

    return packet_size;
}
/**
 * Given the correct inputs, it writes CreateRoomActionPacket to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer.
 * @param username  Provided username
 * @param room_name Name of the room.
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::CreateRoomActionPacket(char *buffer, int buffer_length, int16_t capacity, std::string username, std::string room_name){
    static const char * snprintf_template = "%s,%s,";
    int32_t packet_size;
    int16_t action = (int16_t) CREATE_ROOM;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+sizeof(action)+sizeof(capacity)+username.size()+room_name.size()+2;//two commas
    if(packet_size > MAX_MESSAGE_SIZE){
        //TODO handle error with exception instead.
        std::cout << "Buffer provided is too small\n";
        return -1;
    }
    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    *(int16_t*) (pos+length) = htons(capacity);
    length += sizeof(capacity);
    length += snprintf((pos+length),MAX_MESSAGE_SIZE-length,snprintf_template,username.c_str(),room_name.c_str());
    //Finished template doesn't match size or an error occurred creating it.
    if(length != packet_size){
        std::cout << "Incorrectly sized packet.\n";
        return -1;
    }

    return packet_size;
}

/**
 * Given the correct inputs, it writes JoinRoomActionPacket to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer.
 * @param username  Provided username
 * @param room_name Name of the room.
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::JoinRoomActionPacket(char *buffer, int buffer_length, std::string username, std::string room_name){
    static const char * snprintf_template = "%s,%s,";
    int32_t packet_size;
    int16_t action = (int16_t) JOIN_ROOM;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+sizeof(action)+username.size()+room_name.size()+2;//two commas
    if(packet_size > MAX_MESSAGE_SIZE){
        std::cout << "Buffer provided is too small\n";
        return -1;
    }
    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    length += snprintf(pos,MAX_MESSAGE_SIZE-length,snprintf_template,username.c_str(),room_name.c_str());
    //Finished template doesn't match size or an error occurred creating it.
    if(length != packet_size) return -1;

    return packet_size;
}

/**
 * Given the correct inputs, it writes DISCONNECT Packet to the provided buffer and returns its size.
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::DisconnectActionPacket(char *buffer, int buffer_length){
    int32_t packet_size;
    int16_t action = (int16_t) DISCONNECT;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+sizeof(action);//two commas
    if(packet_size > MAX_MESSAGE_SIZE){
        std::cout << "Buffer provided is too small\n";
        return -1;
    }
    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    if(length != packet_size) return -1;

    return packet_size;
}

/**
 * Given the correct inputs, it writes LeaveRoomActionPacket to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer.
 * @param username  Provided username
 * @param room_name Name of the room.
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::LeaveRoomActionPacket(char *buffer, int buffer_length, std::string username, std::string room_name){
    static const char * snprintf_template = "%s,%s,";
    int32_t packet_size;
    int16_t action = (int16_t) LEAVE_ROOM;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+sizeof(action)+username.size()+room_name.size()+2;//two commas
    if(packet_size > MAX_MESSAGE_SIZE){
        std::cout << "Buffer provided is too small\n";
        return -1;
    }
    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    length += snprintf(pos,MAX_MESSAGE_SIZE-length,snprintf_template,username.c_str(),room_name.c_str());
    //Finished template doesn't match size or an error occurred creating it.
    if(length != packet_size) return -1;

    return packet_size;
}

int32_t CNetworkPacketFactory::EndGameActionPacket(char *buffer, int buffer_length,int playerID, int winnerID){
    int32_t packet_size;
    int16_t action = (int16_t) END_GAME;
    int16_t _playerID = (int16_t) playerID;
    int16_t _winnerID = (int16_t) winnerID;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+3*sizeof(action);
    if(packet_size > MAX_MESSAGE_SIZE){
        std::cout << "Buffer provided is too small\n";
        return -1;
    }
    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    *(int16_t*) (pos) = htons(_playerID);
    pos += sizeof(_playerID);
    length += sizeof(_playerID);

    *(int16_t*) (pos) = htons(_winnerID);
    pos += sizeof(_winnerID);
    length += sizeof(_winnerID);

    //Finished template doesn't match size or an error occurred creating it.
    if(length != packet_size) return -1;

    return packet_size;

}



/**
 * Given the correct inputs, it writes a StartGame packet to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer. * @param action
 * @param username Player's username
 * @param map_data Selected map's related data.
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::StartGameActionPacket(char *buffer, int buffer_length,std::string username, std::string map_data){
    static const char * snprintf_template = "%s,%s,";
    int32_t packet_size;
    int16_t action = (int16_t) START_GAME;
    char* pos;
    pos = buffer;
    int length = 0;

    packet_size = sizeof(packet_size)+sizeof(action)+username.size()+map_data.size()+2;//two commas
    if(packet_size > MAX_MESSAGE_SIZE){
        std::cout << "Buffer provided is too small\n";
        return -1;
    }

    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    length += snprintf(pos,MAX_MESSAGE_SIZE-length,snprintf_template,username.c_str(),map_data.c_str());
    //Finished template doesn't match size or an error occurred creating it.
    if(length != packet_size) return -1;

    return packet_size;
}

/**
 * Given the correct inputs, it writes ActionPacket to the provided buffer and returns its size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param MAX_MESSAGE_SIZE The length of the provided buffer. * @param action
 * @param time_step The time step of the current action.
 * @param player_id  Player id assigned by server.
 * @param x_coordinate Client's x coordinate converted to protocol coordinates
 * @param y_coordinate Client's y coordinate converted to protocol coordinates
 * @return The length of the finished packet or -1 if an error occurred.
 */
int32_t CNetworkPacketFactory::ActionPacket(char * buffer, int buffer_length, int16_t action, int32_t time_step, int16_t player_id, int32_t x_coordinate, int32_t y_coordinate){

    //Calculated packet size
    int32_t packet_size;
    //Actual packet length
    int length = 0;
    char* pos;
    pos = buffer;


    packet_size = sizeof(packet_size)+sizeof(action)+sizeof(time_step)+sizeof(player_id)+sizeof(x_coordinate)+sizeof(y_coordinate);
    if(packet_size > MAX_MESSAGE_SIZE){
        std::cout << "Buffer provided is too small\n";
        return -1;
    }

    length += CNetworkPacketFactory::SetUpHeader(pos,packet_size,action);
    pos += length;

    *(int32_t*) (pos) = htonl(time_step);
    pos += sizeof(time_step);
    length += sizeof(time_step);

    *(int16_t*) (pos) = htons(player_id);
    pos += sizeof(player_id);
    length += sizeof(player_id);

    *(int32_t*) (pos) = htonl(x_coordinate);
    pos += sizeof(x_coordinate);
    length += sizeof(x_coordinate);

    *(int32_t*) (pos) = htonl(y_coordinate);
    pos += sizeof(y_coordinate);
    length += sizeof(y_coordinate);

    //Let's make sure the size we wrote in the packet is correct.
    if(length != packet_size){
        return -1;  
    } 
    return length;
}

/**Helper method: It writes packet_size and action to the provided buffer as per protocol and it returns its finished size.
 * @param buffer An empty buffer in which the finished packet will be placed.
 * @param packet_size size of the packet in host byte order.
 * @param action Action code in host byte order.
 * @return The length of the finished packet or -1 if an error occurred.
 */

int CNetworkPacketFactory::SetUpHeader(char* buffer, int32_t packet_size, int16_t action){
    char *pos;
    pos = buffer;
    int length = 0;
    *(int32_t*) (pos) = htonl(packet_size);
    pos += sizeof(packet_size);
    length += sizeof(packet_size);

    *(int16_t*) (pos) = htons(action);
    pos += sizeof(action);
    length += sizeof(action);
    return length;
}
