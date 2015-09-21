//#define DEBUG
//#define TEST

using System;
using System.Collections.Concurrent;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;
using System.Timers;

namespace FortNitta.Network
{

  // compile with: /doc:Network.xml
  /// <summary>
  /// This is the main network class. It provides an API for clients to 
  /// send messages to the server and to process incoming messages.
  /// </summary>

  public class Network
  {




    /*** data members **/

    static private Network network = null;
    static private int _instance = 0;
    private int _this_instance = 0;
    // Socket Info
    private string _host = null;
    private int _port = 0;
    private Socket _socket = null;
    private NetworkStream _socket_stream;

    // Multithreading Info
    public Mutex _mutexIn = null;
    public Mutex _mutexOut = null;
    public Thread _networkThread = null;
    CReaderWriterLock padlock = new CReaderWriterLock();
    // User / Application Info
    public string _username = "";
    public volatile bool DConnected = false;
    public ConcurrentQueue<Response> _queueIn = null;
    public ConcurrentQueue<Message> _queueOut = null;
    public int _loopcount;
    String msg;
    /*** Getters and Setters (Properties) ***/

    /// <summary>
    /// Host Name. It is set by default to "optical.cs.ucdavis.edu" but can be set if needed.
    /// </summary>
    public string Host
    {
      get
      {
        return _host;
      }
      set
      {
        _host = value;
      }
    }

    /// <summary>
    /// This value specifies the port number to which the client will attempt to connect.
    /// </summary>
    public int Port
    {
      get
      {
        return _port;
      }
      set
      {
        _port = value;
      }
    }

    /*** Constructors and Initializers ***/

    /// <summary>
    /// We're using the singleton pattern in order to prevent clients from instantiating multiple instances 
    /// of the Network class, since only 1 is needed and having more than one can break things. 
    /// </summary>
    /// <returns>An instance of the Network Class</returns>
    static public Network getNetwork()
    {
      /*network = new Network();
      if (network == null)
      {
        network = new Network();
        #if DEBUG
          NetworkLog.AutoFlush = true;
          msg = String.Format("Created Network instance.");          
          CReaderWriterLock.WriteToResource(25, msg);
        #endif
      }*/      
      return new Network();
    }

    private Network()
    {
      // Init Socket Info
      _host = "optical.cs.ucdavis.edu";
      _port = 49999;
      _socket = null;

      // Init Multithreading Info
      _mutexIn = new Mutex();
      _mutexOut = new Mutex();
      _networkThread = null;

      // Init User / Application Info
      _username = "";
      _queueIn = new ConcurrentQueue<Response>();
      _queueOut = new ConcurrentQueue<Message>();
      _this_instance = _instance++;
    }

    private bool ConnectToServer()
    {
      IPHostEntry IP_list;
      try
      {
        IP_list = Dns.GetHostEntry(_host);
        #if DEBUG
        msg = String.Format("Resolving host {0}", _host);
        CReaderWriterLock.WriteToResource(25, msg);
        #endif
        if (IP_list.AddressList.Length <= 0)
        {
          #if DEBUG
          msg = String.Format("Empty list of IP addresses received, quitting.");
          CReaderWriterLock.WriteToResource(25, msg);
          #endif
          return false;
        }
        foreach (IPAddress IP in IP_list.AddressList)
        {

          _socket = new Socket(IPAddress.Parse(IP.ToString()).AddressFamily, SocketType.Stream, ProtocolType.Tcp);
          try
          {
            _socket.Connect(IP, _port);
          }
          catch (Exception e)
          {
            #if DEBUG
            msg = String.Format("Exception trying IP {0}.", e.Message);
            CReaderWriterLock.WriteToResource(25, msg);
            #endif
          }
        }
      }
      catch (Exception e)
      {
        msg = String.Format("Exception resolving remote host name {0}.", e.Message);
        CReaderWriterLock.WriteToResource(25, msg);
        return false;
      }

      if (_socket != null && _socket.Connected)
      {
        _socket.NoDelay = true;
        _socket.Blocking = true;
        #if DEBUG
        msg = String.Format("Socket connected successfully.");
        CReaderWriterLock.WriteToResource(25, msg);
        #endif
        _socket_stream = new NetworkStream(_socket, true);      
        return true;
      }
      return false;

    }



    /*** Control Methods ***/

    /// <summary>
    /// This is the main loop of the network thread. 
    /// Here's where all sockets interactions occur.
    /// </summary>
    /// 

    private void Execute()
    {
      DConnected = ConnectToServer();
      if (!DConnected)
      {
        #if DEBUG
        msg = String.Format("Failed to connect to server.");
        CReaderWriterLock.WriteToResource(25, msg);
        #endif
        return;
      }
      
      int _out_time_step = 0;
      int _in_time_step = 0;
      bool msgpending = false;

      while (DConnected)
      {
        try
        {
          while (_queueOut.Count() > 0 && DConnected)
          {
            Message message = null;
            if (!msgpending && !_queueOut.TryDequeue(out message))
            {
              //We were unable to peek into the queue for some unkown reason.
              //It's not safe to continue as we haven't got a message to send out. 
              //Let's break from the sending operation and go try and receive some 
              //packets.
              #if DEBUG
              msg = String.Format("Unable to get item from outqueue.");
              CReaderWriterLock.WriteToResource(25, msg);
              #endif
              break;
            }
            else
            {
              #if DEBUG
              msg = String.Format("Successfully dequeued message:{0}", message.GetType().ToString());
              CReaderWriterLock.WriteToResource(25, msg);
              #endif
            }

            if (_socket.Poll(25000, SelectMode.SelectWrite))
            //if(_socket_stream.CanWrite)
            {
              try
              {
                _socket.Send(message.GetMessage(), message.GetMessageLength(), SocketFlags.None);
                //_socket_stream.Write(message.GetMessage(),0,message.GetMessageLength());
                msgpending = false;
                #if DEBUG
                msg = String.Format("{0} Sent message ts: {1} out.",_this_instance, _out_time_step++);
                CReaderWriterLock.WriteToResource(25, msg);
                #endif
              }
              catch (SocketException e)
              {
                CReaderWriterLock.WriteToResource(25, "Tx: Socket exception on write.");
                if (!_socket.Connected)
                {
                  DConnected = ConnectToServer();
                  msg = String.Format("Error code: {0}",e.ErrorCode.ToString());
                  CReaderWriterLock.WriteToResource(25, msg);
                  msg = String.Format(e.Message);
                  CReaderWriterLock.WriteToResource(25, msg);
                  msg = String.Format(e.StackTrace.ToString());
                  CReaderWriterLock.WriteToResource(25, msg);
                  msgpending = true;
                }
              }
              catch (ObjectDisposedException e)
              {
                CReaderWriterLock.WriteToResource(25, "Tx: Socket has been closed by remote side.");
                DConnected = false;
                break;
              }
              catch (ArgumentOutOfRangeException e)
              {
                CReaderWriterLock.WriteToResource(25, "Tx: Incorrect size given to socket write.");
                DConnected = false;
                break;
              }
            }
          }

          //processing incoming Queue;
          while (DConnected)
          {
            try
            {
              //Thread.Sleep(50);
              if (_socket.Poll(25000, SelectMode.SelectRead))
              //if (_socket_stream.CanRead && _socket_stream.DataAvailable)
              {
                byte[] packet = new byte[Constants.MAX_MESSAGE_SIZE];
                int networksize = -1;
                int msgsize = -1;
                _socket.Receive(packet, 4, 0);
                //_socket_stream.Read(packet, 0, sizeof(int));

                networksize = BitConverter.ToInt32(packet, 0);
                msgsize = IPAddress.NetworkToHostOrder(networksize);
                #if DEBUG
                msg = String.Format("Msg size: {0}", msgsize);
                CReaderWriterLock.WriteToResource(25, msg);
                #endif
                if (msgsize > sizeof(int))
                {
                  _socket.Receive(packet, sizeof(int), msgsize - sizeof(int), 0);
                }
                else
                {
                  DConnected = false;
                  break;
                }
                //_socket_stream.Read(packet, sizeof(int), msgsize - sizeof(int));

                //Here we call the class in charge of parsing and placing 
                //response messages on the queue going to the client.
                //This class will have to implement mutex when
                //placing items on the queue.
                //Errors in receiving from the socket will transfer control to
                //the catch and will change the value of _socket.Connected.
                //This way, we make sure not to loop infinitely if the 
                //server die on us.
                Response _newmessage = MessageParser.parsePacket(packet);
                _queueIn.Enqueue(_newmessage);
                #if DEBUG
                msg = String.Format("{0} Pushed message into incoming queue: {1}", _this_instance, _in_time_step++);
                CReaderWriterLock.WriteToResource(25, msg);
                #endif
              }
              else
              {
                break;
              }
            }
            catch (SocketException e)
            {
              CReaderWriterLock.WriteToResource(25, "Rx: Socket exception.");

              /*
              if (!_socket.Connected)
              {
                DConnected = ConnectToServer();
                msg = String.Format("Error code: {0}", e.ErrorCode.ToString());
                CReaderWriterLock.WriteToResource(25, msg);
                writeLog(e);
                msgpending = true;
              }
              */
            }
            catch (ObjectDisposedException e)
            {
              CReaderWriterLock.WriteToResource(25, "Rx: Socket has been closed by remote side.");              
              writeLog(e);
              DConnected = false;
              break;
            }
            catch (ArgumentOutOfRangeException e)
            {
              CReaderWriterLock.WriteToResource(25, "Rx: Incorrect length given");
              writeLog(e);
              DConnected = false;
              break;
            }
            catch (ArgumentNullException e)
            {
              CReaderWriterLock.WriteToResource(25, "Rx: Buffer is null for some reason.");
              writeLog(e);
              DConnected = false;
            }
          }
        }
        catch (ThreadAbortException e)
        {
          CReaderWriterLock.WriteToResource(25, "ThreadAbort exception.");
          writeLog(e);
          DConnected = false;
        }
        catch (Exception ex)
        {
          CReaderWriterLock.WriteToResource(25, "General exception.");
          writeLog(ex);
          DConnected = false;
        }

      }//Main loop.
      CReaderWriterLock.WriteToResource(25, "We have been disconnected. Thread dying.");
    }//Execute

    private void writeLog(Exception ex){
      msg = String.Format(ex.GetBaseException().GetType().ToString());
      CReaderWriterLock.WriteToResource(25, msg);
      msg = String.Format(ex.GetType().ToString());
      CReaderWriterLock.WriteToResource(25, msg);
      msg = String.Format(ex.Message);
      CReaderWriterLock.WriteToResource(25, msg);
      msg = String.Format(ex.StackTrace.ToString());
      CReaderWriterLock.WriteToResource(25, msg);
    }

    public bool Status()
    {
      return DConnected;
    }

    /// <summary>
    /// Calling this function attempts to resolve the host to its list of IP addresses and 
    /// tries to connect. Many things can go wrong here so several different exceptions 
    /// can be thrown here. 
    /// <exception cref="ArgumentNullException">The HostName is null</exception>
    /// <exception cref="ArgumentOutOfRangeException">The HostName provided is over 255 characters long or the port number is invalid. 
    /// No need to deal with this exception if the HostName field is limited to less than 255 characters.</exception>
    /// <exception cref="SocketException">An error was encountered when resolving the hostName parameter.</exception>
    /// </summary>
    public bool Connect()
    {
      //We don't want multiple calls to 
      //this function to start triggering 
      //multiple threads. 
      //We're disconnected and the network thread has been initialized already.
      //Let's flush the queues.
      Message message;
      while (_queueOut.TryDequeue(out message)) ;
      Response response;
      while (_queueIn.TryDequeue(out response)) ;

      if (!Status() && (_networkThread != null))
      {
        #if DEBUG
        msg = String.Format("Socket is not connected and networkthread is not null.");
        CReaderWriterLock.WriteToResource(25, msg);
        #endif
        //We need to make sure the thread is either stopped or suspended.
        if ((_networkThread.ThreadState & (ThreadState.Stopped | ThreadState.Suspended)) == 0)
        {
          _networkThread.Abort();
          #if DEBUG
          msg = String.Format("Network thread is stopped, creating new one.");
          CReaderWriterLock.WriteToResource(25, msg);
          #endif
          _networkThread = new Thread(Execute);
          _networkThread.Start();
        }
      }
      if (_networkThread == null)
      {
        #if DEBUG
        msg = String.Format("Network thread is null, creating new one.");
        CReaderWriterLock.WriteToResource(25, msg);
        #endif
        _networkThread = new Thread(Execute);
        _networkThread.Start();
      }
      #if DEBUG
      msg = String.Format("We good... may be?.");
      CReaderWriterLock.WriteToResource(25, msg);
      #endif
      Thread.Sleep(100);
      return Status();
    }//Connect

    public void Disconnect()
    {
      if (Status())
      {
        if (_mutexOut.WaitOne(500))
        {
          //TODO Commented out until CDisconnectMessage class is implemented.
          _queueOut.Enqueue(new DisconnectMessage(_username));
          //Let's allow the Network process any pending outgoing messages.
          Thread.Sleep(1000);
        }
      }
      if (_socket != null)
        _socket.Close(500);
    }//Disconnect

    public void LogIn(string username, string password)
    {
      _username = username;
      _queueOut.Enqueue(new AuthorizationMessage(username, password));
#if DEBUG
      msg = String.Format("Loging in. Pushed Authorization Message containing.");
      CReaderWriterLock.WriteToResource(25, msg);
      msg = String.Format("Username: {0}", username);
      CReaderWriterLock.WriteToResource(25, msg);
      msg = String.Format("Password: {0}", password);
      CReaderWriterLock.WriteToResource(25, msg);
#endif
    }//Login

    public void GetAvailableRooms()
    {
      _queueOut.Enqueue(new GetAvailableRoomsMessage(_username));
    }//GetAvailableRooms

    public void CreateRoom(int capacity, string room_name)
    {
      _queueOut.Enqueue(new CreateRoomMessage(capacity, _username, room_name));
    }//CreateRoom

    public void JoinRoom(string room_name)
    {
      _queueOut.Enqueue(new JoinRoomMessage(_username, room_name));
    }//JoinRoom

    public void LeaveRoom(string room_name)
    {
      _queueOut.Enqueue(new LeaveRoomMessage(_username, room_name));
    }//LeaveRoom

    public void StartGame(string map_data)
    {
      _queueOut.Enqueue(new StartGameMessage(_username, map_data));
    }//StartGame

    public void EndGame(short playerid, short winnerid)
    {
      _queueOut.Enqueue(new EndGameMessage(playerid, winnerid));
    }//StartGame

    public void InGameAction(Int16 action, Int32 time_step, Int16 player_id, Int32 x_coordinate, Int32 y_coordinate)
    {
      _queueOut.Enqueue(new ActionPacketMessage(action, time_step, player_id, x_coordinate, y_coordinate));
    }//InGameAction

    //Modify signature so as to pass the game reference here.
#if TEST
    public void Update()
    {
      Response response;
      while (_queueIn.Count > 0)
      {
        if (_queueIn.TryDequeue(out response))
        {
          //Now yoo have access to the game pointer inside every
          //process function. 
          response.process();
        }
      }
    }//Update
#else
    public void Update(FortNitta game)
    {
      Response response;
      while (_queueIn.Count > 0)
      {
        if (_queueIn.TryDequeue(out response))
        {
          //Now yoo have access to the game pointer inside every
          //process function. 
          response.process(game);
          #if DEBUG
          msg = String.Format("Processed message from incoming queue.");
          #endif
        }
      }
    }//Update
#endif
  }//Network

}//FortNitta.Network
