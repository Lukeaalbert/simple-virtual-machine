#pragma once
#include "Ops.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>
#include "../img/bitmap_image.hpp"
#include "Exceptions.h"

class Machine
{
public:
	Machine();

	void LoadMachineCode(const std::string& fileName);

	int GetRegister(const std::string& regName);

	void SetRegister(const std::string& regName, int n);

	bool GetFlag(const std::string& flagName);

	void SetFlag(const std::string& flagName, bool f);

	void PushStack(int i);

	int PopStack();

	int GetStack(int i);

	void SetStack(int idx, int value);

	void DrawTurtle(int drawAmount);

	void Execute();

private:
	// operations, in order
	std::vector<std::shared_ptr<Op>> mOps;
	// CreatorMap (maps op names to a shared pointer of their struct)
	std::unordered_map<std::string, std::function<std::shared_ptr<Op>()>> mCreatorMap;
	// Map containing registers
	std::unordered_map<std::string, int> mRegisters;
	// Map containing flags
	std::unordered_map<std::string, bool> mFlags;
	// Program Stack
	std::vector<int> mStack;
	// 24-bit per pixel bitmap image. 320 x 240.
	bitmap_image mBitmapImage;
	// Image drawer
	image_drawer mImageDrawer;
	// Int ID to RGB color value for image drawer pen
	std::unordered_map<int, std::tuple<int, int, int> > mColorMap;
};
