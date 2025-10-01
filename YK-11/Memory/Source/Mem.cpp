#include "../Mem.h"

void* g_hProc = nullptr;

unsigned long GetProcID(const wchar_t* procName)
{
	long procID = 0;
	void* hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 procEntry;
		procEntry.dwSize = sizeof(procEntry);

		if (Process32First(hSnap, &procEntry))
		{
			do {

				if (!_wcsicmp(procName, procEntry.szExeFile))
				{
					procID = procEntry.th32ProcessID;
					break;
				}

			} while (Process32Next(hSnap, &procEntry));
		}
	}
	else
	{
		printf("Failed to acquire handle! [GetProcID]\n");
		return 69420;
	}

	CloseHandle(hSnap);
	return procID;
}

unsigned __int64 GetModuleBase(const wchar_t* moduleName, unsigned long procID)
{
	unsigned __int64 moduleBase = 0;
	void* hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);

		if (Module32First(hSnap, &modEntry))
		{
			do {
				if (!_wcsicmp(moduleName, modEntry.szModule))
				{
					moduleBase = (unsigned __int64)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	else
	{
		printf("Failed to acquire handle! [GetModuleBase]\n");
		return 42069;
	}

	CloseHandle(hSnap);
	return moduleBase;
}

unsigned __int64 Attach()
{
	unsigned long pid = GetProcID(L"cs2.exe");
	void* hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);

	g_hProc = hProc;


	return GetModuleBase(L"client.dll", pid);
}