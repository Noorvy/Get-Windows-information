#pragma once
#include <iostream>
#include <string>

#include <zmq.hpp>

class S_connect {
public:
	explicit S_connect(zmq::socket_t& socket) : socket_(&socket) {}

	void send_message(const std::string &str) {
        Serialize(str);
        zmq::message_t message_send(buffer_);
		socket_->send(message_send, zmq::send_flags::none);
		socket_->recv(message_);
        Desirialize();
	}
    void start_list() const;

private:
    void Serialize(const std::string& str);
    void Desirialize();
    void print_result();

    zmq::message_t message_;
    zmq::socket_t* socket_{nullptr};
    std::vector<uint8_t> buffer_;
    std::string argument_;
};


void S_connect::print_result() {
    std::cout << argument_ << std::endl;
}

void S_connect::Serialize(const std::string& str) {
    assert(str.size());
    buffer_.clear(); 
    for (auto it{ str.begin() }; it != str.end(); ++it) {
        buffer_.push_back(static_cast<uint8_t>(*it));
    }
}

void S_connect::Desirialize() {
    buffer_.clear();
    uint8_t* temp_ptr{ message_.data<uint8_t>() };
    for (auto i{ 0 }; i != message_.size(); ++i) {
        buffer_.push_back(*temp_ptr);
        temp_ptr++;
    }

    assert(buffer_.size());
    argument_.clear();
    for (auto it{ buffer_.begin() }; it != buffer_.end(); ++it) {
        argument_.push_back(static_cast<char>(*it));
    }

    print_result();
}

void S_connect::start_list() const {
    std::cout << "write one of next command: \n\n";
    std::cout << "- GetOsVersion (return client's windows version)" << std::endl;
    std::cout << "- GetSystemTime (return client's system time)" << std::endl;
    std::cout << "- GetTickCount (return client's time after turn on OS)" << std::endl;
    std::cout << "- GlobalMemoryStatus (return info about client use memory)" << std::endl;
    std::cout << "- GetDriveType (return local/net file's system)" << std::endl;
    std::cout << "- GetDiskFreeSpace (return free space from local disks)" << std::endl;
    std::cout << "- GetObjectAcl (return riestr key)" << std::endl;
    std::cout << "- GetObjectOwner (return owner riestr key)" << std::endl;
    std::cout << "- Exit (quit server)" << std::endl;
    std::cout << "- Quit (quit client)" << std::endl;
}
