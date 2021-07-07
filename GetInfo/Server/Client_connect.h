#pragma once
#include <iostream>
#include <vector>
#include <mutex>

#include <zmq.hpp>
#include "winapi.h"

class Client_connect {
public:
	Client_connect(zmq::socket_t& socket);
	void Client_command(zmq::message_t& message); //Start point

private:
	void _Serialize();
	void _Deserialize();
	void _Analize();
	void _Send();
	
private:
	Winapi _getInfo;
	zmq::socket_t* _socket{nullptr};
	std::vector<uint8_t> _buffer; //BUFFER FOR SERIALIZE
	std::string _command; //COMAND FROM CLIENT
	std::string _argument; //ANSWEAR TO CLIENT
	std::string _obj_command; //"GetObjectAcl" and "GetObjectOwner"
};

