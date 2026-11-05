#include <kernel.h>
#include <appmgr.h>
#include <stdio.h>

// Memory grow size for tests
#define GROW_SIZE (125 * 1024 * 1024)

void pauseTest()
{
	printf("DMR test pause\n");
	sceKernelDelayThread(0xFFFFFFFF);
	sceKernelExitProcess(0);
}

void testLimit()
{
	int ret = sceAppMgrGrowMemory(GROW_SIZE, 1);
	printf("sceAppMgrGrowMemory(GROW_SIZE): 0x%08X\n", ret);

	pauseTest();
}

int main()
{
	int ret = SCE_OK;

	printf("DMR test start\n");

	testLimit();

	return 0;
}