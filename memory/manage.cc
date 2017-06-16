#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

int bss;
int data=10;

int main(int arg, char* argv[]) {
	int local = 10;
	long addr;
	void* handle;
	char* heap = (char*)malloc(100);
	handle = dlopen("/lib/libc.so.6", RTLD_LAZY);
	addr = (long)dlsym(handle, "printf");

	printf("main: 0x%08x\n", &main);
	printf("data: 0x%08x\n", &data);
	printf("bss: 0x%08x\n", &bss);
	printf("heap: 0x%08x\n", heap);
	printf("library: 0x%08x\n", addr);
	printf("stack: 0x%08x\n", &local);
	free(heap);

}
