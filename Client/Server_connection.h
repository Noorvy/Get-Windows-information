#pragma once
#include <iostream>
#include <string>

#include <zmq.hpp>

class S_connect {
public:
	explicit S_connect(zmq::socket_t& socket) : _socket(&socket) {}

	void send_message(const std::string &str) {
        Serialize(str);
        zmq::message_t message_send(_buffer);
		_socket->send(message_send, zmq::send_flags::none);
		_socket->recv(_message);
        Desirialize();
	}
    void start_list() const;

private:
    void Serialize(const std::string& str);
    void Desirialize();

    zmq::message_t _message;
    zmq::socket_t* _socket{nullptr};
    std::vector<uint8_t> _buffer;
    std::string _argument;
};

void S_connect::Serialize(const std::string& str) {
    assert(str.size());
    _buffer.clear(); 
    for (auto it{ str.begin() }; it != str.end(); ++it) {
        _buffer.push_back(static_cast<uint8_t>(*it));
    }
}

void S_connect::Desirialize() {
    _buffer.clear();
    uint8_t* temp_ptr{ _message.data<uint8_t>() };
    for (auto i{ 0 }; i != _message.size(); ++i) {
        _buffer.push_back(*temp_ptr);
        temp_ptr++;
    }

    assert(_buffer.size());
    _argument.clear();
    for (auto it{ _buffer.begin() }; it != _buffer.end(); ++it) {
        _argument.push_back(static_cast<char>(*it));
    }

    //PRINT RESULT FROM SERVER
    std::cout << _argument << std::endl; 
}

void S_connect::start_list() const {
    std::cout << "write one of next command: \n\n";
    std::cout << "- GetOsVersion (return client's windows version)" << std::endl;
    std::cout << "- GetSystemTime (return client's system time)" << std::endl;
    std::cout << "- GetTickCount (return client's time after turn on OS)" << std::endl;
    std::cout << "- GlobalMemoryStatus (return info about client's memory)" << std::endl;
    std::cout << "- GetDriveType (return local/net file's system)" << std::endl;
    std::cout << "- GetDiskFreeSpace (return local disks free space)" << std::endl;
    std::cout << "- GetObjectAcl (return registry key)" << std::endl;
    std::cout << "- GetObjectOwner (return owner registry key)" << std::endl;
    std::cout << "- Exit (quit server)" << std::endl;
    std::cout << "- Quit (quit client)" << std::endl;
}