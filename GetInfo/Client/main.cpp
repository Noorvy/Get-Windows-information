#include <string>
#include <iostream>
#include <set>

#include <zmq.hpp>
#include "Server_connection.h"

int main()
{
    const std::set<std::string> command_list{ "GetOsVersion", "GetSystemTime", "GetTickCount", 
                                      "GlobalMemoryStatus", "GetDriveType", "GetDiskFreeSpace", 
                                      "GetObjectAcl", "GetObjectOwner", "Exit" };
    zmq::context_t context{ 1 };
    std::string command;

    zmq::socket_t socket{ context, zmq::socket_type::req };
    socket.connect("tcp://localhost:5555");
    S_connect server_message(socket);
    server_message.start_list();

    while (std::cin >> command) {
        if (command_list.find(command) != command_list.end()) {
            if (command == "GetObjectAcl" || command == "GetObjectOwner") {
                std::cout << "Type path to file: (D:/example_path/example_file.txt)" << std::endl;
                std::string temp_path;
                std::cin >> temp_path;
                command.append(temp_path);
            }
            server_message.send_message(command);
        }
        else if (command == "Quit") { return 0; }
        else { std::cout << "Wrong command! Repeat please!" << std::endl; }
    }
}
