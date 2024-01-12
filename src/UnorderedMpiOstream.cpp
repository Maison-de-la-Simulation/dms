#include "UnorderedMpiOstream.hpp"

#include "DistributedMemorySession.hpp"

#include <ostream>

UnorderedMpiOstream umpicerr(Session::getIOProc(), std::cerr);
UnorderedMpiOstream umpiclog(Session::getIOProc(), std::clog);
UnorderedMpiOstream umpicout(Session::getIOProc(), std::cout);

UnorderedMpiOstream::UnorderedMpiOstream(int rank, std::ostream& os) noexcept
    : m_os   (os)
    , m_rank (rank)
{
    if (rank == All)
    {
        m_rank = Session::getRank();
    }
}

UnorderedMpiOstream& UnorderedMpiOstream::fromAllProc() noexcept
{
    return fromRankProc(All);
}

UnorderedMpiOstream& UnorderedMpiOstream::fromIOProc() noexcept
{
    return fromRankProc(Session::getIOProc());
}

UnorderedMpiOstream& UnorderedMpiOstream::fromRankProc(int rank) noexcept
{
    if (rank == All)
    {
        m_rank = Session::getRank();
    }
    else
    {
        m_rank = rank;
    }
    return *this;
}

// This operator is needed because manipulators coming from <iostream>
// are also templated. Without this operator "umpicout << std::endl"
// cannot be resolved due to both templated definitions of std::endl
// and UnorderedMpiOstream::operator<<.
UnorderedMpiOstream& UnorderedMpiOstream::operator<<(std::ostream& (*func)(std::ostream&))
{
    if (shouldPrint())
    {
        m_os << func;
    }
    return *this;
}

bool UnorderedMpiOstream::shouldPrint() const noexcept
{
    return m_rank == Session::getRank();
}
