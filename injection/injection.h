#pragma once
#include <string>
#include <Windows.h>

bool ManualMap();
uintptr_t get_module_base_address(DWORD procId, const char* modName);
DWORD FindProcessId(const std::wstring & processName);