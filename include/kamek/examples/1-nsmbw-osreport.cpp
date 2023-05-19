#include <kamek.h>

// NOTE: bss _must_ be init'ed to zero!
// todo: modify the Riiv patcher to take this into account!
// ALSO, this doesn't work on Gecko as the codehandler resets counter=1
// every frame
int counter = 1;

bool bootTest()
{
	if (counter >= (60 * 10))
		return true;

	++counter;
	if ((counter % 60) == 0)
		OSReport("Delaying for absolutely no good reason... [%d]\n", counter);
	return false;
}

//8015BC60 is a test for BRANCHES!
//kmWritePointer(0x8015BC60, &bootTest);
kmWritePointer(0x80328478, &bootTest);

// Hijack the end of dAcPy_c's constructor

kmBranchDefCpp(0x801447D4, NULL, void *, void *obj)
{
	OSReport("dAcPy_c constructed at %p!\n", obj);
	return obj;
}
