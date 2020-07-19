#include "./../include/Tom_Publisher.h"

/**
 * @brief Constructor assign [Publisher Address, Publisher Port NO, Max_Number_Client, Message_queue_len]
 */ 
Tom_Publisher::Tom_Publisher(std::string addr, std::string port_no, std::string topic_name,int max_number_client, int max_msg_queue_len) : _addr(addr.c_str()),_port_no(port_no.c_str())
{
    ip_addr = addr;
    port_number = port_no;
    host_name = topic_name;

    _max_client_number = max_number_client; //Maximum number of client can be served
    _msg_queue_len = max_msg_queue_len; //Maximum message hold for new client

    finish = false; //Check for completed execution
    FD_ZERO(&_master); //Make file Descriptor zero
    _timeout.tv_sec = 1;
    _timeout.tv_usec = 1000;
}

/**
 * @brief Publisher run - Create socket and start the publisher behavior
 */
void Tom_Publisher::run()
{
    create_socket(); //Create socket using configuration
    t = std::thread(&Tom_Publisher::Tom_behavior,this); //Run Publisher behaviour has separate thread
}

 /**
  * @brief Publisher behavior - Keep on checking for new connection and when it is found, added that client into client socket
  */ 
void Tom_Publisher::Tom_behavior()
{
    while (!finish)
    {
        //check serving client less than max servable client
        if(_client_socket.size() < _max_client_number) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            Tom_listen(); //Listen for new connection, which block execution untill receive new connection
            Tom_accept(); //Accept new connection, return socket for that connection
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

/**
 * @brief Publisher push -Store recent msg_que_length data into msg_queue
 */
void Tom_Publisher::push(std::string &&msg)
{
    msg_queue.emplace_back(msg); //Add new msg to the msg queue

    //Remove old message if buffer is filled
    if(msg_queue.size() > _msg_queue_len) {
        msg_queue.pop_front();    
    }

}

/**
 * @brief Publisher publish message to all the connected socket
 */
void Tom_Publisher::pub()
{
    //Convert string to char * to send
    std::string temp = msg_queue.back();
    const char* msg = temp.c_str();

    //std::cout << "After select\n";

    std::lock_guard<std::mutex> lck(_mtx);
    //Send the message to all the client
    for(int i = 0; i < _client_socket.size(); ++i) {
        int bytes_sent = send(_client_socket.at(i), msg, strlen(msg),0);
       // printf("Send %d of %d bytes.\n", bytes_sent, (int)strlen(msg));
    }

}


//Override Socket configuration [Address Family, Type]
void Tom_Publisher::set_socket()
{
    hints.ai_family = AF_INET; //set to IPV4
    hints.ai_socktype =  SOCK_STREAM; // Set to TCP
    hints.ai_flags = AI_PASSIVE;
}

//Override IP_address and Port No. 
void Tom_Publisher::set_ip_port()
{
    const char *addr_ptr = _addr.release();
    const char *port_ptr = _port_no.release();
    getaddrinfo(addr_ptr, port_ptr, &hints, &bind_address);
}

/**
 * @brief Server Listen for any new connection at this port 
 */
void Tom_Publisher::Tom_listen()
{
    //printf("Listening...\n");
    if(listen(socket_listen, _max_client_number) < 0) {
        fprintf(stderr, "listen() failed. (%d)\n", GETSOCKETERRNO());
    }
}

/**
 * @brief Server Accept new connection at this port 
 */
void Tom_Publisher::Tom_accept()
{
    std::unique_lock<std::mutex> lck(_mtx);
    //printf("Waiting for connection...\n");
    lck.unlock();

    _socket_client = accept(socket_listen, (struct sockaddr*) &_client_address, &_client_len);
    if(!ISVALIDSOCKET(_socket_client)) {
        lck.lock();
        fprintf(stderr, "accept() failed. (%d)\n", GETSOCKETERRNO());
        lck.unlock();
    }

    lck.lock();
   // printf("Client is connected...\n");
    _client_socket.push_back(_socket_client); //Store new client into client list
    FD_SET(_socket_client, &_master); //Store new client into File Descriptor set
    lck.unlock();

    getnameinfo((struct sockaddr*)&_client_address, _client_len, _address_buffer, sizeof(_address_buffer), 0, 0, NI_NUMERICHOST);

    /*
    lck.lock();
    printf("%s\n", _address_buffer);
    lck.unlock();
    */
}

/**
 * @brief recv msg over socket
 */
void Tom_Publisher::Tom_recv()
{
    printf("Reading request... \n");
    char request[1024];
    int bytes_received = recv(_socket_client, request, 1024, 0);
    printf("Received %d bytes. \n", bytes_received);
}

/**
 * @brief Publisher finish - Close all the sockets [Listening and all client socket]
 */
void Tom_Publisher::close_clients_socket()
{
    std::lock_guard<std::mutex> lck(_mtx);
    //closing clients socket
    for(int i = 0; i < _client_socket.size(); ++i) {
        Tom_close(_client_socket[i]);
    }
}


/**
 * @brief Destructor - join the thread
 */
Tom_Publisher::~Tom_Publisher()
{
    finish = true;
    close_clients_socket();
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    t.join(); //Join the thread
}