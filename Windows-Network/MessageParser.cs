using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FortNitta.Network
{
  static class MessageParser
  {
    /**
     * @brief Parse the given packet, transforming it into an instance of the appropriate Response-derived class
     * @param packet the entire packet buffer, including the header consisting of the size and action
     * @return Response represetnting response to message parsed.
     */
    static public Response parsePacket(byte[] packet)
    {
      MessageBuffer msg = new MessageBuffer(packet);
      Response resp = null;

      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      switch (action)
      {
        case Constants.SERVER_AUTH_ACTION:
          resp = parseAuthAction(msg);
          break;

        case Constants.SERVER_SEND_AVAILABLE_ROOMS_ACTION:
          resp = parseSendAvailableRooms(msg);
          break;

        case Constants.SERVER_CREATE_ROOM_ACTION:
          resp = parseCreateRoom(msg);
          break;

        case Constants.SERVER_ALLOW_JOIN_ROOM_ACTION:
          resp = parseAllowJoinRoom(msg);
          break;

        case Constants.SERVER_DENY_JOIN_ROOM_ACTION:
          resp = parseDenyJoinRoom(msg);
          break;

        case Constants.SERVER_PLAYER_JOINED_ROOM_ACTION:
          resp = parsePlayerJoinedRoom(msg);
          break;

        case Constants.SERVER_LEAVE_ROOM_ACTION:
          resp = parseLeaveRoom(msg);
          break;

        case Constants.SERVER_OWNER_LEFT_ROOM_ACTION:
          resp = parseOwnerLeftRoom(msg);
          break;

        case Constants.SERVER_START_GAME_ACTION:
          resp = parseStartGame(msg);
          break;
        case Constants.END_GAME:
          resp = parseEndGame(msg);
          break;
        case Constants.POINTER_LOCATION:
          resp = parseGameAction(msg);
          break;
        case Constants.SELECTED_CASTLE:
          resp = parseGameAction(msg);
          break;
        case Constants.PLACED_CANNON:
          resp = parseGameAction(msg);
          break;
        case Constants.FIRED_CANNON:
          resp = parseGameAction(msg);
          break;
        case Constants.ROTATED_WALL:
          resp = parseGameAction(msg);
          break;
        case Constants.PLACED_WALL:
          resp = parseGameAction(msg);
          break;
        default:
          resp = null;
          break;
      }
      return resp;
    }//parsePacket

    static public Response parseAuthAction(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int16 result = msg.popShort();
      return new LoginResponse(result);
    }//parseAuthAction

    static public Response parseSendAvailableRooms(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int16 numRooms = msg.popShort();
      List<string> roomNames = new List<string>(numRooms);
      for (int i = 0; i < numRooms; i++)
        roomNames.Add(msg.popString());

      List<Room> rooms = new List<Room>(numRooms);
      for (int i = 0; i < numRooms; i++)
        rooms.Add(new Room(roomNames[i], 2));
      return new RoomsAvailableResponse(rooms);
    }//parseSendAvailableRooms

    static public Response parseCreateRoom(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int16 result = msg.popShort();
      string roomName = msg.popString();
      return new RoomCreatedResponse(result, roomName);
    }//parseCreateRoom

    static public Response parseAllowJoinRoom(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int16 numPlayers = msg.popShort();
      string roomName = msg.popString();
      List<string> playerNames = new List<string>(numPlayers);
      for (int i = 0; i < numPlayers; i++)
        playerNames.Add(msg.popString());
      return new RoomJoinedResponse(roomName, playerNames, numPlayers);
    }//RoomJoinedResponse

    static public Response parseDenyJoinRoom(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      string roomName = msg.popString();
      return new RoomNotJoinedResponse(roomName);
    }//parseDenyJoinRoom

    static public Response parsePlayerJoinedRoom(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      string roomName = msg.popString();
      string userName = msg.popString();
      return new PlayerJoinedRoomResponse(userName, roomName);
    }//parsePlayerJoinedRoom

    static public Response parseLeaveRoom(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      string userName = msg.popString();
      string roomName = msg.popString();
      return new PlayerLeftRoomResponse(userName, roomName);
    }//PlayerLeftRoomResponse

    static public Response parseOwnerLeftRoom(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      string userName = msg.popString();
      string roomName = msg.popString();
      return new OwnerLeftRoomResponse(userName, roomName);
    }//parseOwnerLeftRoom

    static public Response parseStartGame(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int16 numPlayers = msg.popShort();
      List<Int16> playerIds = new List<Int16>(numPlayers);
      for (int i = 0; i < numPlayers; i++)
        playerIds.Add(msg.popShort());
      List<string> playerNames = new List<string>(numPlayers);
      for (int i = 0; i < numPlayers; i++)
        playerNames.Add(msg.popString());
      string map = msg.popString();
      return new StartGameResponse(playerNames, map);
    }//parseStartGame

    static public Response parseEndGame(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int16 winnerID = msg.popShort();
      return new EndGameResponse(winnerID);
    }//parseEndGame

    static public Response parseGameAction(MessageBuffer msg)
    {
      msg.reset();
      Int32 size = msg.popInt();
      Int16 action = msg.popShort();
      Int32 timeStep = msg.popInt();
      Int16 playerId = msg.popShort();
      Int32 xPos = msg.popInt();
      Int32 yPos = msg.popInt();
      switch (action)
      {
        case Constants.POINTER_LOCATION:
          return new PointerLocationResponse(timeStep, playerId, xPos, yPos);
        case Constants.SELECTED_CASTLE:
          return new SelectedCastleResponse(timeStep, playerId, xPos, yPos);
        case Constants.PLACED_CANNON:
          return new PlacedCannonResponse(timeStep, playerId, xPos, yPos);
        case Constants.FIRED_CANNON:
          return new FiredCannonResponse(timeStep, playerId, xPos, yPos);
        case Constants.ROTATED_WALL:
          return new RotatedWallResponse(timeStep, playerId, xPos, yPos);
        case Constants.PLACED_WALL:
          return new PlacedWallResponse(timeStep, playerId, xPos, yPos);
        default:
          return null;
      }
    }//parseGameAction

  }//MessageParser
}//FortNitta.Network