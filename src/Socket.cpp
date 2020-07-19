#include "../include/Socket.h"

/**
 * @brief Initialize Winsock and set hints to zero 
 */
Socket::Socket()
{
    #if defined(_WIN32)
        WSDATA d;
  
        if(WSAStartup(MAKEWORD(2,2), &d)) {
            fprinf(stderr, "Failed to initialize.\n");
            return 1;
        }
    #endif

    memset(&hints, 0, sizeof(hints)); //Set to zero
}

/**
 * @brief Initialize socket with default value [Address Family: IPV4 or IPV6, Type: TCP]
 */
void Socket::set_socket()
{
    hints.ai_family = AF_INET; //set to IPV4
    hints.ai_socktype =  SOCK_STREAM; // Set to TCP
    hints.ai_flags = AI_PASSIVE;

}

/**
 * @brief Set ip_addr to local Host and Port NO. 8080 
 */
void Socket::set_ip_port()
{
    getaddrinfo(0, "8080", &hints, &bind_address);
}

/**
 * @brief Create Socket
 */
void Socket::create_socket()
{
    set_socket();
    set_ip_port();

    printf("Creating socket...\n");
    socket_listen = socket(bind_address->ai_family, bind_address->ai_socktype, bind_address->ai_protocol);

    if (!ISVALIDSOCKET(socket_listen)) {
        fprintf(stderr, "socket() failed. (%d)\n", GETSOCKETERRNO());
    }

    Tom_bind();
}

/**
 * @brief Bind the socket with address stored in the hints
 */
void Socket::Tom_bind()
{
    printf("Binding socket to local address...\n");
    const char *addr_ptr = ip_addr.c_str();
    while (bind(socket_listen, bind_address->ai_addr, bind_address->ai_addrlen))
    {
        int port_no_ = (rand() % (65535 - 49152))  + 49152;
        port_number = std::to_string(port_no_);
        const char *port_no = port_number.c_str();
        getaddrinfo(addr_ptr, port_no, &hints, &bind_address);
    }
    std::cout <<"=========================================\n";
    std::cout << "PORT NUMBER : " << port_number << std::endl;
    std::cout <<"==========================================\n";
    store_socket_dns();
}

/**
 * @brief Store Socket Information in DNS
 */
void Socket::store_socket_dns()
{
    // std::ofstream file.open("./../dns.txt");
    std::ofstream myfile ("./../dns.txt",std::ofstream::out);

    myfile << host_name + " " + ip_addr + " " + port_number;
    
    myfile.close();
}

/**
 * @brief Close the socket listen
 */
void Socket::Tom_close()
{
    CLOSESOCKET(socket_listen);
}

/**
 * @brief Close given socket
 */
void Socket::Tom_close(SOCKET s) 
{
    shutdown(s, SHUT_RDWR); //Shutdown socket 
    CLOSESOCKET(s); // close that socket
}

/**
 * @brief Destructor
 */
Socket::~Socket()
{
    std::cout << "Socket destructor \n";

    Tom_close();

    freeaddrinfo(bind_address);

    #if defined(_WIN32)
        WSACleanup();
    #endif

}
