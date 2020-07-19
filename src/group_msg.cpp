/**
 *  This test program explains the usuage of the [TOM] communication Protocol
 *  This works as time publisher, continuously send data to the clients wherenver publisher has data
 */

//#include "./../include/Socket.h"
#include "./../include/Tom_Publisher.h"
#include <sstream>
#include <thread>

int main()
{
    //Default Publisher address and port no
    std::string addr = "127.0.0.1";
    std::string port_no = "8082";
    std::string topic_name = "message_group.com";

    std::string mystr;

    std::cout << "Enter the ip address [127.0.0.1] \n";
    std::cin >> addr ;
    std::cout << "Enter the port no [8082], Some ports are reserved \n";
    std::cin >> port_no;
    std::cout << "Enter the topic name [message_group.com]\n";
    std::cin >> topic_name;


    //Initialize publisher and use with two line of command
    Tom_Publisher publisher(addr,port_no,topic_name,4,4);
    publisher.run();

    std::string msg = "Hello ";

    std::cout << "Broadcast your message to the people who search your message \n";

    while (true)
    {
        publisher.push(msg + "  \n");
        publisher.pub();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::cout << "Enter the msg : \n";
        std::cin >> msg;
    }

}
