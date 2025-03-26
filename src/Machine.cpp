#include <fstream>
#include "Machine.h"

namespace {
	void DumpRegAndFlags(std::ofstream& o, const std::unordered_map<std::string, int>& rMap,
		const std::unordered_map<std::string, bool>& fMap)
		{
			o << "********************\n"
			<< "r0=" << rMap.at("r0") << "\n"
			<< "r1=" << rMap.at("r1") << "\n"
			<< "r2=" << rMap.at("r2") << "\n"
			<< "r3=" << rMap.at("r3") << "\n"
			<< "r4=" << rMap.at("r4") << "\n"
			<< "r5=" << rMap.at("r5") << "\n"
			<< "r6=" << rMap.at("r6") << "\n"
			<< "r7=" << rMap.at("r7") << "\n"
			<< "pc=" << rMap.at("pc") << "\n"
			<< "sc=" << rMap.at("sc") << "\n"
			<< "ex=" << rMap.at("ex") << "\n"
			<< "tx=" << rMap.at("tx") << "\n"
			<< "ty=" << rMap.at("ty") << "\n"
			<< "tr=" << rMap.at("tr") << "\n"
			<< "tc=" << rMap.at("tc") << "\n"
			<< "exit=" << fMap.at("exit") << "\n"
			<< "test=" << fMap.at("test") << "\n"
			<< "pen=" << fMap.at("pen") << "\n";
		}

	void DumpStack(std::ofstream& o, const std::vector<int>& stack, int pc, int sc) {
		o << "pc: " << pc << " Stack: ";
		for (int i = 0; i < sc; ++i) {
			o << stack[i] << ' ';
		}
		o << "\n";
	}
}

Machine::Machine():
	mCreatorMap{
		{"exit", CreateOp<Exit>},
		{"movi", CreateOp<MovI>},
		{"add", CreateOp<Add>},
		{"mov", CreateOp<Mov>},
		{"sub", CreateOp<Sub>},
		{"mul", CreateOp<Mul>},
		{"div", CreateOp<Div>},
		{"inc", CreateOp<Inc>},
		{"dec", CreateOp<Dec>},
		{"jmp", CreateOp<Jmp>},
		{"cmpeq", CreateOp<Cmpeq>},
		{"cmplt", CreateOp<Cmplt>},
		{"jt", CreateOp<Jt>},
		{"jnt", CreateOp<Jnt>},	
		{"push", CreateOp<Push>},
		{"pop", CreateOp<Pop>},
		{"load", CreateOp<Load>},
		{"store", CreateOp<Store>},
		{"loadi", CreateOp<LoadI>},
		{"storei", CreateOp<StoreI>},
		{"loadsc", CreateOp<LoadSc>},
		{"storesc", CreateOp<StoreSc>},
		{"pendown", CreateOp<Pendown>},
		{"penup", CreateOp<Penup>},
		{"fwd", CreateOp<Fwd>},
		{"back", CreateOp<Back>}
	},
	mRegisters{
		{"r0", 0},
		{"r1", 0},
		{"r2", 0},
		{"r3", 0},
		{"r4", 0},
		{"r5", 0},
		{"r6", 0},
		{"r7", 0},
		{"pc", 0},
		{"sc", 0},
		{"ex", 0},
		{"tx", 0},
		{"ty", 0},
		{"tr", 0},
		{"tc", 0}
	},
	mFlags{
		{"exit", false},
		{"test", false},
		{"pen", false}
	},
	mStack(1024),
	mBitmapImage(320, 240),
	mImageDrawer(mBitmapImage),
	mColorMap{
		{0, {255, 255, 255}}, // white
		{1, {255, 255, 0}}, // yellow
		{2, {255, 0, 255}}, // magenta
		{3, {255, 0, 0}}, // red
		{4, {0, 255, 255}}, // cyan
		{5, {0, 255, 0}}, // green
		{6, {0, 0, 255}}, // blue
		{7, {0, 0, 0}} //black
	}
{
	mBitmapImage.set_all_channels(0,0,0); // clear image to black
	mImageDrawer.pen_width(1);
	mImageDrawer.pen_color(255, 255, 255); // white color by default
}

void Machine::LoadMachineCode(const std::string& fileName)
{
	std::ifstream machineCodeFile;
	machineCodeFile.open(fileName);
	std::string line;
	char kDelim = ' '; // split on first occurence of comma in order to get opName and args
	std::string opName;
	std::string params;

	while (getline(machineCodeFile, line)) {
		size_t i = line.find(kDelim);
		if (i != std::string::npos) {
			opName = line.substr(0, i);
			params = line.substr(i+1, line.size());
		}
		else {
			opName = line.substr(0, i);
			params.clear();
		}
		std::shared_ptr<Op> ptr = mCreatorMap.at(opName)();
		ptr -> Parse(params);
		mOps.emplace_back(ptr);
	}
	machineCodeFile.close();
}

void Machine::Execute() {
	std::ofstream lOutfile; 
	lOutfile.open("log.txt");
	std::ofstream sOutfile; 
	sOutfile.open("stack.txt");
	DumpRegAndFlags(lOutfile, mRegisters, mFlags);
	int pc = 0;
	while (!GetFlag("exit")) {
		pc = GetRegister("pc");
		std::shared_ptr<Op> op = mOps[pc];
		lOutfile << "Executing: " << op -> GetName() << "\n";
		DumpStack(sOutfile, mStack, pc, GetRegister("sc"));
		SetRegister("pc", pc + 1);
		try {
			op -> Execute(*this);
		}
		catch (FatalException fe) {
			SetRegister("ex", fe.getExceptionCode());
			SetFlag("exit", true);
		}
		catch (NonfatalException nfe) {
			SetRegister("ex", nfe.getExceptionCode());
		}
		DumpRegAndFlags(lOutfile, mRegisters, mFlags);
	}
	DumpStack(sOutfile, mStack, pc+1, GetRegister("sc"));
	lOutfile.close();
	sOutfile.close();
	mBitmapImage.save_image("output.bmp");
}

int Machine::GetRegister(const std::string& regName) {
	return mRegisters.at(regName);
}

void Machine::SetRegister(const std::string& regName, int n) {
	mRegisters.at(regName) = n;
}

bool Machine::GetFlag(const std::string& flagName) {
	return mFlags.at(flagName);
}

void Machine::SetFlag(const std::string& flagName, bool f) {
	mFlags.at(flagName) = f;
}

void Machine::PushStack(int i) {
	mStack[GetRegister("sc")] = i;
}

int Machine::PopStack() {
	int i = mStack[GetRegister("sc")-1];
	mStack[GetRegister("sc")-1] = 0;
	return i;
}

int Machine::GetStack(int i) {
	return mStack[i];
}

void Machine::SetStack(int idx, int value) {
	mStack[idx] = value;
}

void Machine::DrawTurtle(int drawAmount) {
	int colorMapIndex = GetRegister("tc");
	if (colorMapIndex <= 7) {
		auto penColors = mColorMap[colorMapIndex];
		// pen color should be value of color map at ColorMapIndex
		mImageDrawer.pen_color(std::get<0>(penColors), std::get<1>(penColors), std::get<2>(penColors));
	}
	float angle = (GetRegister("tr") * 0.0174533f); // tr converted to radians
	int tx = GetRegister("tx");
	int ty = GetRegister("ty");
	int endx = tx + static_cast<int>(std::cos(angle) * drawAmount);
	int endy = ty + static_cast<int>(std::sin(angle) * drawAmount);
	if (GetFlag("pen")) {
		mImageDrawer.line_segment(tx, ty, endx, endy);
	}
	SetRegister("tx", endx);
	SetRegister("ty", endy);
}