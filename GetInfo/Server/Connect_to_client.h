#pragma once
#include <iostream>
#include <vector>
#include <mutex>

#include <zmq.hpp>
#include "winapi.h"
#include "Serialize_deserialize.h"

class Server_process {
public:
	Server_process(zmq::socket_t& socket);
	void clients_command(zmq::message_t& message); //Start point

private:
	void analize(const std::string str);
	void send(const std::string& str);
	
private:
	CommandHandler _getInfo;
	Convert_data _convertMessage;
	zmq::socket_t* _socket{nullptr};
	std::string _argument; //ANSWEAR TO CLIENT
	std::string _obj_command; //"GetObjectOwner"
};

