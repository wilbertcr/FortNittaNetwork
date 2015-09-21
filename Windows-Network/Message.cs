using System;


namespace FortNitta.Network
{

  abstract public class Message {

    public Message() {

    }

    abstract public void MakeMessage();
    abstract public byte[] GetMessage();
    abstract public int GetMessageLength();
  }

  public class AuthorizationMessage : Message {
    private string _username;
    private string _password;
    private byte[] _message;
    private int _length;

    public AuthorizationMessage(string username, string password) {
      _username = username;
      _password = password;
      _message = new byte[Constants.MAX_MESSAGE_SIZE];
      _length = 0;
      MakeMessage();
    }

    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeAuthorizationMessage(out _length, _username, _password);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }
  } // Authorization message.


  public class GetAvailableRoomsMessage : Message {

    private string _username;
    private byte[] _message;
    private int _length;

    public GetAvailableRoomsMessage(string username) {
      _username = username;
      _message = new byte[Constants.MAX_MESSAGE_SIZE];
      _length = 0;
      MakeMessage();
    }


    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeGetAvailableRoomsMessage(out _length, _username);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }

  } //GetAvailableRooms


  public class CreateRoomMessage : Message {
    string _username;
    string _room_name;
    int _capacity;
    byte[] _message;
    int _length;

    public CreateRoomMessage(int capacity, string username, string room_name) {
      _username = username;
      _room_name = room_name;
      _capacity = capacity;
      _message = new byte[Constants.MAX_MESSAGE_SIZE];
      _length = 0;
      MakeMessage();
    }

    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeCreateRoomMessage(out _length, _username, _room_name, Convert.ToInt16(_capacity));
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }

  }//CreateRoomMessage


  public class JoinRoomMessage : Message {
    private string _username;
    private string _room_name;
    private byte[] _message;
    private int _length;

    public JoinRoomMessage(string username, string room_name) {
      _username = username;
      _room_name = room_name;
      _length = 0;
      MakeMessage();
    }

    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeJoinRoomMessage(out _length, _username, _room_name);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }

  }//JoinRoomMessage

  public class DisconnectMessage : Message {
    private byte[] _message;
    private int _length;
    private string _username;

    public DisconnectMessage(string username) {
      _length = 0;
      _username = username;
      MakeMessage();

    }
    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeDisconnectMessage(out _length, _username);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }
  }//DisconnectMessage

  public class LeaveRoomMessage : Message {
    private string _username;
    private string _room_name;
    private int _length;
    private byte[] _message;

    public LeaveRoomMessage(string username, string room_name) {
      _username = username;
      _room_name = room_name;
      _length = 0;
      MakeMessage();
    }

    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeLeaveRoomMessage(out _length, _username, _room_name);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }
  }//LeaveRoomMessage

  public class StartGameMessage : Message {
    private string _username;
    private string _map;
    private int _length;
    private byte[] _message;

    public StartGameMessage(string username, string map) {
      _username = username;
      _map = map;
      _length = 0;
      MakeMessage();
    }

    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeStartGameMessage(out _length, _username, _map);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }
  }//StartGameMessage


  public class EndGameMessage : Message
  {
    private short _player_id;
    private short _winner_id;
    private int _length;
    private byte[] _message;

    public EndGameMessage(short playerid, short winnerid)
    {
      _player_id = playerid;
      _winner_id = winnerid;
      MakeMessage();
    }

    public override void MakeMessage()
    {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeEndGameMessage(out _length, _player_id, _winner_id);
    }

    public override byte[] GetMessage()
    {
      return _message;
    }

    public override int GetMessageLength()
    {
      return _length;
    }
  }//StartGameMessage


  public class ActionPacketMessage : Message {
    private Int16 _action;
    private int _time_step;
    private Int16 _player_id;
    private int _x_coord;
    private int _y_coord;
    private byte[] _message;
    private int _length;

    public ActionPacketMessage(Int16 action, int time_step, Int16 player_id, int x_coord, int y_coord) {
      _action = action;
      _time_step = time_step;
      _player_id = player_id;
      _x_coord = x_coord;
      _y_coord = y_coord;
      MakeMessage();
    }

    public override void MakeMessage() {
      MessageFactory factory = new MessageFactory();
      _message = factory.makeGameActionMessage(out _length, _action, _time_step, _player_id, _x_coord, _y_coord);
    }

    public override byte[] GetMessage() {
      return _message;
    }

    public override int GetMessageLength() {
      return _length;
    }
  }
}//ActionPacketMessage