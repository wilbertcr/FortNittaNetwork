using System;
using System.IO;

namespace FortNitta.Network
{  
  static public class Constants
  {

    //Packets sent by client only    
    public const short CLIENT_AUTH = 0;
    public const short GET_AVAILABLE_ROOMS = 1;
    public const short CREATE_ROOM = 2;
    public const short JOIN_ROOM = 3;
    public const short DISCONNECT = 4;
    public const short LEAVE_ROOM = 6;
    public const short START_GAME = 7;

    //Packets sent by Server only

    public const short SERVER_AUTH_ACTION = 0;
    public const short SERVER_SEND_AVAILABLE_ROOMS_ACTION = 1;
    public const short SERVER_CREATE_ROOM_ACTION = 2;
    public const short SERVER_ALLOW_JOIN_ROOM_ACTION = 3;
    public const short SERVER_DENY_JOIN_ROOM_ACTION = 4;
    public const short SERVER_PLAYER_JOINED_ROOM_ACTION = 5;
    public const short SERVER_LEAVE_ROOM_ACTION = 6;
    public const short SERVER_OWNER_LEFT_ROOM_ACTION = 7;
    public const short SERVER_START_GAME_ACTION = 8;

    //Packets sent by both the client and the server.
    public const short POINTER_LOCATION = 10;
    public const short SELECTED_CASTLE = 11;
    public const short PLACED_CANNON = 12;
    public const short FIRED_CANNON = 13;
    public const short ROTATED_WALL = 14;
    public const short PLACED_WALL = 15;
    public const short END_GAME = 16;

    public const short MAX_MESSAGE_SIZE = 4096;
    public const short DEFAULT_MESSAGE_BUFFER_SIZE = 4096;
    public const int NETWORK_ENCODING = 28591;
    public static int testtimeout = 50;
  }
}

