#include "Client_connect.h"

Client_connect::Client_connect(zmq::socket_t& socket) : _socket(&socket) {}

void Client_connect::_Send() {
	_Serialize();
	zmq::message_t message{_buffer};
	_socket->send(message, zmq::send_flags::none);
}

void Client_connect::_Analize() {
	if (_command == "GetOsVersion") {
		_getInfo.GetOsVersion1(_argument);	
		_Send();
	}
	else if (_command == "GetSystemTime") {
		_getInfo.GetSystemTime1(_argument);
		_Send();
	}
	else if (_command == "GlobalMemoryStatus") {
		_getInfo.GetMemoryStatus1(_argument);
		_Send();
	}
	else if (_command == "GetTickCount") {
		_getInfo.GetTickCount1(_argument);
		_Send();
	}
	else if (_command == "GetDriveType") {
		_getInfo.GetDrivetype1(_argument);
		_Send();
	}
	else if (_command == "GetDiskFreeSpace") {
		_getInfo.GetFreeSpace1(_argument);
		_Send();
	}
	else if (_obj_command == "GetObjectOwner") {
		std::string path(_command.begin() + 14, _command.end());
		_getInfo.GetObjectOwner1(path, _argument);
		_Send();
	}
	else if (_obj_command == "GetObjectAcl") {
		std::string path(_command.begin() + 12, _command.end());
		_getInfo.GetObjectAcl1(path, _argument);
		_Send();
	}
}


void Client_connect::Client_command(zmq::message_t& message) {
	_command.clear();
	_argument.clear();
	_buffer.clear();
	_obj_command.clear();
	uint8_t* temp_ptr{ message.data<uint8_t>() };
	for (auto i{ 0 }; i != message.size(); ++i) {
		_buffer.push_back(*temp_ptr);
		temp_ptr++;
	}
	_Deserialize();
}

void Client_connect::_Serialize() {
	assert(_argument.size());
	_buffer.clear();
	for (auto it{ _argument.begin() }; it != _argument.end(); ++it) {
		_buffer.push_back(static_cast<uint8_t>(*it));
	}
}

void Client_connect::_Deserialize() {
	assert(_buffer.size());
	for (auto it{ _buffer.begin() }; it != _buffer.end(); ++it) {
		_command.push_back(static_cast<char>(*it));
		if (_command == "GetObjectOwner" || _command == "GetObjectAcl") {
			_obj_command = _command;
		}
	}
	_Analize();
}
