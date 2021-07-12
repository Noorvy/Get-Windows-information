#pragma once
#include <iostream>
#include <string>

#include <aclapi.h>
#include <Windows.h>
#include <VersionHelpers.h>

class CommandHandler {
public:
	CommandHandler() { }

	void getOsVersion(std::string& str) const;
	void getMemoryStatus(std::string& str) const;
	void getSystemTime(std::string& str) const;
	void getTickCount(std::string& str) const;
	void getDisktype(std::string& str);
	void getFreeSpace(std::string& str);
	void getObjectOwner(const std::string& file_name, std::string& str) const;
private:
	std::string _hard_name; //HARD DISK BUFFER
};

