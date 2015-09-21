using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;

namespace FortNitta.Network
{
  public class CReaderWriterLock
  {
    static ReaderWriterLock rwl = new ReaderWriterLock();
    static int readerTimeouts = 0;
    static int writerTimeouts = 0;
    static int resource = 0;
    static int reads = 0;
    static int writes = 0;
    static String msgout = "";
    static public StreamWriter NetworkLog = new StreamWriter("NetworkLog.txt", false);    
    // Shows how to request and release a reader lock, and 
    // how to handle time-outs. 

    public static String ReadFromResource(int timeOut,ref String msgout)
    {

      try
      {
        rwl.AcquireReaderLock(timeOut);
        try
        {
          // It is safe for this thread to read from 
          // the shared resource.
          
          //No reader needed.
        }
        finally
        {
          // Ensure that the lock is released.
          rwl.ReleaseReaderLock();          
        }
      }
      catch (ApplicationException)
      {
        return null;
      }
      return msgout;
    }

    // Shows how to request and release the writer lock, and 
    // how to handle time-outs. 
    public static int WriteToResource(int timeOut, String msg)
    {
      try
      {
        rwl.AcquireWriterLock(timeOut);
        try
        {
          // It is safe for this thread to read or write 
          // from the shared resource.
          NetworkLog.WriteLine(msg);
          NetworkLog.Flush();
        }
        finally
        {
          // Ensure that the lock is released.
          rwl.ReleaseWriterLock();
        }
      }
      catch (ApplicationException)
      {
        // The writer lock request timed out.
        return 0;
      }
      return msg.Length;
    }

    // Shows how to request a reader lock, upgrade the 
    // reader lock to the writer lock, and downgrade to a 
    // reader lock again. 
    static void UpgradeDowngrade(int timeOut)
    {
      try
      {
        rwl.AcquireReaderLock(timeOut);
        try
        {
          // It is safe for this thread to read from 
          // the shared resource.
          Display("reads resource value " + resource);
          Interlocked.Increment(ref reads);

          // If it is necessary to write to the resource, 
          // you must either release the reader lock and  
          // then request the writer lock, or upgrade the 
          // reader lock. Note that upgrading the reader lock 
          // puts the thread in the write queue, behind any 
          // other threads that might be waiting for the  
          // writer lock. 
          try
          {
            LockCookie lc = rwl.UpgradeToWriterLock(timeOut);
            try
            {
              // It is safe for this thread to read or write 
              // from the shared resource.
              
              Display("writes resource value " + resource);

            }
            finally
            {
              // Ensure that the lock is released.
              rwl.DowngradeFromWriterLock(ref lc);
            }
          }
          catch (ApplicationException)
          {
            // The upgrade request timed out.
            Interlocked.Increment(ref writerTimeouts);
          }

          // When the lock has been downgraded, it is  
          // still safe to read from the resource.
          Display("reads resource value " + resource);
          Interlocked.Increment(ref reads);
        }
        finally
        {
          // Ensure that the lock is released.
          rwl.ReleaseReaderLock();
        }
      }
      catch (ApplicationException)
      {
        // The reader lock request timed out.
        Interlocked.Increment(ref readerTimeouts);
      }
    }

    // Shows how to release all locks and later restore 
    // the lock state. Shows how to use sequence numbers 
    // to determine whether another thread has obtained 
    // a writer lock since this thread last accessed the 
    // resource. 
    static void ReleaseRestore(int timeOut)
    {
      int lastWriter;

      try
      {
        rwl.AcquireReaderLock(timeOut);
        try
        {
          // It is safe for this thread to read from 
          // the shared resource. Cache the value. (You 
          // might do this if reading the resource is 
          // an expensive operation.) 
          int resourceValue = resource;
          Display("reads resource value " + resourceValue);

          // Save the current writer sequence number.
          lastWriter = rwl.WriterSeqNum;

          // Release the lock, and save a cookie so the 
          // lock can be restored later.
          LockCookie lc = rwl.ReleaseLock();

          // Wait for a random interval (up to a  
          // quarter of a second), and then restore 
          // the previous state of the lock. Note that 
          // there is no time-out on the Restore method.

          rwl.RestoreLock(ref lc);

          // Check whether other threads obtained the 
          // writer lock in the interval. If not, then 
          // the cached value of the resource is still 
          // valid. 
          if (rwl.AnyWritersSince(lastWriter))
          {
            Display("resource has changed ");
          }
          else
          {
            Display("resource has not changed ");
          }
        }
        finally
        {
          // Ensure that the lock is released.
          rwl.ReleaseReaderLock();
        }
      }
      catch (ApplicationException)
      {
        // The reader lock request timed out.
        Interlocked.Increment(ref readerTimeouts);
      }
    }

    static void Display(string msg)
    {
      Console.Write(msg, Thread.CurrentThread.Name, msg);
    }
  }
}
