#include "Ops.h"
#include "Machine.h"

void Exit::Execute(Machine& machine)
{
	machine.SetFlag("exit", true);
}

void MovI::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParams), std::get<1>(mParams));
}

void Add::Execute(Machine& machine)
{
	int res = machine.GetRegister(std::get<1>(mParams)) + machine.GetRegister(std::get<2>(mParams));
	machine.SetRegister(std::get<0>(mParams), res);
}

void Mov::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParams), machine.GetRegister(std::get<1>(mParams)));
}

void Sub::Execute(Machine& machine)
{
	int res = machine.GetRegister(std::get<1>(mParams)) - machine.GetRegister(std::get<2>(mParams));
	machine.SetRegister(std::get<0>(mParams), res);
}

void Mul::Execute(Machine& machine)
{
	int res = machine.GetRegister(std::get<1>(mParams)) * machine.GetRegister(std::get<2>(mParams));
	machine.SetRegister(std::get<0>(mParams), res);
}

void Div::Execute(Machine& machine)
{
	int res = machine.GetRegister(std::get<1>(mParams))/machine.GetRegister(std::get<2>(mParams));
	machine.SetRegister(std::get<0>(mParams), res);
}

void Inc::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParams), machine.GetRegister(std::get<0>(mParams)) + 1);
}

void Dec::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParams), machine.GetRegister(std::get<0>(mParams)) - 1);
}

void Jmp::Execute(Machine& machine)
{
	machine.SetRegister("pc", machine.GetRegister(std::get<0>(mParams)));
}

void Cmpeq::Execute(Machine& machine)
{
	bool res = machine.GetRegister(std::get<0>(mParams)) == machine.GetRegister(std::get<1>(mParams));
	machine.SetFlag("test", res);
}

void Cmplt::Execute(Machine& machine)
{
	bool res = machine.GetRegister(std::get<0>(mParams)) < machine.GetRegister(std::get<1>(mParams));
	machine.SetFlag("test", res);
}

void Jt::Execute(Machine& machine)
{
	if (machine.GetFlag("test")) {
		machine.SetRegister("pc",  machine.GetRegister(std::get<0>(mParams)));
	}
}

void Jnt::Execute(Machine& machine)
{
	if (!machine.GetFlag("test")) {
		machine.SetRegister("pc",  machine.GetRegister(std::get<0>(mParams)));
	}
}

void Push::Execute(Machine& machine)
{
	machine.PushStack(machine.GetRegister(std::get<0>(mParams))); 
	machine.SetRegister("sc", machine.GetRegister("sc") + 1); // ++sc
}

void Pop::Execute(Machine& machine)
{
	int i = machine.PopStack();
	machine.SetRegister(std::get<0>(mParams), i); 
	machine.SetRegister("sc", machine.GetRegister("sc") - 1); // --sc
}

void Load::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParams), 
		machine.GetStack(machine.GetRegister(std::get<1>(mParams)))
	);
}

void Store::Execute(Machine& machine)
{
	machine.SetStack(
		machine.GetRegister(std::get<0>(mParams)),
		machine.GetRegister(std::get<1>(mParams))
	);
}

void LoadI::Execute(Machine& machine)
{
	machine.SetRegister(std::get<0>(mParams), 
		machine.GetStack(std::get<1>(mParams))
	);
}

void StoreI::Execute(Machine& machine)
{
	machine.SetStack(
		std::get<0>(mParams),
		machine.GetRegister(std::get<1>(mParams))
	);
}

void LoadSc::Execute(Machine& machine)
{
	machine.SetRegister(
		std::get<0>(mParams), 
		machine.GetStack(
			machine.GetRegister("sc") - 
			machine.GetRegister(std::get<1>(mParams))
			- 1
		)
	);
}

void StoreSc::Execute(Machine& machine)
{
	machine.SetStack(
		machine.GetRegister("sc") - 
		machine.GetRegister(std::get<0>(mParams))
		- 1,
		machine.GetRegister(std::get<1>(mParams))
	);
}

void Pendown::Execute(Machine& machine)
{
	machine.SetFlag("pen", true);
}

void Penup::Execute(Machine& machine)
{
	machine.SetFlag("pen", false);
}

void Fwd::Execute(Machine& machine)
{
	int drawAmount = machine.GetRegister(std::get<0>(mParams));
	machine.DrawTurtle(drawAmount);
}

void Back::Execute(Machine& machine)
{
	int drawAmount = machine.GetRegister(std::get<0>(mParams));
	machine.DrawTurtle(-drawAmount);
}