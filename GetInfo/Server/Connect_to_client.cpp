#include "Connect_to_client.h"

Server_process::Server_process(zmq::socket_t& socket) : _socket(&socket) {}

void Server_process::analize(const std::string command) {
	if (command == "GetOsVersion") {
		_getInfo.getOsVersion(_argument);	
		send(_argument);
	}
	else if (command == "GetSystemTime") {
		_getInfo.getSystemTime(_argument);
		send(_argument);
	}
	else if (command == "GlobalMemoryStatus") {
		_getInfo.getMemoryStatus(_argument);
		send(_argument);
	}
	else if (command == "GetTickCount") {
		_getInfo.getTickCount(_argument);
		send(_argument);
	}
	else if (command == "GetDriveType") {
		_getInfo.getDisktype(_argument);
		send(_argument);
	}
	else if (command == "GetDiskFreeSpace") {
		_getInfo.getFreeSpace(_argument);
		send(_argument);
	}
	else if (_obj_command == "GetObjectOwner") {
		std::string path(command.begin() + 14, command.end());
		_getInfo.getObjectOwner(path, _argument);
		send(_argument);
	}
}

void Server_process::clients_command(zmq::message_t& message) {
	auto command_from_client = _convertMessage.Deserialize(message, _obj_command);
	std::cout << "Request: " << command_from_client << std::endl;
	_argument.clear();
	analize(command_from_client);
}

void Server_process::send(const std::string& str) {
	std::cout << "Reply: " << _argument << std::endl;
	zmq::message_t message{ _convertMessage.Serialize(str) };
	_socket->send(message, zmq::send_flags::none);
}
