//﻿#define TEST
using System.Collections.Generic;
using System;

namespace FortNitta.Network
{
  //Parent class for pre and post game interactions. 
  abstract public class Response
  {
    public int _result;
    public int _action;

    public Response(int result)
    {
      _result = result;
    } // Response
#if TEST
    abstract public void process();
#else
    abstract public void process(FortNitta game);
#endif
  } // Response

  public class LoginResponse : Response
  {
    public LoginResponse(int result)
      : base(result)
    {
      this._action = Constants.SERVER_AUTH_ACTION;
    } // LoginResponse

#if TEST
    public override void process()
    {      

    } // process
#else
    public override void process(FortNitta game)
    {
      if (_result == 1)
      {
        game.setMenu("roomMenu");
      }
    } // process
#endif
  } // LoginResponse

  public class RoomsAvailableResponse : Response
  {
    public List<Room> _rooms = null;

    public RoomsAvailableResponse(List<Room> rooms)
      : base(1)
    {
      this._action = Constants.GET_AVAILABLE_ROOMS;
      _rooms = rooms;
    } // RoomsAvailableResponse

#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      game.rooms.Clear();
      int i;
      for (i = 0; i < _rooms.Count; i++)
      {
        game.rooms.Add((_rooms[i])._name);
      }
    } // process
#endif
  } // RoomsAvailableResponse

  public class RoomCreatedResponse : Response
  {
    public string _roomName = null;

    public RoomCreatedResponse(int result, string roomName)
      : base(result)
    {
      this._action = Constants.SERVER_CREATE_ROOM_ACTION;
      _roomName = roomName;
    } // RoomsAvailableResponse

#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      if (_result == 1)
      {
        game.setMenu("newRoom");
      }//if succesfully room created
      else
      {
        //you stay in SelectRoom menu
      }
    } // process
#endif

  } // RoomsAvailableResponse

  public class RoomJoinedResponse : Response
  {
    public string _roomName = null;
    public List<string> _playerNames = null;
    public int _numPlayers = 0;

    public RoomJoinedResponse(string roomName, List<string> playerNames, int numPlayers)
      : base(1)
    {
      this._action = Constants.SERVER_ALLOW_JOIN_ROOM_ACTION;
      _roomName = roomName;
      _playerNames = playerNames;
      _numPlayers = numPlayers;
    } // RoomJoinedResonse

#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      int i;
      game.players.Clear();
      if (_result == 1)
      {
        for (i = 0; i < _numPlayers; i++)
        {
          game.players.Add(_playerNames[i]);
        }
        game.setMenu("joinRoom");
      }
      //go to new room
      //list players
    } // process
#endif

  } // RoomJoinedResponse

  public class RoomNotJoinedResponse : Response
  {
    public string _roomName = null;

    public RoomNotJoinedResponse(string roomName)
      : base(1)
    {
      this._action = Constants.SERVER_DENY_JOIN_ROOM_ACTION;
      _roomName = roomName;
    } // RoomNotJoinedResponse

#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      game.setMenu("roomMenu");
      //go back to lobby
    } // process
#endif
  } // RoomNotJoinedResponse

  public class PlayerJoinedRoomResponse : Response
  {
    public string _userName = null;
    public string _roomName = null;

    public PlayerJoinedRoomResponse(string userName, string roomName)
      : base(1)
    {
      this._action = Constants.SERVER_PLAYER_JOINED_ROOM_ACTION;
      _userName = userName;
      _roomName = roomName;
    } // PlayerJoinedRoomResponse

#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      game.players.Add(_userName);
    } // process
#endif

  } // PlayerJoinedRoomResponse

  public class PlayerLeftRoomResponse : Response
  {
    public string _userName = null;
    public string _roomName = null;

    public PlayerLeftRoomResponse(string userName, string roomName)
      : base(1)
    {
      this._action = Constants.SERVER_LEAVE_ROOM_ACTION;
      _userName = roomName;
      _roomName = roomName;
    } // PlayerLeftRoomResponse

#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      game.players.Remove(_userName);
    } // process
#endif
  } // PlayerLeftResponse

  public class OwnerLeftRoomResponse : Response
  {
    public string _userName = null;
    public string _roomName = null;

    public OwnerLeftRoomResponse(string userName, string roomName)
      : base(1)
    {
      this._action = Constants.SERVER_OWNER_LEFT_ROOM_ACTION;
      _userName = roomName;
      _roomName = roomName;
    } // OwnerLeftRoomResponse

#if TEST
    public override void process()
    {
    } // process
#else
    public override void process(FortNitta game)
    {
      game.rooms.Clear();
      game.players.Clear();
      game.setMenu("roomMenu");
    } // process
#endif
  } // OwnerLeftRoomResponse

  public class StartGameResponse : Response
  {
    public List<string> _playerNames;
    public string _map;

    public StartGameResponse(List<string> playerNames, string map)
      : base(1)
    {
      this._action = Constants.SERVER_START_GAME_ACTION;
      _playerNames = playerNames;
      _map = map;
    }
#if TEST
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      game.setMap(_map);
      game.startGame();
    } // process
#endif

  }

  public class EndGameResponse : Response
  {
    public int _winner_id;

    public EndGameResponse(int winner_id)
      : base(1)
    {
      this._action = Constants.END_GAME;
      _winner_id = winner_id;
    }

#if TEST 
    public override void process()
    {

    } // process
#else
    public override void process(FortNitta game)
    {
      // implement start game
    } // process
#endif
  }

  /// <summary>
  /// Parent class for in game interactions.
  /// </summary>
  public class PointerLocationResponse : Response
  {
    private int _time_step;
    protected int _player_id;
    protected int _x_coordinate;
    protected int _y_coordinate;
    public PointerLocationResponse(int time_step, int player_id, int x, int y)
      : base(1)
    {
      this._action = Constants.POINTER_LOCATION;
      _time_step = time_step;
      _player_id = player_id;
      _x_coordinate = x;
      _y_coordinate = y;
    }

#if TEST
    public override void process()
    {

    }
#else
    public override void process(FortNitta game)
    {
      int x = this._x_coordinate;
      int y = this._y_coordinate;
      int id = this._player_id - 1;
      game.cursorLocations[id] = new Microsoft.Xna.Framework.Point(x, y);
    }
#endif

  }

  public class SelectedCastleResponse : PointerLocationResponse
  {
    public SelectedCastleResponse(int time_step, int player_id, int x, int y)
      : base(time_step, player_id, x, y)
    {
      this._action = Constants.SELECTED_CASTLE;

    }

#if TEST
    public override void process()
    {

    }
#else
    public override void process(FortNitta game)
    {
      int x = this._x_coordinate;
      int y = this._y_coordinate;
      int id = this._player_id - 1;
      game.selectCastle(id, x, y);
    }
#endif
  }

  public class PlacedCannonResponse : PointerLocationResponse
  {
    public PlacedCannonResponse(int time_step, int player_id, int x, int y)
      : base(time_step, player_id, x, y)
    {
      this._action = Constants.PLACED_CANNON;

    }

#if TEST
    public override void process()
    {

    }
#else
    public override void process(FortNitta game)
    {
      int x = this._x_coordinate;
      int y = this._y_coordinate;
      int id = this._player_id - 1;
      game.placeCannon(id, x, y);
    }
#endif

  }

  public class FiredCannonResponse : PointerLocationResponse
  {
    public FiredCannonResponse(int time_step, int player_id, int x, int y)
      : base(time_step, player_id, x, y)
    {
      this._action = Constants.FIRED_CANNON;

    }
#if TEST
    public override void process()
    {

    }
#else
    public override void process(FortNitta game)
    {
      int x = this._x_coordinate;
      int y = this._y_coordinate;
      int id = this._player_id - 1;
      game.fireCannons(id, x, y);
    }
#endif
  }

  public class RotatedWallResponse : PointerLocationResponse
  {
    public RotatedWallResponse(int time_step, int player_id, int x, int y)
      : base(time_step, player_id, x, y)
    {
      this._action = Constants.ROTATED_WALL;
    }

#if TEST
    public override void process()
    {

    }
#else
    public override void process(FortNitta game)
    {
      int x = this._x_coordinate;
      int y = this._y_coordinate;
      int id = this._player_id - 1;
      game.rotateWalls(id);
    }
#endif
  }

  public class PlacedWallResponse : PointerLocationResponse
  {
    public PlacedWallResponse(int time_step, int player_id, int x, int y)
      : base(time_step, player_id, x, y)
    {
      this._action = Constants.PLACED_WALL;
    }

#if TEST
    public override void process()
    {

    }
#else
    public override void process(FortNitta game)
    {
      int x = this._x_coordinate;
      int y = this._y_coordinate;
      int id = this._player_id - 1;
      game.placeWalls(id);
    }
#endif
  }

}