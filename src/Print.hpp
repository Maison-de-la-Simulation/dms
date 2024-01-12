#pragma once

#include <iostream>
#include <sstream>
#include <string>

//! This class provides the user with a few print options
class Print
{
    friend std::ostream& operator<<(std::ostream& os, Print& print);

public:
    constexpr static int AllProcs = -1;

    Print(std::ostream& os = std::cout);
    Print(int rank, std::ostream& os = std::cout);
    ~Print();

    Print& SetPrecision(int p);

    template <typename T>
    Print& operator<<(const T& t);
    Print& operator<<(std::ostream& (*func)(std::ostream&));

private:
    const bool m_allowed_to_print;
    std::ostream& m_os;
    std::ostringstream m_ss;
};

//! Print on all processors of the default communicator
class AllPrint : public Print
{
public:
    //! Example: AllPrint() << " x = " << x << std::endl;
    AllPrint (std::ostream& os = std::cout)
        : Print {Print::AllProcs, os}
    {}
};

template <typename T>
Print& Print::operator<<(const T& t)
{
    if (m_allowed_to_print)
    {
        m_ss << t;
    }
    return *this;
}
