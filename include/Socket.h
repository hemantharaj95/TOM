/**
 * Contains header for Socket Class
 * This Communication Protocol is compatible with windows, mac and Linux. Becasue header contains both WINSocket and Berkely Socket
 */
 
//Header for WINSOCK
#if defined(_WIN32)
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

//Header for Berkeley Sockets
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

//Macro for Portability 

#if defined(_WIN32)
#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#else
#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)
#endif

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <random>
#include <fstream>


/**
 * @brief Base class for TCP communication protocol [TOM]. This class will provide Interface between Winsock or Berkeley socket programming.
 *        Contain all the functionality Interface necessary for Creating a Socket family [server or client or peer].
 */

class Socket
{
    protected:
        //Constructor
        Socket();

        //Destructor
        ~Socket();

        //Set Socket to default value [Address Family :IPV4, Type: TCP]
        virtual void set_socket();   

        //Set IP address and Port NO. default value [0, 8080]
        virtual void set_ip_port();

        //Create socket with speceifed addrinfo
        void create_socket();

        //Store socket information in th dns[Domain Name Server] file
        void store_socket_dns();

        //Send msg through socket
       // virtual void Tom_send() = 0;

        //Receive msg through socket
        virtual void Tom_recv() = 0;

        //Close the socket
        void Tom_close(); 

        //Close given socket
        void Tom_close(SOCKET socket);

        struct addrinfo hints; //HINTS about IP family: IPV4 or IPV6, Protocol : TCP or UDP  
        struct addrinfo *bind_address; //Contain same information as hints. Used for binding socket
        struct sockaddr sa;
        socklen_t sa_len = sizeof(sa);

        SOCKET socket_listen;

        std::string ip_addr;
        std::string port_number;
        std::string host_name;

    private:
        //bind socket with address 
        void Tom_bind();

};