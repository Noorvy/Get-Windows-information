#pragma once
#include <iostream>
#include <assert.h>
#include <vector>

#include "zmq.hpp"

class Convert_data {
public:
	Convert_data() {}

	std::vector<uint8_t> Serialize(const std::string& str) {
		assert(str.size());
		std::vector<uint8_t> buffer;
		for (auto it{ str.begin() }; it != str.end(); ++it) {
			buffer.push_back(static_cast<uint8_t>(*it));
		}
		return buffer;
	}

	std::string Deserialize(zmq::message_t& message) {
		std::vector<uint8_t> buffer;
        std::string result;
        uint8_t* temp_ptr{ message.data<uint8_t>() };
        for (auto i{ 0 }; i != message.size(); ++i) {
            buffer.push_back(*temp_ptr);
            temp_ptr++;
        }

        assert(buffer.size());
        for (auto it{ buffer.begin() }; it != buffer.end(); ++it) {
            result.push_back(static_cast<char>(*it));
        }
		return result;
	}

    std::string Deserialize(zmq::message_t& message, std::string& obj_command) {
		std::vector<uint8_t> buffer;
		std::string result;
		uint8_t* temp_ptr{ message.data<uint8_t>() };
		for (auto i{ 0 }; i != message.size(); ++i) {
			buffer.push_back(*temp_ptr);
			temp_ptr++;
		}

		for (auto it{ buffer.begin() }; it != buffer.end(); ++it) {
			result.push_back(static_cast<char>(*it));
			if (result == "GetObjectOwner" || result == "GetObjectAcl") {
				obj_command = result;
			}
		}
		return result;
    }
};
