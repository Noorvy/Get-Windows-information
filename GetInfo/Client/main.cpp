#include <string>
#include <iostream>
#include <set>

#include <zmq.hpp>
#include "Connect_to_server.h"

int main(int argc, char* argv[])
{
    const std::set<std::string> Command_list{ "GetOsVersion", "GetSystemTime", "GetTickCount", 
                                      "GlobalMemoryStatus", "GetDriveType", "GetDiskFreeSpace",
                                      "GetObjectOwner", "Exit" };
    zmq::context_t context{ 1 };
    zmq::socket_t socket{ context, zmq::socket_type::req };
    socket.connect("tcp://localhost:5555");
    
    std::string Command_to_server;
    Client_process ClientProcess(socket);
    ClientProcess.help_list();

    while (std::cin >> Command_to_server) {
        if (Command_list.find(Command_to_server) != Command_list.end()) {
            if (Command_to_server == "GetObjectOwner") {
                std::cout << "Type path to file: (D:/example_path/example_file.txt)" << std::endl;
                std::string temp_path;
                std::cin >> temp_path;
                Command_to_server.append(temp_path);
            }
            ClientProcess.send(Command_to_server);
        }
        else if (Command_to_server == "Quit") { return 0; }
        else { std::cout << "Wrong command! Repeat please!" << std::endl; }
    }
    return 0;
}
