#include "winapi.h"


void Winapi::GetOsVersion1(std::string& str) const {
    if (IsWindows10OrGreater()) { str.append("Windows 10.0"); return; }
	if (IsWindows8Point1OrGreater()) { str.append("Windows 8.1"); return; }
	if (IsWindows8OrGreater()) { str.append("Windows 8"); return; }
	if (IsWindows7OrGreater()) { str.append("Windows 7"); return; }
	if (IsWindowsVistaOrGreater()) { str.append("Windows Vista"); return; }
};

void Winapi::GetMemoryStatus1(std::string& str) const {
	MEMORYSTATUSEX statex;
	const auto DIV{ 1048576 }; // To convert bytes to Mb
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);

    str.append("Total physical memory: ");
    str.append(std::to_string(statex.ullTotalPhys / DIV));
    str.append(" Mb\n");
    //-------------
    str.append("Physical memory free: ");
    str.append(std::to_string(statex.ullAvailPhys / DIV));
    str.append(" Mb\n");
    //-------------
    str.append("Total paging file memory: ");
    str.append(std::to_string(statex.ullTotalPageFile / DIV));
    str.append(" Mb\n");
    //-------------
    str.append("Paging file memory free: ");
    str.append(std::to_string(statex.ullAvailPageFile / DIV));
    str.append(" Mb\n");
    //-------------
    str.append("Total virtual memory: ");
    str.append(std::to_string(statex.ullTotalVirtual / DIV));
    str.append(" Mb\n");
    //-------------
    str.append("Virtual memory free: ");
    str.append(std::to_string(statex.ullAvailVirtual / DIV));
    str.append(" Mb\n");
};


void Winapi::GetSystemTime1(std::string& str) const {
    SYSTEMTIME lt;
    GetLocalTime(&lt);

    str.append(std::to_string(lt.wHour));
    str.append(":");
    str.append(std::to_string(lt.wMinute));
}

void Winapi::GetTickCount1(std::string& str) const {
    auto hour_value = (static_cast<int>(GetTickCount64())) / 3600000;
    auto min_value = ((static_cast<int>(GetTickCount64())) / 60000) - (hour_value * 60);

    str.append(std::to_string(hour_value));
    str.append(":");
    str.append(std::to_string(min_value));
}

void Winapi::GetDrivetype1(std::string& str) {
    wchar_t buffer[256];
    DWORD sizebuf{ 256 };
    GetLogicalDriveStrings(sizebuf, buffer);
    wchar_t* buf{ buffer };
    std::wstring ws;
   
    while (*buf) {
        ws.push_back(*buf);
        auto d = GetDriveType(buf);
        ws.push_back(d);
        buf += wcslen(buf) + 1;
    }
    std::string s(ws.begin(), ws.end());
    _hard_name.clear();
    for (auto i{ 0 }; i != s.size(); i++) {
        switch (s[i]) {
        case(0): str.push_back(s[i]); str.append(": The drive type cannot be determined\n"); break;
        case(1): str.push_back(s[i - 1]); str.append(": The root path is invalid\n"); break;
        case(2): str.push_back(s[i - 1]); str.append(": Flash card\n"); break;
        case(3): str.push_back(s[i - 1]); str.append(": Hard drive\n"); _hard_name.push_back(s[i - 1]); _hard_name.append(":\\"); break;
        case(4): str.push_back(s[i - 1]); str.append(": Remote (network) drive\n"); break;
        case(5): str.push_back(s[i - 1]); str.append(": CD-ROM drive\n"); break;
        case(6): str.push_back(s[i - 1]); str.append(": RAM disk\n"); break;
        default: break;
        }
    }
}

typedef BOOL(WINAPI* P_GDFSE)(LPCTSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

void Winapi::GetFreeSpace1(std::string& str) {
    const auto DIV{ 1048576 }; // To convert bytes to Mb
    std::string temp;
    GetDrivetype1(temp);
    unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

    auto it_name_begin = _hard_name.begin();
    auto it_name_end = _hard_name.begin();
    it_name_end += 2;
    while (it_name_begin != _hard_name.end()) {
        std::wstring ws;
        ws.assign(it_name_begin, it_name_end);
        auto result = GetDiskFreeSpaceEx(ws.c_str(), (PULARGE_INTEGER)&i64FreeBytesToCaller,
            (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);

        if (result) {
            str.append(it_name_begin, it_name_end);
            str.append("Total space = ");
            str.append(std::to_string((i64TotalBytes) / (DIV)));
            str.append("   Free space = ");
            str.append(std::to_string((i64FreeBytes) / (DIV)));
            str.append("\n");
        }
        if ((it_name_end + 1) == _hard_name.end()) break;
        it_name_end += 3;
        it_name_begin += 3;
    }
}

void Winapi::GetObjectOwner1(const std::string& file_name, std::string& str) const {
    PSID OwnerSid;
    PACL Dacl;
    WCHAR buf[1024];
    WCHAR szName[1024];
    DWORD dwNameLen;
    WCHAR szDomainName[1024];
    DWORD dwDomainNameLen;
    SID_NAME_USE use;
    std::wstring temp_wstr;
    temp_wstr.assign(file_name.begin(), file_name.end());

    GetNamedSecurityInfoW(temp_wstr.c_str(), SE_FILE_OBJECT,
        OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, &OwnerSid,
        NULL, &Dacl, NULL, NULL);

    LookupAccountSidW(NULL, OwnerSid, szName, &dwNameLen, szDomainName, &dwDomainNameLen, &use);
    wsprintfW(buf, L"Owner: %s\n", szName);

    auto i{ 0 };
    temp_wstr.clear();
    while (buf[i]) {
        temp_wstr.push_back(buf[i]);
        ++i;
    }
    if (temp_wstr.size() > 100) { str.append("Wrong file path! Repeat!"); }
    else str.append(temp_wstr.begin(), temp_wstr.end());
}

void Winapi::GetObjectAcl1(const std::string& file_name, std::string& str) const {
    PSID OwnerSid;
    PACL Dacl;
    PSECURITY_DESCRIPTOR pSecDesc;
    TRUSTEE_W Trustee;
    ACCESS_MASK Rigth;
    std::wstring temp_wstr;
    temp_wstr.assign(file_name.begin(), file_name.end());

    GetNamedSecurityInfoW(temp_wstr.c_str(), SE_FILE_OBJECT,
        OWNER_SECURITY_INFORMATION | DACL_SECURITY_INFORMATION, &OwnerSid,
        NULL, &Dacl, NULL, &pSecDesc);

    BuildTrusteeWithSidW(&Trustee, OwnerSid);
    if (GetEffectiveRightsFromAcl(Dacl, &Trustee, &Rigth) != ERROR_SUCCESS) {
        str.append("Access denial!\n");
    }
    else {
        str.append("You have access!\n");
    }
}