#pragma once
#include "Parser.hpp"
#include <string>
#include <tuple>
#include <vector>
#include <memory>
#include <algorithm>


struct Op // NOLINT
{
	virtual const char* GetName() const = 0;
	virtual void Parse(const std::string& params) = 0;
	virtual void Execute(class Machine& machine) = 0;
	virtual ~Op() = default;
};

template <typename... Args>
struct OpBase : Op
{
	const char* GetName() const override {}
	void Parse(const std::string& params) override
	{
		// 1) Split
		std::vector<std::string> args = Split(params);
		// 2) Reverse
		std::reverse(args.begin(), args.end());
		// 3) Generate Tuple
		mParams = ParseStr<Args...>(args);
	}
	void Execute(class Machine& machine) override
	{}
	std::tuple<Args...> mParams;
};

struct Exit : OpBase<>
{
	const char* GetName() const override { return "exit"; }
	void Execute(class Machine& machine) override;
};

struct MovI : OpBase<std::string, int>
{
	const char* GetName() const override { return "movi"; }
	void Execute(class Machine& machine) override;
};

struct Add : OpBase<std::string, std::string, std::string>
{
	const char* GetName() const override { return "add"; }
	void Execute(class Machine& machine) override;
};

struct Mov : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "mov"; }
	void Execute(class Machine& machine) override;
};

struct Sub : OpBase<std::string, std::string, std::string>
{
	const char* GetName() const override { return "sub"; }
	void Execute(class Machine& machine) override;
};

struct Mul : OpBase<std::string, std::string, std::string>
{
	const char* GetName() const override { return "mul"; }
	void Execute(class Machine& machine) override;
};

struct Div : OpBase<std::string, std::string, std::string>
{
	const char* GetName() const override { return "div"; }
	void Execute(class Machine& machine) override;
};

struct Inc : OpBase<std::string>
{
	const char* GetName() const override { return "inc"; }
	void Execute(class Machine& machine) override;
};

struct Dec : OpBase<std::string>
{
	const char* GetName() const override { return "dec"; }
	void Execute(class Machine& machine) override;
};

struct Jmp : OpBase<std::string>
{
	const char* GetName() const override { return "jmp"; }
	void Execute(class Machine& machine) override;
};

struct Cmpeq : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "cmpeq"; }
	void Execute(class Machine& machine) override;
};

struct Cmplt : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "cmplt"; }
	void Execute(class Machine& machine) override;
};

struct Jt : OpBase<std::string>
{
	const char* GetName() const override { return "jt"; }
	void Execute(class Machine& machine) override;
};

struct Jnt : OpBase<std::string>
{
	const char* GetName() const override { return "jnt"; }
	void Execute(class Machine& machine) override;
};

struct Push : OpBase<std::string>
{
	const char* GetName() const override { return "push"; }
	void Execute(class Machine& machine) override;
};

struct Pop : OpBase<std::string>
{
	const char* GetName() const override { return "pop"; }
	void Execute(class Machine& machine) override;
};

struct Load : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "load"; }
	void Execute(class Machine& machine) override;
};

struct Store : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "store"; }
	void Execute(class Machine& machine) override;
};

struct LoadI : OpBase<std::string, int>
{
	const char* GetName() const override { return "loadi"; }
	void Execute(class Machine& machine) override;
};

struct StoreI : OpBase<int, std::string>
{
	const char* GetName() const override { return "storei"; }
	void Execute(class Machine& machine) override;
};

struct LoadSc : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "loadsc"; }
	void Execute(class Machine& machine) override;
};

struct StoreSc : OpBase<std::string, std::string>
{
	const char* GetName() const override { return "storesc"; }
	void Execute(class Machine& machine) override;
};

struct Pendown : OpBase<>
{
	const char* GetName() const override {return "pendown";}
	void Execute(class Machine& machine) override;
};

struct Penup : OpBase <>
{
	const char* GetName() const override {return "penup";}
	void Execute(class Machine& machine) override;
};

struct Fwd : OpBase <std::string>
{
	const char* GetName() const override {return "fwd";}
	void Execute(class Machine& machine) override;
};

struct Back : OpBase <std::string>
{
	const char* GetName() const override {return "back";}
	void Execute(class Machine& machine) override;
};

// CreateOp: makes shared pointer to desired op subclass T
template <typename T>
std::shared_ptr<Op> CreateOp() {
	return std::make_shared<T>();
}