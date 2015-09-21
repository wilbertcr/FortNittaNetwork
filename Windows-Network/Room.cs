using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FortNitta.Network
{
  public class Room
  {
    public string _name = null;
    public int _numPlayers = 0;

    public List<string> _playerNames = null;
    public bool _isOwnedByLocalUser = false;


    public Room()
    {
    } //Room

    public Room(string name, int numPlayers)
    {
      _name = name;
      _numPlayers = numPlayers;
    } //Room


    public string Name
    {
      get
      {
        return _name;
      }

      set
      {
        _name = value;
      }
    } // Name

    public int NumPlayers
    {
      get
      {
        return _numPlayers;
      }

      set
      {
        _numPlayers = value;
      }
    } //NumPlayers

    // TODO present better interface to Room class
  }
}
