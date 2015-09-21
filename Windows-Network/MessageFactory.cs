using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FortNitta.Network
{
  public class MessageFactory
  {

    public MessageFactory()
    {

    }
    public byte[] makeAuthorizationMessage(out int length, string username, string password)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.CLIENT_AUTH)
          .pushString(username)
          .pushString(password)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeGetAvailableRoomsMessage(out int length, string username)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.GET_AVAILABLE_ROOMS)
          .pushString(username)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeCreateRoomMessage(out int length, string username, string roomname, Int16 capacity)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.CREATE_ROOM)
          .pushShort(capacity)
          .pushString(username)
          .pushString(roomname)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeJoinRoomMessage(out int length, string username, string roomname)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.JOIN_ROOM)
          .pushString(username)
          .pushString(roomname)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeDisconnectMessage(out int length, string username)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.DISCONNECT)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeLeaveRoomMessage(out int length, string username, string roomname)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.LEAVE_ROOM)
          .pushString(username)
          .pushString(roomname)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeStartGameMessage(out int length, string username, string map)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.START_GAME)
          .pushString(username)
          .pushString(map)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeEndGameMessage(out int length, short player_id, short winner_id)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(Constants.END_GAME)
          .pushShort(player_id)
          .pushShort(winner_id)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }

    public byte[] makeGameActionMessage(out int length, Int16 action, Int32 timeStep, Int16 playerId, Int32 xPos, Int32 yPos)
    {
      MessageBuffer msg = new MessageBuffer();
      msg.pushInt(0)
          .pushShort(action)
          .pushInt(timeStep)
          .pushShort(playerId)
          .pushInt(xPos)
          .pushInt(yPos)
          .writeSize();
      length = msg.getPos();
      return msg.getData();
    }
  }
}
