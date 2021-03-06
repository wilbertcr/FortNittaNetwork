#ifndef NETWORK_CONSTANTS_H
#define NETWORK_CONSTANTS_H

//Packets sent by client only
#define CLIENT_AUTH 0
#define GET_AVAILABLE_ROOMS 1
#define CREATE_ROOM 2
#define JOIN_ROOM 3
#define DISCONNECT 4
#define LEAVE_ROOM 6
#define START_GAME 7

//Packets sent by Server only

#define SERVER_AUTH_ACTION 0
#define SERVER_SEND_AVAILABLE_ROOMS_ACTION 1
#define SERVER_CREATE_ROOM_ACTION 2
#define SERVER_ALLOW_JOIN_ROOM_ACTION 3
#define SERVER_DENY_JOIN_ROOM_ACTION 4
#define SERVER_PLAYER_JOINED_ROOM_ACTION 5
#define SERVER_LEAVE_ROOM_ACTION 6
#define SERVER_OWNER_LEFT_ROOM_ACTION 7
#define SERVER_START_GAME_ACTION 8

//Packets sent by both the client and the server.
#define POINTER_LOCATION 10
#define SELECTED_CASTLE 11
#define PLACED_CANNON 12
#define FIRED_CANNON 13
#define ROTATED_WALL 14
#define PLACED_WALL 15
#define END_GAME 16
#define MAX_MESSAGE_SIZE 4096
#define LOG_ON

#endif

