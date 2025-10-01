#include "Memory/Mem.h"
#include "Cheat/Threads/Reader.h"


int main()
{
	void* hThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Reader, 0, 0, 0);
	void* hThread3 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)RenderOverlayThread, 0, 0, 0);

a:
	Sleep(10000);
	goto a;
}