#include <string>
#include <iostream>

#include <zmq.hpp>
#include "Connect_to_client.h"


int main(int agrc, char* argv[]) 
{
    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, zmq::socket_type::rep };
    socket.bind("tcp://*:5555");
    Server_process ServProcess(socket);

    while (true)
    {
        zmq::message_t request;
        socket.recv(request, zmq::recv_flags::none);
        if (request.to_string() == "Exit") { 
            socket.send(zmq::buffer("Turn off server!"), zmq::send_flags::none);
            return 0; 
        }
        else { 
            ServProcess.clients_command(request);
        }
    }  
    return 0;
}
