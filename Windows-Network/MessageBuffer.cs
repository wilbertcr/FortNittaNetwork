using System;
using System.Text;
using System.IO;
using System.Net;


namespace FortNitta.Network
{
  public class MessageBuffer
  {
    private int _pos = 0;
    private int _size = 0;
    private byte[] _buffer;
    static private char delim = ',';


    /*** Constructors ***/

    public MessageBuffer(byte[] buffer)
    {
      _buffer = buffer;
      _size = buffer.Length;
    }

    public MessageBuffer(int bufferSize = Constants.DEFAULT_MESSAGE_BUFFER_SIZE)
    {
      _buffer = new byte[bufferSize];
    }



    /*** Getters and Setters ***/

    public byte[] getData()
    {
      return (byte[])_buffer.Clone();
    } // getData

    public int getPos()
    {
      return _pos;
    } // getPos

    public int getSize()
    {
      return _size;
    } // getSize

    public int bytesLeft()
    {
      return _pos > _size ? 0 : _size - _pos;
    } // getBytesLeft



    /*** Control methods ***/

    public int seek(int nbytes)
    {
      int tempPos = _pos + nbytes;
      tempPos = tempPos < 0 ? 0 : tempPos;
      tempPos = tempPos >= _size ? _size - 1 : tempPos;
      _pos = tempPos;
      return _pos;
    } // seek

    public void clear()
    {
      _pos = 0;
      _size = 0;
    } // clear

    public void reset()
    {
      _pos = 0;
    } // clear

    public void resize(int size)
    {
      byte[] temp;

      // if size requested is larger than bufferSize must reallocate buffer
      if (size > _buffer.Length)
      {
        temp = _buffer;

        // allocate twice as much as requested, memory is cheap
        _buffer = new byte[2 * size];

        // copy old buffer into new buffer
        Array.Copy(temp, _buffer, temp.Length);
      } // buffer is too large

      // set the size as requested
      _size = size;
      _pos = _size > _pos ? _pos : _size;
    } // resize

    private void ensure(int size)
    {
      if (_size < size)
        resize(size);
    }



    /*** Push methods ***/

    public MessageBuffer pushShort(short value)
    {
      // Convert integral value to network order and get bytes
      short valueNetworkOrder = IPAddress.HostToNetworkOrder(value);
      byte[] preparedBytes = BitConverter.GetBytes(valueNetworkOrder);

      // Ensure message size has room and copy preparedBytes
      ensure(_pos + preparedBytes.Length);
      Array.Copy(preparedBytes, 0, _buffer, _pos, preparedBytes.Length);
      _pos += preparedBytes.Length;
      return this;
    } // pushShort

    public MessageBuffer pushInt(int value)
    {
      // Convert integral value to network order and get bytes
      int valueNetworkOrder = IPAddress.HostToNetworkOrder(value);
      byte[] preparedBytes = BitConverter.GetBytes(valueNetworkOrder);

      // Ensure message size has room and copy preparedBytes
      ensure(_pos + preparedBytes.Length);
      Array.Copy(preparedBytes, 0, _buffer, _pos, preparedBytes.Length);
      _pos += preparedBytes.Length;
      return this;
    } // pushInt

    public MessageBuffer pushString(string value)
    {
      // Gets 8-bit encoding corresponding to ISO 8859-1
      //  (ASCII is 7-bit and UTF-8 is variable length)
      Encoding enc = Encoding.GetEncoding(Constants.NETWORK_ENCODING);
      byte[] preparedBytes = enc.GetBytes(value + delim);

      // Ensure message size has room and copy preparedBytes
      ensure(_pos + preparedBytes.Length);
      Array.Copy(preparedBytes, 0, _buffer, _pos, preparedBytes.Length);
      _pos = _pos + preparedBytes.Length;
      return this;
    } // pushString

    public void writeSize()
    {
      // Convert integral value to network order and get bytes
      int valueNetworkOrder = IPAddress.HostToNetworkOrder(_size);
      byte[] preparedBytes = BitConverter.GetBytes(valueNetworkOrder);

      // Ensure message size has room and copy preparedBytes
      ensure(_pos + preparedBytes.Length);
      Array.Copy(preparedBytes, _buffer, preparedBytes.Length);
      if (_pos == 0)
      {
        _pos += preparedBytes.Length;
      }
      
    } // writeSize



    /*** Pop methods ***/

    public short popShort()
    {
      if (bytesLeft() < sizeof(short))
        return -1; // should we throw an exception?

      short valueNetworkOrder = BitConverter.ToInt16(_buffer, _pos);
      short value = IPAddress.NetworkToHostOrder(valueNetworkOrder);
      _pos += sizeof(short);
      return value;
    } // popShort

    public int popInt()
    {
      if (bytesLeft() < sizeof(int))
        return -1; // should we throw an exception?

      int valueNetworkOrder = BitConverter.ToInt32(_buffer, _pos);
      int value = IPAddress.NetworkToHostOrder(valueNetworkOrder);
      _pos += sizeof(int);
      return value;
    } // popInt

    public string popString()
    {
      if (bytesLeft() == 0)
        return "";

      Encoding enc = Encoding.GetEncoding(Constants.NETWORK_ENCODING);
      int index = Array.IndexOf<byte>(_buffer, (byte)delim, _pos);
      int end = index == -1 ? _size : index;
      int length = end - _pos;

      string value = enc.GetString(_buffer, _pos, length);
      _pos += length + 1;
      return value;
    } // popString

  }

}
