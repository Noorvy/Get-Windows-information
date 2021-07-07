#pragma once
#include <iostream>
#include <string>

#include <aclapi.h>
#include <Windows.h>
#include <VersionHelpers.h>

class Winapi {
public:
	Winapi() { }

	void GetOsVersion1(std::string& str) const;
	void GetMemoryStatus1(std::string& str) const;
	void GetSystemTime1(std::string& str) const;
	void GetTickCount1(std::string& str) const;
	void GetDrivetype1(std::string& str);
	void GetFreeSpace1(std::string& str);
	void GetObjectAcl1(const std::string& file_name, std::string& str) const;
	void GetObjectOwner1(const std::string& file_name, std::string& str) const;
private:
	std::string _hard_name; //HARD DISK BUFFER
};

