#pragma once
#include <exception>

class FatalException : public virtual std::exception
{
    public:
        const char* what() const noexcept override {return "Fatal Exception";}
        int getExceptionCode() {return mExceptionCode;}
        void setExceptionCode(int n) {mExceptionCode = n;}
    private:
        int mExceptionCode;
};

class NonfatalException : public virtual std::exception
{
     public:
        const char* what() const noexcept override {return "Nonfatal Exception";}
        int getExceptionCode() {return mExceptionCode;}
        void setExceptionCode(int n) {mExceptionCode = n;}
    private:
        int mExceptionCode;
};