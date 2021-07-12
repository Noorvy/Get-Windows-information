#pragma once
#include <iostream>
#include <string>

#include <zmq.hpp>
#include "Serialize_deserialize.h"

class Client_process{
public:
	explicit Client_process(zmq::socket_t& socket) : _socket(&socket) {}

	void send(const std::string &str) {
        zmq::message_t message_send(Conv_data.Serialize(str));
		_socket->send(message_send, zmq::send_flags::none);
		_socket->recv(_message);
        std::cout << Conv_data.Deserialize(_message) << std::endl;
	}
    void help_list() const;

private:
    Convert_data Conv_data;
    zmq::message_t _message;
    zmq::socket_t* _socket{nullptr};
};

void Client_process::help_list() const {
    std::cout << "write one of next command: \n\n";
    std::cout << "- GetOsVersion (return client's windows version)" << std::endl;
    std::cout << "- GetSystemTime (return client's system time)" << std::endl;
    std::cout << "- GetTickCount (return client's time after turn on OS)" << std::endl;
    std::cout << "- GlobalMemoryStatus (return info about client's memory)" << std::endl;
    std::cout << "- GetDriveType (return local/net file's system)" << std::endl;
    std::cout << "- GetDiskFreeSpace (return local disks free space)" << std::endl;
    std::cout << "- GetObjectOwner (return owner registry key)" << std::endl;
    std::cout << "- Exit (quit server)" << std::endl;
    std::cout << "- Quit (quit client)" << std::endl;
}
