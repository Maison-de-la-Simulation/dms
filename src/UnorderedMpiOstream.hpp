#pragma once

#include "DistributedMemorySession.hpp"

#include <iostream>

class UnorderedMpiOstream;

using umaostream = UnorderedMpiOstream;

extern UnorderedMpiOstream umpicerr;
extern UnorderedMpiOstream umpiclog;
extern UnorderedMpiOstream umpicout;

//! This class provides the user with a few print options
class UnorderedMpiOstream
{
public:
    enum : int { All = -1 };

    UnorderedMpiOstream(int rank = Session::getIOProc(),
                        std::ostream& os = std::cout) noexcept;

    template <typename T>
    UnorderedMpiOstream& operator<<(const T& t);
    UnorderedMpiOstream& operator<<(std::ostream& (*func)(std::ostream&));

    UnorderedMpiOstream& fromAllProc() noexcept;
    UnorderedMpiOstream& fromIOProc() noexcept;
    UnorderedMpiOstream& fromRankProc(int rank) noexcept;

private:
    bool shouldPrint() const noexcept;

    std::ostream& m_os;
    int m_rank;
};

template <typename T>
UnorderedMpiOstream& UnorderedMpiOstream::operator<<(const T& t)
{
    if (shouldPrint())
    {
        m_os << t;
    }
    return *this;
}
