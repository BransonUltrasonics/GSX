/**********************************************************************************************************

      Copyright (c) Branson Ultrasonics Corporation, 1996-2018
 
     This program is the property of Branson Ultrasonics Corporation
     Copying of this software is expressly forbidden, without the prior
     written consent of Branson Ultrasonics Corporation.


---------------------------- MODULE DESCRIPTION ----------------------------

     Socket related functionality implementation for interface with QT and business logic
 
**********************************************************************************************************/

#ifdef _WIN32
    //#include "C:\Users\ee209300\Downloads\wil4c311\winsock2.h"
   #include <winsock2.h>
//   #include  <ws2tcpip.h>
#endif

#include "SasSocket.h"

// Set the flag value to false at first time calling
bool SasSocket::bStaticWSAStartupSocket = false;  


/*
** The socket identifier is initialized.
*/
SasSocket::SasSocket()
{

//Initialize the socket version as 1.1
#ifdef _WIN32
if(!bStaticWSAStartupSocket)
{
	
	WORD	wVersionRequested;
	WSADATA wsaData;
 
	wVersionRequested = MAKEWORD( 1, 1 );
 
 	if (!WSAStartup( wVersionRequested, &wsaData ))
	{
		// Startup succ
		bStaticWSAStartupSocket = true;

	}//if
}
#endif
	// Clear out the internal socket fields

	dwsock = -1;
	bIsServer = false;
}


/*
** Here the socket is closed.
*/
SasSocket::~SasSocket()
{
  // Close the socket
  Close();
}


/*
** This method creates a new Socket.It also sets the option to the socket
**  such that the socket may listen at the same port no.
*/
bool SasSocket::Create()
{
	const int one = 1;
	// Check that the old socket was closed
	if (dwsock >= 0)
		Close();

	#ifdef _WIN32

		// Create the socket
		if ((dwsock = socket(AF_INET, SOCK_STREAM, 0))==INVALID_SOCKET )
		{
	
			WriteLog("Cannot create the socket ");
			return false;
		}
	#else
		if ((dwsock = socket(AF_INET, SOCK_STREAM, 0))<0 )
		{
	
			WriteLog("\nCannot create the socket ");
			return false;
		}
	#endif

		 
	if (setsockopt(dwsock, SOL_SOCKET, SO_REUSEADDR, (char *)&one, sizeof(one)))
    {
		return false;
    }
    return true;
	
}//Create


/*
**  This method closes the the socket. The shutdown() stops the communication.
*/

bool SasSocket::Close()
{
	int dwResult;
	char buff[1000];	// To read data
	int currlen = 1000;
	int dwRetBuffsize;

	if (dwsock >= 0)
	{
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;

		while(1)
		{
			if((dwResult = select(dwsock + 1,&ReadFdset,(fd_set *)0,(fd_set *)0,&timeout)) <= 0)
				break;
			
#ifdef _WIN32
			// Before going to read we have to Sleep for 1 Second
			Sleep(1000);
#else
			// For Linux, it means 1 Second
			sleep(1);
#endif
			// If we will not read the buffer from socket before closing, it goes to
			// TIME_WAIT state. For that we are Reading here.
			dwRetBuffsize = Read(buff, currlen);
			
			if(dwRetBuffsize <= 0)
				break;
			
		}

#ifdef _WIN32
		closesocket(dwsock);//WIN32 method
#else
		close(dwsock); //Unix method
#endif
		dwsock = -1;
	}
    return true;
}



/*
**  This method checks whether the port no is free or not
**  This is not a public method
*/
bool SasSocket::IsFree(const unsigned int port)
{
	SasSocket dummy;
	if (dummy.Create())
	{
		// If we're not able to connect then the port number is in use...
		if (dummy.Connect("localhost", port,2))
		{
			WriteLog("The port no is in use");
		}
		else
		{
			return true;
		}
	}

	WriteLog("Socket can not be created");
	return false;

}//IsFree


/*
** This method binds the socket to the specified port no.
*/
bool SasSocket::Bind(const unsigned int port)
{
	bIsServer = true;

	struct sockaddr_in addr;

	// Check that the socket is open
	if (dwsock < 0)
	{
		WriteLog("The socket is not yet created");
		return false;
	}

	// If a specific port is being set then check it's not already used!
	if (port != 0)
	{
	   if(IsFree(port) == false)
		  return false;
	}

	// Set up the address to bind the socket to
	addr.sin_family		 = AF_INET;
	addr.sin_port		 = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	#ifdef _WIN32
		// And do the binding
		if (bind(dwsock, (struct sockaddr *)&addr, sizeof(addr)) == SOCKET_ERROR)
		{
			WriteLog("Cannot bind to the port no ");
			return false;
		}
	#else
		if (bind(dwsock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
			WriteLog("Cannot bind to the port no ");
			return false;
		}
	#endif

	return true;
}//Bind


/* 
** This method is used to connect with timeout to the specified address and port no.
** If Timeout is not specified then the default timeout will be 60 seconds. 
*/
bool SasSocket::Connect(const char * address, const unsigned int port,const int dwTimeOutInSec)
{
  // Check the socket
  if (dwsock < 0)
    return false;

  // Create an address structure and clear it
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));

  // Fill in the address if possible
  addr.sin_family		= AF_INET;
  addr.sin_addr.s_addr  = inet_addr(address);

#ifdef _WIN32
  // Was the string a valid IP address?
  if (addr.sin_addr.s_addr == -1)
    {
      // No, so get the actual IP address of the host name specified
      struct hostent *pHost;
      pHost = gethostbyname(address);
      if (pHost != NULL)
	  {
		  if (pHost->h_addr == NULL)
			  return false;
		  addr.sin_addr.s_addr = ((struct in_addr *)pHost->h_addr)->s_addr;
	  }
	  else
	    return false;
    }
#endif


    // Set the port number in the correct format
	addr.sin_port = htons(port);

#ifdef _WIN32
	unsigned long NonBlocking = 1;
	if(ioctlsocket(dwsock, FIONBIO, &NonBlocking))
		return false;
#else
	long		dwOrigFlag, dwFlag;
	
	// Read the file descriptor flags
	if( (dwOrigFlag = fcntl(dwsock, F_GETFL, NULL)) < 0) 
		return false;

	// Add blocking mode to the existing flags
	dwFlag = dwOrigFlag & ~O_NONBLOCK; 
	
	// Save the file descriptor flags
	if( fcntl(dwsock, F_SETFL, dwFlag) < 0) 
		return false; 

#endif
	
	// Connect to the server
	int dwRet = connect (dwsock, (struct sockaddr*)&addr, sizeof(struct sockaddr));

	if (dwRet < 0 )
	{
		// If the connection is still in progress
#ifdef _WIN32
		errno = WSAGetLastError();
		if (errno == WSAEWOULDBLOCK) 
#else
		if (errno == EINPROGRESS) 
#endif
		{ 
			fd_set		myset; 
			struct		timeval timeout; 
			
#ifdef _WIN32
			char		valopt; 
			int			lon; 
#else
			int			valopt; 
			socklen_t	lon; 
#endif

			// Specify the time to wait before getting timeout.
			timeout.tv_sec	= dwTimeOutInSec; 
			timeout.tv_usec = 0; 
			FD_ZERO(&myset); 
			FD_SET(dwsock, &myset); 

			// Blocked here until connection is established or time out occurs.
			if (select(dwsock+1, NULL, &myset, NULL, &timeout) > 0) 
			{ 
				lon = sizeof(int); 
				getsockopt(dwsock, SOL_SOCKET, SO_ERROR, &valopt, &lon);
				// Get the Socket error status .
				if (valopt) 
					return false;
			} 
			else 
			{ 
				// Wait time expired.
				return false;
			} 
		} 
		else 
		{ 
			// Connect failed.
			return false;
		} 
	}

#ifdef _WIN32
	NonBlocking = 0;
	if(ioctlsocket(dwsock, FIONBIO, &NonBlocking))
#else
	if( fcntl(dwsock, F_SETFL, dwOrigFlag) < 0) 
#endif
	{
		Close();
		return false;
	}
    return true;
}//Connect


/*
**  This method listens for connections at the binded port no.
*/

bool SasSocket::Listen()
{
	// Check socket
	if (dwsock < 0)
		return false;

	// The number is the size of the waiting que
	if (listen(dwsock, 10) < 0)
		return false;

	return true;
}


/*
**  This method accepts the incoming connections. This is the method where
**	the process blocks after listen() for incoming connections.
*/
int SasSocket::Accept()
{
	int new_socket_id;
	int dwLen;
	dwLen = sizeof(msockaddr_in);
	

	// Check this socket
	if (dwsock < 0)
	{
		return NULL;
	}

	#ifdef _WIN32
		// Accept an incoming connection
		if ((new_socket_id = accept(dwsock, (struct sockaddr *)&msockaddr_in,&dwLen)) == INVALID_SOCKET)
		{
			long dwErrorCode = WSAGetLastError ();
			char		szTemp[512];
			
			sprintf(szTemp, "ErrorCode:%d in Accept",dwErrorCode);

			return NULL;

		}
	#else
		if ((new_socket_id = accept(dwsock,(struct sockaddr *)&msockaddr_in,(socklen_t *)&dwLen)) < 0)
		{
			WriteLog("\nAccept Error");
			return NULL;
		}
	#endif

	return new_socket_id;
}


/*
** This method is used to send the information between the connected sockets
*/
int SasSocket::Send(const char *buff, const unsigned int bufflen)
{
	long	dwReturnCode;
	dwReturnCode = send(dwsock, buff, bufflen, 0);
	if(dwReturnCode < 0)
	{
#ifdef _WIN32
		errno = WSAGetLastError ();
#endif
		printf("\nSasSocket:Send: errno: %s\n", strerror (errno));
		return -1;

	}

	return dwReturnCode;
}


/*
** This method is used to get the information from the connected sockets.
** recv() blocks till a message comes from the specified socket.
*/
int SasSocket::Read(char *buff, const unsigned int bufflen)
{

	int  len=0;
	len =recv(dwsock, buff, bufflen, 0);

	if(len < 0)
	{
#ifdef _WIN32
		errno=WSAGetLastError();
#endif
		printf("recv : The Error code = %d,%d\n",errno,len);
	}
	return len;
}


/*
**  This method exactly reads the data from the connected socket.
*/
int SasSocket::ReadExact(char *buff, const unsigned int bufflen)
{
	int n;
	unsigned int currlen = bufflen;
	int dwResult;
	
	while (currlen > 0)
	{
		if(bIsServer)
		{
			timeout.tv_sec  = MAX_READ_WAIT_TIME;
		}
		else
		{
			timeout.tv_sec  = MAX_READ_WAIT_TIME_CLIENT;
		}

		timeout.tv_usec = 0;
		FD_ZERO(&ReadFdset);
		FD_SET(dwsock,&ReadFdset);

		if((dwResult = select(dwsock + 1,&ReadFdset,(fd_set *)0,(fd_set *)0,&timeout)) < 0)
		{
			WriteLog("Error in Select\n");
			return -1;
		}//if

			
		if(dwResult == 0)
		{
			WriteLog("Time out.\n");
			return 0;
		}//if

		// Try to read some data in
		n = Read(buff, currlen);

		if (n > 0)
		{
			// Adjust the buffer position and size
			buff += n;
			currlen -= n;
		} 
		else if (n == 0) 
		{
           if(dwResult)
                return -1; // If Select was success and Read was fail then the socket was Closed.
			// Log has to be written
			// WriteLog("Zero bytes read");
			return 0;
		} 
		else 
		{
		   if(dwResult)
				return -1; // If Select was success and Read was fail then the socket was Closed.
		#ifdef _WIN32
			if (errno != WSAEWOULDBLOCK)
		#else
			if(errno != EAGAIN)
		#endif
			{
				char   szError[512];

				sprintf(szError,"Error %d In Read Exact", errno);

				//WriteLog(szError);
				return -2;
			}
		}
    }

	return bufflen;
}//ReadExact


//This Method is used to get IP address of the client who is connected to the server
void SasSocket::GetIPAddress(char *pszIPAddress)
{
	strcpy(pszIPAddress,inet_ntoa(msockaddr_in.sin_addr));
}



