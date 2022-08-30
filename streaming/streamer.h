#pragma once
#include <Windows.h>
#include <Urlmon.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "../blowfish\blowfish.h"
#pragma comment( lib, "Urlmon.lib" )

namespace fs = std::filesystem;

bool replace(std::string& str, const std::string& from, const std::string& to);

struct EncryptedData_t
{
	std::string a;
	std::string b;
	std::string c;
	std::string d;

};

std::string Decrypt(EncryptedData_t encryptedData);
std::string StreamFileToMemString(std::wstring URL);
std::string GetDecryptedDLL();