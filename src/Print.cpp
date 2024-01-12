#include "Print.hpp"
#include "DistributedMemorySession.hpp"
#include <ostream>

std::ostream& operator<<(std::ostream& os, Print& print)
{
    if (print.m_allowed_to_print)
    {
        os << print.m_ss.str();
    }
    print.m_ss.str("");
    print.m_ss.clear();
    return os;
}

Print::Print(std::ostream& os)
    : m_allowed_to_print {Session::isIOProc()}
    , m_os               {os}
    , m_ss               {}
{
}

Print::Print(int rank, std::ostream& os)
    : m_allowed_to_print {rank < 0 || rank == Session::getRank()}
    , m_os               {os}
    , m_ss               {}
{
}

Print::~Print()
{
    if (m_allowed_to_print)
    {
        m_os << m_ss.str() << std::flush;
    }
}

Print& Print::SetPrecision(int p)
{
    if (m_allowed_to_print)
    {
        m_ss.precision(p);
    }
    return *this;
}

Print& Print::operator<<(std::ostream& (*func)(std::ostream&))
{
    if (m_allowed_to_print)
    {
        m_ss << func;
    }
    return *this;
}
