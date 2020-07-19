/**
 * TOM alternated name for Communication[COM].
 * Here lies implementation for Tom_Publisher class.
 * 
 * Details:
 *         1.Tom_Publisher is kind of Server. 
 *         2.And It can connected to multiple client.[Max_Number_Client]
 *         3.After accepted client, which is less than Tom_Publisher can serve. 
 *         4.Whenever called send, it will send the data to all connected client.  
 */

#include "./../include/Socket.h"
#include <sys/time.h>
#include <vector>
#include <deque>
#include <iostream>
#include <memory>
#include <future>
#include <string>
#include <thread>
#include <mutex>

class Tom_Publisher : private Socket 
{
    public:
        //Constructor - Assign value [Publisher Address, Publisher Port NO, Max_Number_Client, Message_queue_len]
        Tom_Publisher(std::string addr, std::string port_no, std::string topic_name, int max_no_client, int msg_queue_length );

        //Destructor
        ~Tom_Publisher();

        //Copy Constructor deleted
        //Copy Assignment deleted 
        
        //Move constructor
        Tom_Publisher(Tom_Publisher &&source);

        //Move assignment operator
        Tom_Publisher& operator= (Tom_Publisher &&source);

        //Publisher run - Create socket and start the publisher behavior
        void run();

        //Publisher push -Store recent msg_que_length data into msg_queue
        void push(std::string &&msg);

        //Publisher publish message to all the connected socket
        void pub();

    protected:
        //Publisher behavior
        void Tom_behavior();

        //Override Socket configuration [Address Family, Type]
        void set_socket() override;

        //Override IP_address and Port No. 
        void set_ip_port() override;

        //Publisher Listen for any new connection at this port 
        void Tom_listen();

        //Publisher accept new connection and add that socket to the list.
        void Tom_accept();

        //Publisher receive message through socket
        void Tom_recv() override;

        //Publisher close socket which listening socket and all the client connected socket
        void close_clients_socket();


    private:
        std::unique_ptr<const char> _addr; //Source address or Host Name
        std::unique_ptr<const char> _port_no; //Source Port or Host port
        int _max_client_number = 1; //Number of client can be served at a time
        int _msg_queue_len = 1;

       
        std::vector<SOCKET> _client_socket; //Collection of client socket
        SOCKET _socket_client;
        SOCKET _max_socket = 1;
        struct timeval _timeout;

        struct sockaddr_storage _client_address;
        socklen_t _client_len = sizeof(_client_address);
        char _address_buffer[100];
        //std::deque<std::unique_ptr<std::string>> msg_queue; //Message queue 
        std::deque<std::string> msg_queue;

        fd_set _master; //Store all the client in file Descriptor set

        std::thread t;
        std::mutex _mtx;
        std::atomic<bool> finish;

};


