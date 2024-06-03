#include "debugger.h"
#include "cpu.h"
#include "stdio.h"
#include "processus.h"

int fact(int n)
{
	if (n < 2)
		return 1;

	return n * fact(n-1);
}


void kernel_start(void)
{
	int i;
	// call_debugger(); useless with qemu -s -S

	i = 10;

	i = fact(i);

	printf("\ffact(10)=%d", i);

	while(1)
	  hlt();

	return;
}
