#pragma once
#include "../Includes.h"

extern void* g_hProc;

template<class T>
T Read(unsigned __int64 addr)
{
	T buf;
	ReadProcessMemory(g_hProc, (BYTE*)addr, &buf, sizeof(buf), 0);
	return buf;
}

unsigned __int64 Attach();

unsigned long GetProcID(const wchar_t* procName);

unsigned __int64 GetModuleBase(const wchar_t* moduleName, unsigned long procID);