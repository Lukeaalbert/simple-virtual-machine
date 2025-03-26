#include "SrcMain.h"
#include <iostream>
#include "Machine.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	Machine machine;
	machine.LoadMachineCode(argv[1]);
	machine.Execute();
}
